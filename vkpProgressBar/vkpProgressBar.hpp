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

/* Example:
	try {
		vkpProgressBar vkpPB("[","]","=",">",0,1e9,20);
		for (int i = 0; i < 1e9; i++) { vkpPB.Update(i); }
	} catch(std::exception &e) {
		 std::cout<< std::endl<<"(*) Exception occured: "<< std::endl << "  --> " << e.what() << std::endl;
	}
*/

#ifndef __VKPPROGRESSBAR__HEADER_FILE__
#define __VKPPROGRESSBAR__HEADER_FILE__

#ifndef __STDEXCEPT_VKP_HEADER_FILE__
#define __STDEXCEPT_VKP_HEADER_FILE__
	#include <stdexcept>
	#include <cstring>
	#ifndef __FNAME__
		#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
	#endif
	#define MAKE_ERRSTR std::stringstream ErrStr;\
		ErrStr << "["<<__FNAME__<<", L-"<<__LINE__<< "]: "
#endif

#include <stdio.h>
#include <stdlib.h>

class vkpProgressBar {
public:
	float Min;
	float Max;
	int Spaces;
	int curr_space;
	char Open[2]={0};
	char Close[2]={0};
	char Progress[2]={0};
	char Arrow[2]={0};
	
	vkpProgressBar(
		const char* Open_,
		const char* Close_,
		const char* Progress_,
		const char* Arrow_,
		const float Min_,
		const float Max_,
		const int Spaces_
	): Min(Min_), Max(Max_), Spaces(Spaces_), curr_space(1) {
		if (Max <= Min) { MAKE_ERRSTR;
			ErrStr << "vkpProgressBar():: Max("<<Max_<<") <= Min("<<Min_<<")." << endl;
			THROW_ERRSTR;
		}
		if (Spaces_ <=1) { MAKE_ERRSTR;
			ErrStr << "vkpProgressBar():: Spaces("<<Spaces_<<") must be > 1." << endl;
			THROW_ERRSTR;
		}
		if (Spaces > 512) Spaces = 512;
		Open[0]     = Open_[0];
		Close[0]    = Close_[0];
		Progress[0] = Progress_[0];
		Arrow[0]    = Arrow_[0];
		
		printf("%s%s",Open,Arrow);
		for (int i = 0; i < Spaces-1; i++) printf(" ");
		printf("%s",Close);
	}
	
	void Update(float iter_value) {
		if (curr_space == Spaces) return;
		if (iter_value < Min) iter_value = Min;
		else if (iter_value > Max) iter_value = Max-1;

		const float Diff = Max-Min;
		const float ratio = (iter_value-Min) / Diff;
		int target_space = floor(ratio*Spaces)+1;
		// cout <<target_space<<", "<<curr_space<<endl;
		if (target_space == curr_space) return;
		else if (target_space > curr_space) {
			// cout <<target_space<<", "<<curr_space<<", "<<Spaces+2-curr_space<<endl;
			if (target_space > Spaces) target_space = Spaces;
			
			char bstr[514]={0};
			const int maxi = (Spaces+2-curr_space > 500)?500:Spaces+2-curr_space;
			for (int i = 0; i < maxi; i++) bstr[i]='\b';
			printf(bstr);
			printf(Progress);

			for (; curr_space < target_space-1; curr_space++)
				printf(Progress);

			curr_space=target_space;

			if (target_space < Spaces) {
				printf(Arrow);
				int cnt = 0;
				for (; target_space < Spaces; target_space++) bstr[cnt++] = ' ';
				bstr[cnt] = 0;
				printf(bstr);
				printf(Close);
			}
			else { printf(Progress); }
		}
		fflush(stdout);
		if (curr_space > Spaces) curr_space = Spaces;
		if (target_space < curr_space) { MAKE_ERRSTR;
			ErrStr << "vkpProgressBar():: Negative progress not supported!" << endl;
			THROW_ERRSTR;
		}
	}
	
private:
	vkpProgressBar(){}
};

#endif
