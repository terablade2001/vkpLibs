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
#include "../vkpVersion.hpp"
#include "vkpCircularBuffer.hpp"

namespace vkp {

CECS_MODULE("vkpCircularBuffer")

template <class T>
vkpCircularBuffer<T>::vkpCircularBuffer(std::string& name_):
  name(name_), status(-1), idx(0)
{}

template <class T>
vkpCircularBuffer<T>::vkpCircularBuffer(std::string&& name_):
  name(name_), status(-1), idx(0)
{}

template <class T>
vkpCircularBuffer<T>::vkpCircularBuffer(std::string& name_, int64_t size_):
  name(name_), status(-1), idx(0)
{
  _ERR(init(size_), "vkpCircularBuffer: Failed to initialize");
}

template <class T>
vkpCircularBuffer<T>::vkpCircularBuffer(std::string&& name_, int64_t size_):
  name(name_), status(-1), idx(0)
{
  _ERR(init(size_), "vkpCircularBuffer: Failed to initialize");
}

template <class T>
vkpCircularBuffer<T>::~vkpCircularBuffer()
{}

template <class T>
int vkpCircularBuffer<T>::init(int64_t size_) {
  _ERRI(status >= 0,"vkpCircularBuffer [%s]: Buffer is already used. First reset().", name.c_str())
  status = 1;
  idx = 0;
  _ERRI(resize(size_),"vkpCircularBuffer [%s]: Failed to resize buffer", name.c_str())
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::resize(int64_t size_) {
  _ERRI(status <  0, "vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  _ERRI(size_  <= 0, ("vkpCircularBuffer [%s]: size_ is <= 0."), name.c_str())
  try {
    buffer.resize(size_);
  } catch (std::exception& e) {
    _ERRI("Buffer failed to resize. Exception: [%s]", e.what())
  }
  return 0;
}

template <class T>
T* vkpCircularBuffer<T>::data() {
  _ERRN(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  return buffer.data();
}

template <class T>
int64_t vkpCircularBuffer<T>::size() {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  return buffer.size();
}

template <class T>
int vkpCircularBuffer<T>::clear() {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  status = -1;
  idx = 0;
  buffer.clear();
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::setIndex(int64_t pos_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  _ERRI(buffer.size() == 0,"vkpCircularBuffer [%s]: Buffer size is 0!", name.c_str())
  idx = pos_;
  return 0;
}

template <class T>
int64_t vkpCircularBuffer<T>::getIndex(int64_t pos_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  return idx+pos_;
}

template <class T>
T& vkpCircularBuffer<T>::get() {
  _ERRO(status < 0, {static T ret; return ret; }, "vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(idx)%(int64_t)buffer.size();
  return buffer[absoluteIndex];
}


template <class T>
T& vkpCircularBuffer<T>::get(int64_t pos_) {
  _ERRO(status < 0, {static T ret; return ret; }, "vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(pos_)%(int64_t)buffer.size();
  return buffer[absoluteIndex];
}

template <class T>
T& vkpCircularBuffer<T>::getMove(int64_t pos_, int64_t moveSteps_) {
  _ERRO(status < 0, {static T ret; return ret; }, "vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(pos_)%(int64_t)buffer.size();
  idx += moveSteps_;
  return buffer[absoluteIndex];
}

template <class T>
T& vkpCircularBuffer<T>::getRelativeMove(int64_t pos_, int64_t moveSteps_) {
  _ERRO(status < 0, {static T ret; return ret; }, "vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(idx+pos_)%(int64_t)buffer.size();
  idx += moveSteps_;
  return buffer[absoluteIndex];
}

template <class T>
int vkpCircularBuffer<T>::set(T& value_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(idx)%(int64_t)buffer.size();
  buffer[absoluteIndex] = value_;
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::set(T&& value_) {
  _ERRI(set(value_), "set failed!")
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::setMove(T& value_, int64_t moveSteps_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(idx)%(int64_t)buffer.size();
  buffer[absoluteIndex] = value_;
  idx += moveSteps_;
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::setMove(T&& value_, int64_t moveSteps_) {
  _ERRI(setMove(value_, moveSteps_), "setMove failed!")
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::move(int64_t moveSteps_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  idx += moveSteps_;
  return 0;
}


template <class T>
int vkpCircularBuffer<T>::set(int64_t pos_, T& value_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  int64_t absoluteIndex = std::abs(pos_)%(int64_t)buffer.size();
  buffer[absoluteIndex] = value_;
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::allSetTo(T& value_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  for (size_t i=0; i < buffer.size(); i++)
    buffer[i] = value_;
  return 0;
}

template <class T>
int vkpCircularBuffer<T>::allSetTo(T&& value_) {
  _ERRI(status < 0,"vkpCircularBuffer [%s]: Not Initialized yet!", name.c_str())
  _ERRI(allSetTo(value_),"allSetTo() Failed")
  return 0;
}

template <class T>
std::string vkpCircularBuffer<T>::apiVersion() {
  return std::string(__VKPLIBS__VERSION);
}

template class vkpCircularBuffer<int>;
template class vkpCircularBuffer<float>;

}; // namespace vkp;
