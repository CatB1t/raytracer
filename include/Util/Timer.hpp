#pragma once

#include <chrono>

class Timer {
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;

public:
  void start() { m_startTime = std::chrono::high_resolution_clock::now(); };
  void end() { m_endTime = std::chrono::high_resolution_clock::now(); };
  std::chrono::milliseconds getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime);
  };
};
