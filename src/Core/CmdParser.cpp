#include "Core/CmdParser.hpp"
#include <string>
#include <utility>

CmdParser::CmdParser(int argc, char *argv[]) {
  // TODO implement argument handling
  m_arguments.insert(std::make_pair("-w", "800"));
  m_arguments.insert(std::make_pair("-h", "800"));
  m_arguments.insert(std::make_pair("-o", "test"));
}

std::string CmdParser::getOptStr(const std::string &option) {
  return m_arguments.at(option);
}
unsigned int CmdParser::getOptUint(const std::string &option) {
  std::string val = m_arguments.at(option);
  return std::stoul(val, nullptr, 0);
}
