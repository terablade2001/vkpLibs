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
#define __VKPPROGRESSBAR__VERSION__ (0.004)

#ifndef __ECSOBJ__
	#ifndef __STDEXCEPT_VKP_HEADER_FILE__
	#define __STDEXCEPT_VKP_HEADER_FILE__
		#include <stdexcept>
		#include <cstring>
		#ifndef __FNAME__
			#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
		#endif
		#define MAKE_ERRSTR std::stringstream ErrStr;\
			ErrStr << "["<<__FNAME__<<", L-"<<__LINE__<< "]: "
		#define THROW_ERRSTR throw std::runtime_error(ErrStr.str().c_str());
	#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

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
	);

	int Start();
	
	int Update(float iter_value);
private:
	vkpProgressBar(){}
};

#endif
