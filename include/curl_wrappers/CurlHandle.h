#pragma once
#include <curl/curl.h>

class CurlHandle
{
public:
    CurlHandle() : m_curl(curl_easy_init())
    {
    }

    ~CurlHandle()
    {
        curl_easy_cleanup(m_curl);
    }

    CurlHandle(const CurlHandle&) = delete;
    CurlHandle& operator=(const CurlHandle&) = delete;

    CurlHandle(CurlHandle&& other) noexcept : m_curl(other.m_curl)
    {
        other.m_curl = nullptr;
    }

    CurlHandle& operator=(CurlHandle&& other) noexcept
    {
        if (this != &other)
        {
            if (m_curl)
                curl_easy_cleanup(m_curl);

            m_curl = other.m_curl;
            other.m_curl = nullptr;
        }

        return *this;
    }

    [[nodiscard]] CURL* Get() const
    {
        return m_curl;
    }

    explicit operator bool() const
    {
        return m_curl != nullptr;
    }

private:
    CURL* m_curl;
};
