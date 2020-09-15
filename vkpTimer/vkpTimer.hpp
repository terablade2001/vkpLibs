// MIT License

// Copyright (c) 2017 - 2020 Vasileios Kon. Pothos (terablade2001)
// https://github.com/terablade2001

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifdef _WIN32
  #include <Windows.h>
#endif
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <ctime>
#ifdef __unix__
#include <sys/time.h>
#endif

#define VKPTIMER_VERSION (0.101)

class vkpTimer {
private:
  bool logFlag;
  float diffTime;
  bool calculate;
  float totalTime;
public:
  std::string name;
  size_t count;
  bool enabled;

#ifndef _WIN32
  timeval startVal;
  timeval stopVal;
#else
  LARGE_INTEGER lpFrequency;
  LARGE_INTEGER startVal;
  LARGE_INTEGER stopVal;
#endif
  std::vector<float> log;

  vkpTimer();
  vkpTimer(std::string timerName);
  void setLogging(bool enableLogging);
  void setEnable(bool enableTimer);
  void start();
  void stop();
  float getTimeDiff();
  std::vector<float>& getTimeLog();
  float getTotalTime();
  float getAverageTime();
  void reset();

  std::string str();
};


class vkpTimersMap {
  public:
  bool enabled;
  std::string name;
  std::map<std::string, vkpTimer> map;

  vkpTimersMap();
  vkpTimersMap(std::string mapName);
  void setName(const char* timersMapName);
  vkpTimer& addTimer(const char* timerName);
  vkpTimer& getTimer(const char* timerName);
  void clearTimers();
  std::string str();
};








inline void vkpTimer::start() {
  if (!enabled) return;
  calculate=true;
#ifndef _WIN32
  gettimeofday(&startVal, NULL);
#else
  QueryPerformanceCounter(&startVal);
#endif
}

inline void vkpTimer::stop() {
  if (!enabled) return;
#ifndef _WIN32
  gettimeofday(&stopVal, NULL);
#else
  QueryPerformanceCounter(&stopVal);
#endif
  count++;
  getTimeDiff();
}
