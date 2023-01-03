#pragma once

#include "http.hpp"
#include <memory>
#include <string>

class Socket {
protected:
  int fd = 0;
  Frame current_frame{};

public:
  ~Socket();
};

class TCP : public Socket {
  CLI commands;

  void Send(const std::string& str) const;

public:
  void Download(const std::string_view& sv, const byte_pair& chunk,
                int iteration);

  Frame Query();

  explicit TCP(const CLI& cli);
};
