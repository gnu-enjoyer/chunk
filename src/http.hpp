#pragma once

#include <array>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <string_view>
#include <numeric>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ranges>
#include <ctre.hpp>

typedef std::pair <std::string, std::string> byte_pair;

enum class HTTP {
    GET, HEAD
};

struct Frame {
    std::unordered_map <std::string_view, std::string_view> headers{};
    std::array<char, 4096> buffer{0};

    static constexpr std::string_view
    http_crlf{ "\r\n\r\n" }, http_delim{ "\r\n" }, hdr_delim{ ":" };

    std::optional <std::string_view> GetHeader(const std::string_view &key) {
        if (auto itr = headers.find(key); itr != headers.end())
            return std::make_optional(itr->second);

        return std::nullopt;
    }

    bool ParseHeaders() {
        headers.clear();

        for (auto &&[start, end]: std::views::split(buffer, http_delim)) {
            auto it = std::find(start, end, *hdr_delim.data());

            headers.emplace(std::string_view{start, it},
                            std::string_view{std::next(it), end});
        }

        return !headers.empty();
    }

    void Write(std::basic_fstream<char> &stream, ssize_t len) {
        auto itr = std::search(buffer.begin(), buffer.end(), http_crlf.begin(),
                               http_crlf.end());

        if (itr != buffer.end()) {
            auto dist = std::distance(buffer.begin(), itr);

            stream.write(
                    (buffer.begin() + dist + 4), (len - dist - 4));

            return;
        }

        stream.write(buffer.data(), len);
    };

};


consteval const char *EnumToString(HTTP method) {
    switch (method) {
        case HTTP::GET:
            return "GET /";
        case HTTP::HEAD:
            return "HEAD /";
    }
}

struct CLI {
    static constexpr bool isValidAddress(const std::string &str) {
        return ctre::match<R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)">(str);
    }

    std::string host, port, uri;

    template<HTTP method>
    std::string BuildPayload(std::vector <std::string> &&input) {
        std::string payload{EnumToString(method)};

        payload.reserve(128);

        payload += uri + " HTTP/1.1\r\n";

        input.emplace_back("Host: " + host);

        for (auto &i: input)
            payload += i + "\r\n";

        std::cout << payload;

        return payload + "\r\n\r\n";
    }

    int GetPort() const {
        return std::stoi(port);
    }

    bool Validate() {
        std::stoi(port);
        return isValidAddress(host);
    };

    template<typename... Args>
    static std::optional <CLI> Parse(Args &&... args) {
        try {
            auto out = CLI{std::forward<Args>(args)...};
            return out.Validate() ? out : std::optional < CLI > {std::nullopt};
        } catch (...) {
            return {};
        }
    }
};
