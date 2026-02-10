#include "TeamsClient.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "base64.hpp"
#include "cpr/cpr.h"


TeamsClient::TeamsClient(std::string token) : m_token(std::move(token))
{
}

Response TeamsClient::CheckIn(const std::string& location) const
{
    const std::string baseUrl = "https://teamsapps.strschool.co.uk/api/touchreg/mobileCheckIn/";
    std::string locationCode = base64::to_base64(location);

    std::ranges::replace(locationCode, '+', '-');
    std::ranges::replace(locationCode, '/', '_');

    const std::string url = baseUrl + locationCode;

    const auto response = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"Accept", "application/json"},
            {"token", m_token}
        },
        cpr::Body{""},
        cpr::Timeout{30000},
        cpr::ConnectTimeout{10000}
    );

    if (response.error)
    {
        const std::string errorMessage = "Network request failed: " + response.error.message;
        std::cerr << errorMessage << "\n";
        return {0, errorMessage};
    }

    return Response{response.status_code, response.text};
}