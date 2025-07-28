#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <ostream>
#include <sstream>
#include <string>
#include <termios.h>
#include <unistd.h>
class cli {
public:
  static std::string getPassword() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // get current terminal attributes
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::string password;
    char ch;

    while (true) {
      ch = getchar(); // getchar() reads one char immediately because ICANON
                      // disabled

      if (ch == '\n' || ch == '\r') { // Enter key pressed
        std::cout << '\n';
        break;
      } else if (ch == '\t') {
        continue;
      } else if (ch == 127 || ch == 8) { // Backspace
        if (!password.empty()) {
          password.pop_back();
          std::cout << "\b \b" << std::flush; // erase last star
        }
      } else {
        password.push_back(ch);
        std::cout << '*' << std::flush;
      }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore terminal attributes

    return password;
  }

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
}

;
class mc {
public:
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
  static bool is_pw_correct(std::string test) {
    std::ifstream file("pswd"); // open the file
    if (!file) {
      std::cerr << "Could not open the file\n";
      return 1;
    }
    std::string pw;
    std::getline(file, pw);
    file.close();
    if (pw == get_SHA256(test))
      return true;
    return false;
  }
};
int main() {

  std::string cmnd_full = "";
  std::string sec_input = "";
  bool unlocked = false;
  int i = 0;
  while (cmnd_full != "exit") {
    std::cout << "[]->";
    std::getline(std::cin, cmnd_full);
    std::vector<std::string> cmnd = cli::split_by_space(cmnd_full);
    if (cmnd[0] == "hash_SHA256" && cmnd.size() == 2) {
      std::cout << "Hashing input '" << cmnd[1] << "' with SHA256.\n";
      std::cout << mc::get_SHA256(cmnd[1]) << std::endl;
    } else if (cmnd[0] == "unlock_db" && cmnd.size() == 1) {
      while (!unlocked && i < 3) {
        if (i > 0) {
          std::cout << "Incorrect password, try again\n";
          std::cout << "Enter the password (" << i << "/3 attempts):\n";
        } else {
          std::cout << "Enter the password:\n";
        }
        sec_input = cli::getPassword();
        unlocked = mc::is_pw_correct(sec_input);
        i++;
      }
      if (i == 3) {
        std::cout
            << "unlocking db failed, entered the wrong password 3 times.\n";
      }
      if(unlocked){
        std::cout<<"Database unlocked\n";
      }
      i = 0;
      sec_input = "";
    }
  }

  std::cout << "Exiting..." << std::endl;
  return 0;
}
