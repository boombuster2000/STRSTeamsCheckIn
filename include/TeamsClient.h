#pragma once
#include <string>

class TeamsClient
{
public:
    explicit TeamsClient(std::string token);
    ~TeamsClient() = default;

    [[nodiscard]] bool CheckIn(const std::string& location) const;

private:
    static std::string ToBase64(const std::string& input);
    static size_t WriteCallback(void* data, size_t elementSize, size_t elementCount, void* userContext);

private:
    std::string m_token;
};