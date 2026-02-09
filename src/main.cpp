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

std::filesystem::path GetTokenFilePath()
{
    const char* homeDir = std::getenv("HOME");
    if (!homeDir)
        throw std::runtime_error("HOME environment variable not set");

    const std::filesystem::path configDir = std::filesystem::path(homeDir) / ".config" / "teams-checkin";

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

        // Set permissions to 600 (owner read/write only)
        std::filesystem::permissions(tokenFilePath,
                                     std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
                                     std::filesystem::perm_options::replace);
    }

    return tokenFilePath;
}

// Function to read password without echoing to terminal
std::string ReadPasswordFromStdin()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    std::string password;
    std::getline(std::cin, password);

    SetConsoleMode(hStdin, mode);
    std::cout << std::endl;
    return password;
#else
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::string password;
    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    return password;
#endif
}

std::string GetTokenFromUser()
{
    // First, try to get token from environment variable
    if (const char* envToken = std::getenv("TEAMS_TOKEN"); envToken && envToken[0] != '\0')
    {
        std::cout << "Using token from TEAMS_TOKEN environment variable." << std::endl;
        return std::string(envToken);
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
