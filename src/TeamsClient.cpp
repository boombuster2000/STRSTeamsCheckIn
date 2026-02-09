#include "TeamsClient.h"

#include <algorithm>
#include <cstdint>
#include <curl/curl.h>
#include <iostream>
#include <utility>

#include "curl_wrappers/CurlHandle.h"
#include "curl_wrappers/CurlHeaders.h"

TeamsClient::TeamsClient(std::string token) : m_token(std::move(token))
{
}

Response TeamsClient::CheckIn(const std::string& location) const
{
    const std::string baseUrl = "https://teamsapps.strschool.co.uk/api/touchreg/mobileCheckIn/";
    std::string locationCode = ToBase64(location);

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

std::string TeamsClient::ToBase64(const std::string& input)
{
    static constexpr char kBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string output;
    output.reserve(((input.size() + 2) / 3) * 4);

    size_t i = 0;
    while (i < input.size())
    {
        std::uint32_t triple = 0;
        int bytes_collected = 0;

        for (int j = 0; j < 3; ++j)
        {
            triple <<= 8;
            if (i < input.size())
            {
                triple |= static_cast<unsigned char>(input[i++]);
                ++bytes_collected;
            }
        }

        for (int j = 0; j < 4; ++j)
        {
            if (j <= bytes_collected)
            {
                output.push_back(kBase64Table[(triple >> (18 - j * 6)) & 0x3F]);
            }
            else
            {
                output.push_back('=');
            }
        }
    }

    return output;
}

size_t TeamsClient::WriteCallback(void* data, const size_t elementSize, const size_t elementCount, void* userContext)
{
    const size_t totalSize = elementSize * elementCount;
    auto* response = static_cast<std::string*>(userContext);
    response->append(static_cast<char*>(data), totalSize);
    return totalSize;
}
