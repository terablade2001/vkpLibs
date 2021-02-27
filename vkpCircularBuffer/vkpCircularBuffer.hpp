// MIT License

// Copyright (c) 2017 - 2020 Vasileios Kon. Pothos (terablade2001)
// https://github.com/terablade2001/vkpLibs

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

#ifndef __ECSOBJ__
  #error "CECS library required (https://github.com/terablade2001/CECS)"
#endif
#include <CECS.hpp>

namespace vkp {

template <class T>
class vkpCircularBuffer {
  public:
  vkpCircularBuffer(std::string& name_, int64_t size_);
  vkpCircularBuffer(std::string&& name_, int64_t size_);
  vkpCircularBuffer(std::string& name_);
  vkpCircularBuffer(std::string&& name_);
  ~vkpCircularBuffer();

  int init(int64_t size_);
  int resize(int64_t size_);
  T* data();
  int64_t size();
  int clear();

  int setIndex(int64_t pos_);
  int64_t getIndex(int64_t pos_ = 0);


  T& get();
  T& get(int64_t pos_);
  T& getMove(int64_t pos_, int64_t moveSteps_);
  T& getRelativeMove(int64_t pos_=0, int64_t moveSteps_=1);
  int set(T& value_);
  int set(T&& value_);
  int set(int64_t pos_, T& value_);
  int setMove(T& value_, int64_t moveSteps_);
  int setMove(T&& value_, int64_t moveSteps_);
  int move(int64_t moveSteps_=1);

  int allSetTo(T& value_);
  int allSetTo(T&& value_);

  std::string apiVersion();

  private:
  std::string name;
  int status;
  int64_t idx;
  std::vector<T> buffer;
  vkpCircularBuffer();
};

}; // namespace vkp
