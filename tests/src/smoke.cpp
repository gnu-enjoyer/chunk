#include "downloader.h"
#include "http.hpp"

/*
 * * * * * Smoke Test
 * Checks  1.1.1.1:80
 * Should return: 301
 */

int main() {
  if (auto cli_args = CLI::Parse("1.1.1.1", "80", "index.html")) {
    auto DownloadManager = std::make_unique<Downloader>(cli_args.value());

    DownloadManager->StartDownload();
  } else
    return -1;
}