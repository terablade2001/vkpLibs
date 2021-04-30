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

#include "vkpOpmlCPP.hpp"

CECS_MODULE("vkpOpmlCPP")
using namespace std;

namespace vkp {

opmlCPP::opmlCPP(std::string title, std::string filename):
s(), enabled(true)
{
  ss << "<?xml version=\"1.0\"?>\n<opml version=\"1.0\">\n<head>\n<title>\n"<<title<<"\n</title>\n</head>\n<body>\n";
  storeFile = filename;
  pushCounts = 0;
  push(title);
}

opmlCPP::~opmlCPP() {
  if (!enabled) return;
  while(pushCounts > 0) { pop(); }
  ss << "</body>\n</opml>";
  FILE* fp = fopen(storeFile.c_str(),"w");
  if (NULL!=fp) {
    fputs(ss.str().c_str(),fp);
  } else {
    std::cout << "opmlCPP: Failed to store ["<<storeFile.c_str()<<"]." << std::endl;
  }
}

int opmlCPP::push(std::string text) {
  if (!enabled) return 0;
  ss<<"<outline text=\""<<format(text)<<"\" >\n";
  pushCounts++;
  return 0;
}
int opmlCPP::push(const char* text) {
  push(string(text));
  return 0;
}
int opmlCPP::push(char* text) {
  push(string(text));
  return 0;
}
int opmlCPP::push(std::ostream& text) {
  if (!enabled) return 0;
  push(dynamic_cast<std::ostringstream*>(&text)->str());
  s.str("");
  return 0;
}


int opmlCPP::pop() {
  if (!enabled) return 0;
  ss<<"</outline>\n";
  pushCounts--;
  _ERRI(pushCounts<0,"More pop() called than push()!")
  return 0;
}

int opmlCPP::add(std::string text) {
  if (!enabled) return 0;
  ss<<"<outline text=\""<<format(text)<<"\" />\n";
  return 0;
}
int opmlCPP::add(const char* text) {
  add(string(text));
  return 0;
}
int opmlCPP::add(char* text) {
  add(string(text));
  return 0;
}
int opmlCPP::add(std::ostream& text) {
  if (!enabled) return 0;
  add(dynamic_cast<std::ostringstream*>(&text)->str());
  s.str("");
  return 0;
}


std::string opmlCPP::format(std::string inp) {
  std::string out;
  auto p = inp.find("\"");
  if (p == string::npos) {
    out = inp;
  } else {
    while (p != string::npos) {
      out += inp.substr(0,p)+string("&quot;");
      inp = inp.substr(p+1);
      p = inp.find("\"");
    }
  }
  return out;
}


void opmlCPP::enable(bool yesno) {
  ss.str("");
  pushCounts = 0;
  enabled=yesno;
}

}; // namespace vkp
