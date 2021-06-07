// MIT License

// Copyright (c) 2017 - 2021 Vasileios Kon. Pothos (terablade2001)
// https://github.com/terablade2001/vkpConfigReader

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

#ifndef __VKP_CONFIG_READER__HPP__
#define __VKP_CONFIG_READER__HPP__
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#ifdef _MSC_VER
  #include <string>
#endif

namespace vkpConfigReader {

typedef std::vector<std::pair<std::string,std::string>> cfg_type;

std::string cfg_apiVersion();

int cfg_LoadFile(const char* cfgfile, cfg_type& cfg_data);

int cfg_ValueConvert(std::string& string_value, std::string& value);

template <typename T>
int cfg_ValueConvert(std::string& string_value, T& value);
template <typename T>
int cfg_ValueConvert(std::string&& string_value, T& value);

template <typename T>
int cfg_GetParam(cfg_type& cfg_data, const char* param, T& value);
template <typename T>
int cfg_GetParam(cfg_type& cfg_data, const char* param, std::vector<T>& value);

int cfg_CheckParams(
  cfg_type& cfg_data,
  std::vector<std::string>& checklist,
  std::string& missing_params
);





// ------------------------ Implementation -------------------------------------
template <typename T>
int inline cfg_ValueConvert(std::string& string_value, T& value) {
   if (std::is_same<T, int>::value)
    value = std::stoi(string_value);
  else if (std::is_same<T, float>::value)
    value = std::stof(string_value);
  else if (std::is_same<T, double>::value)
    value = std::stod(string_value);
  else if (std::is_same<T, long int>::value)
    value = std::stol(string_value);
  else if (std::is_same<T, long double>::value)
    value = std::stold(string_value);
  else if (std::is_same<T, long long>::value)
    value = std::stoll(string_value);
  else if (std::is_same<T, unsigned long>::value)
    value = std::stoul(string_value);
  else if (std::is_same<T, unsigned long long>::value)
    value = std::stoull(string_value);
  else if (std::is_same<T, bool>::value) {
    try {
      value = (bool)std::stoi(string_value);
    } catch(std::exception &e) {
      if ((string_value.compare("true") == 0) ||
          (string_value.compare("TRUE") == 0) ||
          (string_value.compare("True") == 0)
      ) value = true;
      else value = false;
    }
  } else {
    std::cout << "cfg_ValueConvert(): Not defined type!" << std::endl;
    return -1;
  }
  return 0;
}

template <typename T>
int inline cfg_ValueConvert(std::string&& string_value, T& value) {
  return cfg_ValueConvert(string_value, value);
}

template <typename T>
int inline cfg_GetParam(cfg_type& cfg_data, const char* param, T& value) {
  if (param == nullptr) {
    std::cout << "cfg_GetParam():: Error: 2nd argument == NULL. Aborting." <<
    std::endl;
    return -1;
  }
  auto it = std::find_if(cfg_data.begin(), cfg_data.end(),
      [param](const std::pair<std::string, std::string>& el) {
        return el.first == param;
      }
    );

  if (it == cfg_data.end()) {
    std::cout << "cfg_GetParam():: Error: [" << param << "] not found!" <<
    std::endl;
    return -1;
  }

  if (0!=cfg_ValueConvert((*it).second, value)) {
    std::cout << "cfg_GetParam():: Error: Failed to convert string ["<<(*it).second<<"] to proper value." << std::endl;
    return -1;
  }
  return 0;
}


template<typename T>
int inline cfg_convertToVector(std::string& string_value, std::vector<T>& output) {
  if (string_value.empty()) {
    std::cout << "Can't convert empty string ["<<string_value<<"] to vector values."<<std::endl;
    return -1;
  }
  std::string str = string_value;
  output.clear();
  int cnt = 0;
  auto p = str.find(",");
  int breakv = 0;
  while (breakv < 2) {
    T Value;
    try {
      if (p != std::string::npos) {
        if (0!=cfg_ValueConvert(str.substr(0, p),Value)) { std::cout<<"Failed to convert value..."<<std::endl; return -1; }
      } else {
        if (0!=cfg_ValueConvert(str,Value)) { std::cout<<"Failed to convert value..."<<std::endl; return -1; }
      }
    } catch(std::exception &e) {
      std::cout << "Failed to convert: ["<<str<<"], "<<cnt<<". Exception:\n"<<e.what()<<std::endl;
    }
    output.push_back(Value);
    if (p == std::string::npos) break;
    str = str.substr(p+1);
    p = str.find(",");
    if (p == std::string::npos) breakv++;
  }
  return 0;
}


template <typename T>
int inline cfg_GetParam(cfg_type& cfg_data, const char* param, std::vector<T>& value) {
  if (param == nullptr) {
    std::cout << "cfg_GetParam():: Error: 2nd argument == NULL. Aborting." <<
    std::endl;
    return -1;
  }
  auto it = std::find_if(cfg_data.begin(), cfg_data.end(),
      [param](const std::pair<std::string, std::string>& el) {
        return el.first == param;
      }
    );

  if (it == cfg_data.end()) {
    std::cout << "cfg_GetParam():: Error: [" << param << "] not found!" <<
    std::endl;
    return -1;
  }


  if (0!=cfg_convertToVector((*it).second, value)) {
    std::cout << "cfg_GetParam():: Error: Failed to convert string ["<<(*it).second<<"] to proper vector." << std::endl;
    return -1;
  }
  return 0;
}

}; // namespace vkpConfigReader

#endif
