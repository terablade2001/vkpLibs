// MIT License

// Copyright (c) 2017 - 2019 Vasileios Kon. Pothos (terablade2001)
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

#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
  FILE* fp = fopen("BuildVersion.hpp","r");
  size_t version = 0;
  size_t subZeros = 3;
  if (argc == 2) {
    int requested_subz = atoi(argv[1]);
    if ((requested_subz >= 1) && (requested_subz <= 3)) {
      subZeros = (size_t)requested_subz;
    }
  }
  if (fp == NULL) {
    fp = fopen("BuildVersion.hpp","w");
    fprintf(fp,"#define VERSION_NUMBER (0.0)");
    fclose(fp);
  } else {
    fscanf(fp,"#define VERSION_NUMBER (%zu.0)", &version);
    fclose(fp);
    version++;
    fp=fopen("BuildVersion.hpp","w");
    fprintf(fp,"#define VERSION_NUMBER (%zu.0)", version);
    fclose(fp);
  }
  if (subZeros == 3) {
    size_t tr4 = version / (1000000000);
    size_t tr3 = (version-tr4*1000000000) / (1000000);
    size_t tr2 = (version-tr4*1000000000-tr3*1000000) / (1000);
    size_t tr1 = (version-tr4*1000000000-tr3*1000000-tr2*1000);
    printf("------> Build Version Number: [%zu.%3.3zu.%3.3zu.%3.3zu] --------####\n",
      tr4, tr3, tr2, tr1
    );
  } else if (subZeros == 2) {
    size_t tr3 = (version) / (1000000);
    size_t tr2 = (version-tr3*1000000) / (1000);
    size_t tr1 = (version-tr3*1000000-tr2*1000);
    printf("------> Build Version Number: [%zu.%3.3zu.%3.3zu] --------####\n",
      tr3, tr2, tr1
    );
  } else if (subZeros == 1) {
    size_t tr2 = (version) / (1000);
    size_t tr1 = (version-tr2*1000);
    printf("------> Build Version Number: [%zu.%3.3zu] --------####\n",
      tr2, tr1
    );
  }
  return 0;
}
