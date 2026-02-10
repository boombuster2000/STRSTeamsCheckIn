#pragma once
#include <string>

struct Response
{
    long statusCode;
    std::string responseBody;
};

class TeamsClient
{
public:
    explicit TeamsClient(std::string token);
    ~TeamsClient() = default;

    [[nodiscard]] Response CheckIn(const std::string& location) const;

private:
    std::string m_token;
};