// MIT License

// Copyright (c) 2017 - 2018 Vasileios Kon. Pothos (terablade2001)
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

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "../src/vkpConfigReader.hpp"

using namespace vkpConfigReader;

int main(int argc, char **argv) {
  std::cout << "Using vkpConfigReader version "<< cfg_apiVersion() << std::endl;

  cfg_type cfg_data;
  int r = cfg_LoadFile("DefaultConfig.cfg", cfg_data);
  if (r != 0) return -1;

  std::vector<std::string> CheckParamList = {
    "Version", "Mode", "FileA", "FileB", "Speed", "Rate"
  };
  std::string NotExistingParams;

  r = cfg_CheckParams(cfg_data, CheckParamList, NotExistingParams);
  if (r != 0) {
    std::cout << "The following parameters were not found: \n" <<
            "[" << NotExistingParams << "]\n" << std::endl;
  }

  int Mode = -1;
  float Rate = 0.0f;
  std::string FilenameA;
  std::string FilenameB;

  std::string Ints,Floats,Strings;

  std::vector<int> vInts;
  std::vector<float> vFloats;
  std::vector<std::string> vStrings;

  cfg_GetParam(cfg_data, "Mode", Mode);
  cfg_GetParam(cfg_data, "FileA", FilenameA);
  cfg_GetParam(cfg_data, "FileB", FilenameB);
  cfg_GetParam(cfg_data, "Rate", Rate);
  cfg_GetParam(cfg_data, "Ints", Ints);
  cfg_GetParam(cfg_data, "Floats", Floats);
  cfg_GetParam(cfg_data, "Strings", Strings);

  std::cout << "Mode = [" << Mode << "]" << "|"<< std::endl;
  std::cout << "FileA = [" << FilenameA << "]" << "|"<< std::endl;
  std::cout << "FileB = [" << FilenameB << "]" << "|"<< std::endl;
  std::cout << "Rate = [" << Rate << "]" << "|"<< std::endl;
  std::cout << "Ints = [" << Ints << "]" << "|"<< std::endl;
  std::cout << "Floats = [" << Floats << "]" << "|"<< std::endl;
  std::cout << "Strings = [" << Strings << "]" << "|"<< std::endl;

  cfg_convertToVector(Ints,vInts);
  cfg_convertToVector(Floats,vFloats);
  cfg_convertToVector(Strings,vStrings);
  std::cout << "Ints: "; for (auto& i : vInts) { std::cout <<"["<< i <<"] "; } std::cout << "|"<< std::endl;
  std::cout << "Floats: "; for (auto& i : vFloats) { std::cout <<"["<< i <<"] "; } std::cout << "|"<< std::endl;
  std::cout << "Strings: "; for (auto& i : vStrings) { std::cout <<"["<< i <<"] "; } std::cout << "|"<< std::endl;

  std::cout << std::endl;

  return 0;
}