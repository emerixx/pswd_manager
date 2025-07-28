#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <ostream>
#include <sstream>
#include <string>
class mc {
public:
  static std::vector<std::string> split_by_space(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    // Extract words split by whitespace
    while (iss >> token) {
      tokens.push_back(token);
    }

    return tokens;
  }
  static std::string get_SHA256(const std::string &input) {
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
  bool is_pw_correct(std::string) { return false; }
};
int main() {
  std::ifstream file("pswd"); // open the file
  if (!file) {
    std::cerr << "Could not open the file\n";
    return 1;
  }
  std::string pw;
  std::getline(file, pw);
  file.close();

  std::string cmnd_full = "";

  while (cmnd_full != "exit") {
    std::cout << "[]->";
    std::getline(std::cin, cmnd_full);
    std::vector<std::string> cmnd = mc::split_by_space(cmnd_full);
    if(cmnd[0]=="hash_SHA256"){
      std::cout<<"Hashing input '"<<cmnd[1]<<"' with SHA256.\n";
      std::cout<<mc::get_SHA256(cmnd[1])<<std::endl;
    }
  }

  std::cout << "Exiting..." << std::endl;
  return 0;
}
