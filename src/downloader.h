#pragma once

#include "http.hpp"
#include <memory>
#include <string>

class Downloader {
    unsigned int content_length = 0;
    std::optional <std::uint32_t> crc32_hash = std::nullopt;
    CLI cli_args;

    void Reassemble(const int n, const std::string_view &sv);

public:
    void StartDownload();

    explicit Downloader(const CLI &cli);

    ~Downloader();
};
