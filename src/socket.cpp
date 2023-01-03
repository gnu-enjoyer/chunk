#include "socket.h"

#include <arpa/inet.h>
#include <netinet/in.h>

Socket::~Socket() { close(fd); }

void TCP::Send(const std::string& str) const {
  write(fd, str.data(), str.size());
}

void TCP::Download(const std::string_view& sv, const byte_pair& chunk,
                   int iteration) {

  const std::string chunk_range =
      "Range: bytes=" + chunk.first + "-" + chunk.second;

  Send(commands.BuildPayload<HTTP::GET>({"Connection: close", chunk_range}));

  const std::string file_name = sv.data() + std::to_string(iteration);

  auto stream = std::fstream(file_name, std::ios::app | std::ios_base::binary);

  while (true) {
    current_frame.buffer = {0};

    auto incoming =
        recv(fd, current_frame.buffer.data(), current_frame.buffer.size(), 0);

    if (incoming > 0)
      current_frame.Write(stream, incoming);
    else
      break;
  }
}

Frame TCP::Query() {

  Send(commands.BuildPayload<HTTP::HEAD>({"Connection: close"}));

  while (recv(fd, current_frame.buffer.data(), current_frame.buffer.size(), 0) >
         0)
    if (current_frame.ParseHeaders())
      return current_frame;

  return {};
}

TCP::TCP(const CLI& cli) : commands(cli) {
  sockaddr_in remote{AF_INET, htons(cli.GetPort()), in_addr{}, {0}};

  try {
    inet_pton(AF_INET, cli.host.data(), &remote.sin_addr);

    fd = socket(AF_INET, SOCK_STREAM, 0);

    connect(fd, (sockaddr*)&remote, sizeof(remote));

  } catch (...) {
    throw std::runtime_error("Could not setup socket.");
  }
}
