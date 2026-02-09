#include "TeamsClient.h"

#include <algorithm>
#include <curl/curl.h>
#include <iostream>
#include <utility>

#include "base64.hpp"

#include "curl_wrappers/CurlHandle.h"
#include "curl_wrappers/CurlHeaders.h"

TeamsClient::TeamsClient(std::string token) : m_token(std::move(token))
{
}

Response TeamsClient::CheckIn(const std::string& location) const
{
    const std::string baseUrl = "https://teamsapps.strschool.co.uk/api/touchreg/mobileCheckIn/";
    std::string locationCode = base64::to_base64(location);

    std::ranges::replace(locationCode, '+', '-');
    std::ranges::replace(locationCode, '/', '_');

    const CurlHandle curl;

    if (!curl)
        return {0, "Failed to initialize CURL library."};

    const std::string url = baseUrl + locationCode;
    std::string responseBody;

    CurlHeaders headers;
    const std::string tokenHeader = "token: " + m_token;

    headers.Append("Content-Type: application/json");
    headers.Append("Accept: application/json");
    headers.Append(tokenHeader.c_str());

    curl_easy_setopt(curl.Get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.Get(), CURLOPT_HTTPHEADER, headers.Get());
    curl_easy_setopt(curl.Get(), CURLOPT_POST, 1L);
    curl_easy_setopt(curl.Get(), CURLOPT_POSTFIELDS, "");

    curl_easy_setopt(curl.Get(), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl.Get(), CURLOPT_WRITEDATA, &responseBody);

    curl_easy_setopt(curl.Get(), CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl.Get(), CURLOPT_TIMEOUT, 30L);

    if (const CURLcode res = curl_easy_perform(curl.Get()); res != CURLE_OK)
    {
        std::string errorMessage = "Network request failed: ";
        errorMessage += curl_easy_strerror(res);
        std::cerr << errorMessage << "\n";
        return {0, errorMessage};
    }

    long statusCode = 0;
    curl_easy_getinfo(curl.Get(), CURLINFO_RESPONSE_CODE, &statusCode);

    return Response{statusCode, responseBody};
}

size_t TeamsClient::WriteCallback(void* data, const size_t elementSize, const size_t elementCount, void* userContext)
{
    const size_t totalSize = elementSize * elementCount;
    auto* response = static_cast<std::string*>(userContext);
    response->append(static_cast<char*>(data), totalSize);
    return totalSize;
}
