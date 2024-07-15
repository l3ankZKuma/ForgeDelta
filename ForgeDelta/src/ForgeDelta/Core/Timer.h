#pragma once

#include "Log.h"
#include <chrono>
#include <string>
#include<string_view>

namespace ForgeDelta {

  class Timer
  {
  public:
    Timer(const std::string_view name)
      : m_Name(name), m_Stopped(false) {
      m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
      if (!m_Stopped)
        Stop();
    }

    void Stop() {
      auto endTimepoint = std::chrono::high_resolution_clock::now();
      auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
      auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

      auto duration = end - start;
      double ms = duration * 0.001;

      FD_CORE_INFO("{0} took {1} ms", m_Name, ms);

      m_Stopped = true;
    }

  private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
  };

}
