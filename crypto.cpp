#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <ostream>
#include <sstream>

class SHA256Hasher {
public:
  static std::string ComputeSHA256(const std::string &input) {
    unsigned char hash[SHA256_DIGEST_LENGTH]; // 32 bytes

    // Compute the SHA-256 hash
    SHA256(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(),
           hash);

    std::ostringstream hexStream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
      // Convert each byte to hex, padded with zero if needed
      hexStream << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(hash[i]);
    }
    return hexStream.str();
  }
};

int main() {
  std::cout << "hello world" << std::endl;
  // login to the db

  return 0;
}
