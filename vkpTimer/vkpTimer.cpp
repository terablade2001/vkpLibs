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

#include "vkpTimer.hpp"

vkpTimer::vkpTimer(std::string timerName):
  logFlag(false),
  diffTime(0),
  calculate(false),
  totalTime(0),
  name(timerName),
  count(0),
  enabled(true)
{
#ifdef _WIN32
  QueryPerformanceFrequency(&lpFrequency);
#endif
}


vkpTimer::vkpTimer(): vkpTimer("") {
}

void vkpTimer::setLogging(bool enableLogging) {
  logFlag = enableLogging;
}

void vkpTimer::setEnable(bool enableTimer) {
  enabled=enableTimer;
}

float vkpTimer::getTimeDiff() {
  if (!enabled) return -1.0;
  if (calculate) {
    #ifndef _WIN32
      diffTime = (((stopVal.tv_sec + stopVal.tv_usec * 1e-6) - (startVal.tv_sec + startVal.tv_usec * 1e-6)) * 1000);
    #else
      diffTime = (stopVal.QuadPart - startVal.QuadPart) * 1000.0 / lpFrequency.QuadPart;
    #endif
    if (logFlag) log.push_back(diffTime);
    totalTime += diffTime;
    calculate = false;
  }
  return diffTime;
}

std::vector<float>& vkpTimer::getTimeLog() {
  return log;
}

float vkpTimer::getTotalTime() {
  if (!enabled) return -2.0;
  return totalTime;
}
float vkpTimer::getAverageTime() {
  if (!enabled) return -2.0;
  if (count == 0) return -1.0;
  return totalTime / ((float)count);
}

void vkpTimer::reset() {
  logFlag = false;
  diffTime = 0;
  count = 0;
  calculate = false;
  enabled = true;
  log.clear();
}


std::string vkpTimer::str() {
  std::stringstream ss;
  if (!enabled) { ss << "- Timer ["<<name<<"] is not enabled"; return ss.str(); }
  ss << "- Timer ["<<name<<"]: Avg = " << getAverageTime() << " ms. ("<<totalTime<<" / " << count<<")";
  return ss.str();
}















vkpTimersMap::vkpTimersMap(std::string mapName):
  enabled(true),
  name(mapName)
{}

vkpTimersMap::vkpTimersMap(): vkpTimersMap("")
{}

void vkpTimersMap::setName(const char* timersMapName) {
  if (timersMapName == nullptr) return;
  name = std::string(timersMapName);
}

vkpTimer& vkpTimersMap::addTimer(const char* timerName) {
  if (timerName == nullptr) { return getTimer("vkpTimer-dummy"); }
  map.emplace(std::string(timerName),vkpTimer(std::string(timerName)));
  return getTimer(timerName);
}

vkpTimer& vkpTimersMap::getTimer(const char* timerName) {
  if ((!enabled) || (timerName == nullptr)) {
    static vkpTimer dummy("vkpTimer-dummy");
    dummy.setEnable(false);
    return dummy;
  }
  try {
    vkpTimer& r = map.at(std::string(timerName));
    return r;
  } catch(std::exception& e) {
    addTimer(timerName);
    vkpTimer& r = map.at(std::string(timerName));
    return r;
  }
}

void vkpTimersMap::clearTimers() {
  map.clear();
}

void vkpTimersMap::resetTimers() {
  for (auto it = map.begin(); it != map.end(); it++) {
    it->second.reset();
  }
}

std::string vkpTimersMap::str() {
  std::stringstream ss;
  int timersUsed = 0;
  ss << "- Timers Map ["<<name<<"]:";
  if (!enabled) { ss<<" Disabled."; return ss.str(); }
  float totalAvgTime = 0.0f;
  for (auto it = map.begin(); it != map.end(); it++) {
    ss << std::endl << "  " << it->second.str();
    if (it->second.getAverageTime() >= 0) {
      totalAvgTime += it->second.getAverageTime();
      timersUsed++;
    }
  }
  ss << std::endl << " * Total Avg. Time: " << totalAvgTime << " ms, for "<<timersUsed<<"/"<<map.size()<<" timers.";
  return ss.str();
}
