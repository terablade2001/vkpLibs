// MIT License

// Copyright (c) 2017 - 2021 Vasileios Kon. Pothos (terablade2001)
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

/*
  Requires PRE/POST-Build execution of "UpdateBuildVersion.exe" which
  updates the value of the BuildVersion.hpp file which contains for example:
  "#define VERSION_NUMBER (8.0)"

  Example on getting the version:
  static vkpBuildVersioner BV1(1, VERSION_NUMBER);
  static vkpBuildVersioner BV2(2, VERSION_NUMBER);
  static vkpBuildVersioner BV3(3, VERSION_NUMBER);

  int main(int argc, char** argv) {
    std::cout << BV1.version << std::endl;
    std::cout << BV2.version << std::endl;
    std::cout << BV3.version << std::endl;
    return 0;
  }
*/

#ifndef __VKP_BUILD_VERSIONER__HEADERFILE
#define __VKP_BUILD_VERSIONER__HEADERFILE
#include <BuildVersion/BuildVersion.hpp>
#include "../../vkpVersion.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

namespace vkp {

class vkpBuildVersioner {
public:
  string version;
  vkpBuildVersioner(int display = 0):version(), vnumber(0)
  {
    double vl = VERSION_NUMBER;
    vkpBuildVersioner(display, vl);
  }

  vkpBuildVersioner(int display, double vl):version(), vnumber(0)
  {
    version.clear();
    vnumber = int(vl);
    char v[256] = {0};
    if (display == 3) {
      int tr4 = size_t(vnumber / (1000000000));
      int tr3 = size_t((vnumber-tr4*1000000000) / (1000000));
      int tr2 = size_t((vnumber-tr4*1000000000-tr3*1000000) / (1000));
      int tr1 = size_t(vnumber-tr4*1000000000-tr3*1000000-tr2*1000);
      snprintf(v,255,"[%i.%3.3i.%3.3i.%3.3i]",tr4,tr3,tr2,tr1);
      string tmpv(v);
      version.append(tmpv);
    } else if (display == 2) {
      int tr3 = size_t((vnumber) / (1000000));
      int tr2 = size_t((vnumber-tr3*1000000) / (1000));
      int tr1 = size_t((vnumber-tr3*1000000-tr2*1000));
      snprintf(v,255,"[%i.%3.3i.%3.3i]",tr3,tr2,tr1);
      string tmpv(v);
      version.append(tmpv);
    } else {
      int tr2 = size_t((vnumber) / (1000));
      int tr1 = size_t((vnumber-tr2*1000));
      snprintf(v,255,"[%i.%3.3i]",tr2,tr1);
      string tmpv(v);
      version.append(tmpv);
    }
  }

  std::string apiVersion() {
    return std::string(__VKPLIBS__VERSION);
  }

private:
  int vnumber;
};

}; // namespace vkp

#endif
