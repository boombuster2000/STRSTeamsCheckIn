#include "TeamsClient.h"

#include <algorithm>
#include <cstdint>
#include <curl/curl.h>
#include <iostream>
#include <utility>

TeamsClient::TeamsClient(std::string token) : m_token(std::move(token))
{
}

Response TeamsClient::CheckIn(const std::string& location) const
{
    const std::string baseUrl = "https://teamsapps.strschool.co.uk/api/touchreg/mobileCheckIn/";
    std::string locationCode = ToBase64(location);

    std::ranges::replace(locationCode, '+', '-');
    std::ranges::replace(locationCode, '/', '_');

    CURL* curl = curl_easy_init();
    if (!curl)
        return {0, "Failed to initialize CURL library."};

    const std::string url = baseUrl + locationCode;
    std::string responseBody;

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, std::string("token: " + m_token).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK)
    {
        std::string errorMessage = "Network request failed: ";
        errorMessage += curl_easy_strerror(res);

        std::cerr << errorMessage << "\n";
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return {0, errorMessage};
    }

    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

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