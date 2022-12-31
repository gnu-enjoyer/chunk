#include "downloader.h"
#include "http.hpp"
#include "socket.h"
#include <ctre.hpp>
#include <fstream>
#include <future>

static std::vector<byte_pair> CalcChunks(unsigned int num, unsigned int n) {

  std::vector<byte_pair> chunks;
  const unsigned int chunk_size = (num + n - 1) / n;

  int start = 0;

  for (int i = 0; i < n; i++) {
    int end = start + chunk_size;

    if (end > num)
      end = num;

    if (start != 0)
      ++start;

    chunks.emplace_back(std::to_string(start), std::to_string(end));
    start = end;
  }
  return chunks;
}

void Downloader::Reassemble(const int n, const std::string_view& sv) {
  std::string path(sv);

  std::ofstream out_stream(path, std::ios::app | std::ios::binary);

  if (!out_stream)
    return;

  for (int i = 0; i <= n; ++i) {
    std::ifstream in_stream(path + std::to_string(i), std::ios::binary);

    if (!in_stream)
      return;

    out_stream << in_stream.rdbuf();
  }
}

void Downloader::StartDownload() {
  auto thread_count = 3;

  auto args = cli_args;

  auto byte_chunks = CalcChunks(content_length, thread_count);

  std::vector<std::future<void>> futures{};

  int i = 0;

  for (i = 0; i < thread_count; i++) {
    futures.emplace_back(
        std::async(std::launch::async, [args, byte_chunks, i]() {
          auto tcp_socket = std::make_unique<TCP>(args);
          tcp_socket->Download(args.uri, byte_chunks[i], i);
        }));
  }

  for (auto& f : futures)
    f.wait();

  Reassemble(i, cli_args.uri);
}

static constexpr auto isCRC32(const std::string_view& sv) {
  return ctre::search<R"([A-Za-z0-9]+)">(sv);
}

Downloader::Downloader(const CLI& cli) : cli_args(cli) {
  auto tcp_socket = std::make_unique<TCP>(cli);

  auto response = tcp_socket->Query();

  content_length =
      std::stoi(response.GetHeader("Content-Length").value_or("0").data());

  auto etag = response.GetHeader("ETag").value_or("");

  if (auto m = isCRC32(etag))
    crc32_hash = std::stoi(m.get<0>().data());

  if (content_length < 1)
    throw std::runtime_error("HTTP error.");
}

Downloader::~Downloader() {}
