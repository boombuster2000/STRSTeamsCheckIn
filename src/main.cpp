#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "CLI/CLI.hpp"
#include "TeamsClient.h"

// Platform-specific includes for no-echo input
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

class EchoGuard
{
public:
    EchoGuard()
    {
#ifdef _WIN32
        hStdin = GetStdHandle(STD_INPUT_HANDLE);
        if (hStdin == INVALID_HANDLE_VALUE)
            return;

        if (!GetConsoleMode(hStdin, &original_mode))
            return;

        DWORD new_mode = original_mode & ~ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hStdin, new_mode))
            return;

        active = true;
#else
        if (!isatty(STDIN_FILENO))
            return;

        if (tcgetattr(STDIN_FILENO, &original_settings) != 0)
            return;

        termios new_settings = original_settings;
        new_settings.c_lflag &= ~ECHO;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &new_settings) != 0)
            return;

        active = true;
#endif
    }

    ~EchoGuard()
    {
        if (!active)
            return;

#ifdef _WIN32
        SetConsoleMode(hStdin, original_mode);
#else
        tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
#endif
    }

    [[nodiscard]] bool IsActive() const { return active; }

    EchoGuard(const EchoGuard&) = delete;
    EchoGuard& operator=(const EchoGuard&) = delete;
    EchoGuard(EchoGuard&&) = delete;
    EchoGuard& operator=(EchoGuard&&) = delete;

private:
    bool active{false};

#ifdef _WIN32
    HANDLE hStdin{INVALID_HANDLE_VALUE};
    DWORD original_mode{};
#else
    termios original_settings{};
#endif
};

std::filesystem::path GetTokenFilePath()
{
    std::filesystem::path configDir;

#ifdef _WIN32
    const char* appData = std::getenv("APPDATA");
    if (!appData)
        throw std::runtime_error("APPDATA environment variable not set");
    configDir = std::filesystem::path(appData) / "teams-checkin";
#elif __APPLE__
    const char* homeDir = std::getenv("HOME");
    if (!homeDir)
        throw std::runtime_error("HOME environment variable not set");

    configDir = std::filesystem::path(homeDir) / "Library" / "Application Support" / "teams-checkin";
#else
    const char* homeDir = std::getenv("HOME");
    if (!homeDir)
        throw std::runtime_error("HOME environment variable not set");

    configDir = std::filesystem::path(homeDir) / ".config" / "teams-checkin";
#endif

    // Create config directory if it doesn't exist
    if (!std::filesystem::exists(configDir))
        std::filesystem::create_directories(configDir);

    // Creates token file if it doesn't exist
    const std::filesystem::path tokenFilePath = configDir / "token";

    if (!std::filesystem::exists(tokenFilePath))
    {
        std::ofstream file(tokenFilePath);

        if (!file)
            throw std::runtime_error("Failed to create token file");

        file.close();

#ifndef _WIN32
        // Set permissions to 600 (owner read/write only) on Unix-like systems
        // Windows uses a different permission model (ACLs), so skip this
        std::filesystem::permissions(tokenFilePath,
                                     std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
                                     std::filesystem::perm_options::replace);
#endif
    }

    return tokenFilePath;
}

std::string ReadPasswordFromStdin()
{
    std::string password;
    {
        if (const EchoGuard guard; !guard.IsActive())
            std::cerr << "Warning: Unable to hide input. Your token will be visible on screen." << std::endl;


        if (!std::getline(std::cin, password))
            throw std::runtime_error("Failed to read input");
    }

    std::cout << std::endl; // Move to a new line since Enter wasn't echoed
    return password;
}

std::string GetTokenFromUser()
{
    // First, try to get token from environment variable
    if (const char* envToken = std::getenv("TEAMS_TOKEN"); envToken && envToken[0] != '\0')
    {
        std::cout << "Using token from TEAMS_TOKEN environment variable." << std::endl;
        return {envToken};
    }

    // Fall back to prompting user for input (with no echo)
    std::cout << "Enter your token (input hidden): ";
    std::string token = ReadPasswordFromStdin();

    if (token.empty())
        throw std::runtime_error("Token cannot be empty");

    return token;
}

void SetToken(const std::string& token)
{
    const auto tokenFilePath = GetTokenFilePath();

    std::ofstream file(tokenFilePath);
    if (!file)
        throw std::runtime_error("Failed to open token file for writing");

    file << token;
    std::cout << "Token saved successfully to " << tokenFilePath << std::endl;
}

std::string GetToken()
{
    auto tokenFilePath = GetTokenFilePath();

    if (!std::filesystem::exists(tokenFilePath))
        throw std::runtime_error("Token file not found. Please run 'setToken' first.");

    std::ifstream file(tokenFilePath);
    if (!file)
        throw std::runtime_error("Failed to open token file for reading");

    std::string token;
    std::getline(file, token);

    if (token.empty())
        throw std::runtime_error("Token file is empty. Please run 'setToken' first.");
    return token;
}

void PrintReasonsForEmptyBody()
{
    std::cout << "Make sure you are connected to school WiFi.\n";
    std::cout << "Make sure your token is correct.\n";
    std::cout << "Make sure your token hasn't expired (when password changes).\n";
}

int main(const int argc, char* argv[])
{
    CLI::App app{"Teams Check-in CLI"};
    argv = app.ensure_utf8(argv);

    // SetToken subcommand (no positional argument)
    const auto setToken_cmd = app.add_subcommand("setToken", "Command used to add your token");
    setToken_cmd->callback([]() {
        try
        {
            std::string token = GetTokenFromUser();
            SetToken(token);
        }
        catch (std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    });

    // CheckIn subcommand
    std::string location;
    const auto checkIn_cmd = app.add_subcommand("checkin", "Command used to sign in.");
    checkIn_cmd->add_option("location", location, "The location you want to sign in at.")->required();
    checkIn_cmd->callback([&location]() {
        try
        {
            const std::string savedToken = GetToken();
            const TeamsClient client(savedToken);

            if (auto [statusCode, responseBody] = client.CheckIn(location); statusCode == 204)
            {
                std::cout << "Not signed in!\n";
                PrintReasonsForEmptyBody();
            }
            else
            {
                std::cout << "Status Code: " << statusCode << std::endl;
                std::cout << "Response: " << responseBody << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
