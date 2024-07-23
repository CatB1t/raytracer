#include "Core/CmdParser.hpp"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

CmdParser::CmdParser(int argc, char *argv[]) {

  m_argumentsTypes.insert(std::make_pair("-w", "uint"));
  m_argumentsTypes.insert(std::make_pair("-h", "uint"));
  m_argumentsTypes.insert(std::make_pair("-o", "str"));
  m_argumentsTypes.insert(std::make_pair("-c", "str"));
  m_argumentsTypes.insert(std::make_pair("-g", "str"));

  // Default values
  m_arguments.insert(std::make_pair("-w", "800"));
  m_arguments.insert(std::make_pair("-h", "800"));
  m_arguments.insert(std::make_pair("-o", "image"));
  m_arguments.insert(std::make_pair("-c", "scene.lua"));

  _cacheOptions(argc, argv);
}

bool CmdParser::isValidOption(const std::string &option) {
  return !(m_arguments.find(option) == m_arguments.end());
}

bool CmdParser::_isOption(const std::string &option) {
  return !(m_argumentsTypes.find(option) == m_argumentsTypes.end());
}

void CmdParser::_cacheOptions(int argc, char *argv[]) {
  m_arguments.insert(std::make_pair("_exec_path_", argv[0]));

  for (int i = 1; i < argc; ++i) {
    char *str = argv[i];

    if (str[0] != '-')
      continue;

    if (_isOption(str)) {
      if (i + 1 >= argc) {
        printf("Missing value for option %s\n", str);
        break;
      }

      ++i;
      std::string &arg_type = m_argumentsTypes.at(str);
      if (arg_type.compare("uint") == 0) {
        try {
          unsigned int temp = std::stoul(argv[i]);
        } catch (std::invalid_argument e) {
          printf("Invalid value for option %s\n", str);
          continue;
        }
      }

      m_arguments[str] = argv[i];
    } else {
      printf("Invalid option %s\n", str);
    }
  }
}

std::string CmdParser::getOptStr(const std::string &option) {
  return m_arguments.at(option);
}
unsigned int CmdParser::getOptUint(const std::string &option) {
  std::string val = m_arguments.at(option);
  return std::stoul(val, nullptr, 0);
}
