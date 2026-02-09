#pragma once
#include <curl/curl.h>

class CurlHeaders
{
public:
    CurlHeaders() : m_headers(nullptr)
    {
    }

    ~CurlHeaders()
    {
        if (m_headers)
            curl_slist_free_all(m_headers);
    }

    CurlHeaders(const CurlHeaders&) = delete;
    CurlHeaders& operator=(const CurlHeaders&) = delete;

    CurlHeaders(CurlHeaders&& other) noexcept : m_headers(other.m_headers)
    {
        other.m_headers = nullptr;
    }

    CurlHeaders& operator=(CurlHeaders&& other) noexcept
    {
        if (this != &other)
        {
            if (m_headers)
                curl_slist_free_all(m_headers);

            m_headers = other.m_headers;
            other.m_headers = nullptr;
        }

        return *this;
    }

    void Append(const char* header)
    {
        m_headers = curl_slist_append(m_headers, header);
    }

    [[nodiscard]] curl_slist* Get() const
    {
        return m_headers;
    }

private:
    curl_slist* m_headers;
};
