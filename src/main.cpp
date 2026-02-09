#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "CLI/CLI.hpp"
#include "TeamsClient.h"

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
    if (const std::filesystem::path tokenFilePath = std::filesystem::path(configDir) / "token";
        !std::filesystem::exists(tokenFilePath))
    {
        std::ofstream file(tokenFilePath);
        file.close();

        // Set permissions to 600 (owner read/write only)
        std::filesystem::permissions(tokenFilePath,
                                     std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
                                     std::filesystem::perm_options::replace);
    }

    return configDir / "token";
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

int main(const int argc, char* argv[])
{
    CLI::App app{"Teams Check-in CLI"};
    argv = app.ensure_utf8(argv);

    // SetToken subcommand
    std::string token;
    const auto setToken_cmd = app.add_subcommand("setToken", "Command used to add your token");
    setToken_cmd->add_option("token", token, "The token for your teams account.")->required();
    setToken_cmd->callback([&token]() { SetToken(token); });

    // CheckIn subcommand
    std::string location;
    const auto checkIn_cmd = app.add_subcommand("checkin", "Command used to sign in.");
    checkIn_cmd->add_option("location", location, "The location you want to sign in at.")->required();
    checkIn_cmd->callback([&location]() {
        try
        {
            const std::string savedToken = GetToken();
            const TeamsClient client(savedToken);
            auto [statusCode, responseBody] = client.CheckIn(location);

            std::cout << "Status Code: " << statusCode << std::endl;
            std::cout << "Response: " << responseBody << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}