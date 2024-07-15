#pragma once

#include <chrono>
#include <unordered_map>
#include <string>
#include <mutex>
#include "ForgeDelta/Core/Log.h"

namespace ForgeDelta {

  class Timer {
  public:
    Timer() { Reset(); }
    void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }
    float Elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f; }
    float ElapsedMillis() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f; }
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
  };

  class ScopedTimer {
  public:
    ScopedTimer(const std::string& name)
      : m_Name(name) {}
    ~ScopedTimer() {
      float time = m_Timer.ElapsedMillis();
      FD_CORE_INFO("TIMER", "{0} - {1}ms", m_Name, time);
    }
  private:
    std::string m_Name;
    Timer m_Timer;
  };

  class PerformanceProfiler {
  public:
    struct PerFrameData {
      float Time = 0.0f;
      uint32_t Samples = 0;

      PerFrameData() = default;
      PerFrameData(float time) : Time(time) {}

      operator float() const { return Time; }
      inline PerFrameData& operator+=(float time) {
        Time += time;
        Samples++;
        return *this;
      }
    };

    void SetPerFrameTiming(const char* name, float time) {
      std::scoped_lock<std::mutex> lock(m_PerFrameDataMutex);

      if (m_PerFrameData.find(name) == m_PerFrameData.end())
        m_PerFrameData[name] = PerFrameData(time);
      else
        m_PerFrameData[name] += time;
    }

    void Clear() {
      std::scoped_lock<std::mutex> lock(m_PerFrameDataMutex);
      m_PerFrameData.clear();
    }

    const std::unordered_map<std::string, PerFrameData>& GetPerFrameData() const { return m_PerFrameData; }
  private:
    std::unordered_map<std::string, PerFrameData> m_PerFrameData;
    std::mutex m_PerFrameDataMutex;
  };

  class ScopePerfTimer {
  public:
    ScopePerfTimer(const char* name, PerformanceProfiler* profiler)
      : m_Name(name), m_Profiler(profiler) {}

    ~ScopePerfTimer() {
      float time = m_Timer.ElapsedMillis();
      m_Profiler->SetPerFrameTiming(m_Name, time);
    }
  private:
    const char* m_Name;
    PerformanceProfiler* m_Profiler;
    Timer m_Timer;
  };

#if 1
#define FD_SCOPE_PERF(name) \
    ForgeDelta::ScopePerfTimer timer##__LINE__(name, &ForgeDelta::Application::Get().GetPerformanceProfiler());

#define FD_SCOPE_TIMER(name) \
    ForgeDelta::ScopedTimer timer##__LINE__(name);
#else
#define FD_SCOPE_PERF(name)
#define FD_SCOPE_TIMER(name)
#endif

} // namespace ForgeDelta
