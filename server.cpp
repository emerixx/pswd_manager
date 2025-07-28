#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <iostream>

int main() {
  // Create SSLServer with paths to your SSL certificate and private key
  httplib::SSLServer svr("server.crt", "server.key");

  if (!svr.is_valid()) {
    std::cerr << "Failed to start SSL server. Check your cert and key paths.\n";
    std::cout << "Failed to start SSL server. Check your cert and key paths.\n";
    return 1;
  }

  // Define routes
  svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
    res.set_content("Hello HTTPS from cpp-httplib!", "text/plain");
  });

  std::cout << "server starting\n";
  // Listen on all interfaces with port 443 (default HTTPS port)
  svr.listen("0.0.0.0", 8080);
  std::cout << "server stopping\n";
  return 0;
}
