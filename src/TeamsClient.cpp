#include "TeamsClient.h"

#include <algorithm>
#include <cstdint>
#include <curl/curl.h>
#include <iostream>
#include <utility>

TeamsClient::TeamsClient(std::string token) : m_token(std::move(token))
{
}

bool TeamsClient::CheckIn(const std::string& location) const
{

    const std::string baseUrl = "https://teamsapps.strschool.co.uk/api/touchreg/mobileCheckIn/";
    std::string locationCode = ToBase64(location);

    std::ranges::replace(locationCode, '+', '-');
    std::ranges::replace(locationCode, '/', '_');

    CURL* curl = curl_easy_init();
    if (!curl)
        return false;

    const std::string url = baseUrl + locationCode;
    std::string response_body;

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, std::string("token: " + m_token).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << curl_easy_strerror(res) << "\n";
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }

    long status_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

    std::cout << "Status Code: " << status_code << "\n";
    std::cout << "Message: " << response_body << "\n";

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return true;
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