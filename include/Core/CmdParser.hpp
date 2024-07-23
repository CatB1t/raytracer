#pragma once

#include <string>
#include <unordered_map>

class CmdParser {
public:
  CmdParser(int argc, char *argv[]);
  std::string getOptStr(const std::string& option);
  unsigned int getOptUint(const std::string& option);
  bool isValidOption(const std::string& opt);
private:
  bool _isOption(const std::string& opt);
  void _cacheOptions(int argc, char *argv[]);
  std::unordered_map<std::string, std::string> m_arguments;
  std::unordered_map<std::string, std::string> m_argumentsTypes;
};
