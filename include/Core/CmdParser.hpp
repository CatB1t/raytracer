#pragma once

#include <string>
#include <unordered_map>

class CmdParser {
public:
  CmdParser(int argc, char *argv[]);
  std::string getOptStr(const std::string& option);
  unsigned int getOptUint(const std::string& option);
private:
  std::unordered_map<std::string, std::string> m_arguments;
};
