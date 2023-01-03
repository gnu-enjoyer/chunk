#include "downloader.h"
#include "http.hpp"

#include <stdexcept>

/*
 * CLI args: IP PORT URI
 * e.g. 127.0.0.1 80 payload.bin
 */

int main(int argc, char* argv[]) {
  if (argc != 4)
    throw std::runtime_error("Wrong args provided, needs: IP PORT URI");

  if (auto cli_args = CLI::Parse(argv[1], argv[2], argv[3])) {
    auto DownloadManager = std::make_unique<Downloader>(cli_args.value());

    DownloadManager->StartDownload();
  } else
    return -1;
}