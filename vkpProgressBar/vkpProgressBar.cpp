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

#include "vkpProgressBar.hpp"
#ifdef __ECSOBJ__
	#include <CECS.hpp>
	static CECS __ECSOBJ__("vkpProgressBar");
#endif

using namespace std;

vkpProgressBar::vkpProgressBar(
  const char* Open_,
  const char* Close_,
  const char* Progress_,
  const char* Arrow_,
  const float Min_,
  const float Max_,
  const int Spaces_
): Min(Min_), Max(Max_), Spaces(Spaces_), curr_space(1) {
  #ifdef __ECSOBJ__
    _ERR(Max <= Min,"vkpProgressBar():: Max(%f) <= Min(%f).",Max_,Min_)
    _ERR(Spaces_<=1,"vkpProgressBar():: Spaces(%i) must be > 1.",Spaces_)
  #else
    if (Max <= Min) { MAKE_ERRSTR;
      ErrStr << "vkpProgressBar():: Max("<<Max_<<") <= Min("<<Min_<<")." << endl;
      THROW_ERRSTR;
    }
    if (Spaces_ <=1) { MAKE_ERRSTR;
      ErrStr << "vkpProgressBar():: Spaces("<<Spaces_<<") must be > 1." << endl;
      THROW_ERRSTR;
    }
  #endif
  if (Spaces > 512) Spaces = 512;
  Open[0]     = Open_[0];
  Close[0]    = Close_[0];
  Progress[0] = Progress_[0];
  Arrow[0]    = Arrow_[0];
}

int vkpProgressBar::Start() {
  #ifdef __ECSOBJ__
    _ERRI(Open[0]==0,"vkpProgressBar::Start()>> Open[0] = 0")
    _ERRI(Arrow[0]==0,"vkpProgressBar::Start()>> Arrow[0] = 0")
    _ERRI(Close[0]==0,"vkpProgressBar::Start()>> Close[0] = 0")
    _ERRI(Max <= Min,"vkpProgressBar():: Max(%f) <= Min(%f).",Max,Min)
    _ERRI(Spaces<=1,"vkpProgressBar():: Spaces(%i) must be > 1.",Spaces)
  #endif
  printf("%s%s",Open,Arrow);
  for (int i = 0; i < Spaces-1; i++) printf(" ");
  printf("%s",Close);
  return 0;
}
	
int vkpProgressBar::Update(float iter_value) {
  if (curr_space == Spaces) return 0;
  if (iter_value < Min) iter_value = Min;
  else if (iter_value > Max) iter_value = Max-1;

  const float Diff = Max-Min;
  const float ratio = (iter_value-Min) / Diff;
  int target_space = floor(ratio*Spaces)+1;
  // cout <<target_space<<", "<<curr_space<<endl;
  if (target_space == curr_space) return 0;
  else if (target_space > curr_space) {
    // cout <<target_space<<", "<<curr_space<<", "<<Spaces+2-curr_space<<endl;
    if (target_space > Spaces) target_space = Spaces;
    
    char bstr[514]={0};
    const int maxi = (Spaces+2-curr_space > 500)?500:Spaces+2-curr_space;
    for (int i = 0; i < maxi; i++) bstr[i]='\b';
    printf("%s",bstr);
    printf("%s",Progress);

    for (; curr_space < target_space-1; curr_space++)
      printf("%s",Progress);

    curr_space=target_space;

    if (target_space < Spaces) {
      printf("%s",Arrow);
      int cnt = 0;
      for (; target_space < Spaces; target_space++) bstr[cnt++] = ' ';
      bstr[cnt] = 0;
      printf("%s",bstr);
      printf("%s",Close);
    }
    else { printf("%s",Progress); }
  }
  fflush(stdout);
  if (curr_space > Spaces) curr_space = Spaces;
  #ifdef __ECSOBJ__
    _ERRI(target_space < curr_space,"vkpProgressBar():: Negative progress not supported!")
  #else
    if (target_space < curr_space) { MAKE_ERRSTR;
      ErrStr << "vkpProgressBar():: Negative progress not supported!" << endl;
      THROW_ERRSTR;
    }
  #endif
  return 0;
}