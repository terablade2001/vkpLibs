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

#pragma once

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
#include <string>
#include <vector>
#include <list>
#include <map>

namespace vkp {

class vkpCSVHandlerData {
public:
  enum class DataType {NONE, INT, DOUBLE, STRING};
  DataType type;
  long long i;
  double d;
  std::string s;
  vkpCSVHandlerData();
  vkpCSVHandlerData(long long i_);
  vkpCSVHandlerData(double d_);
  vkpCSVHandlerData(std::string s_);
  vkpCSVHandlerData(DataType dataType_);
  vkpCSVHandlerData(unsigned char dataType_);
  vkpCSVHandlerData(std::string text_, DataType dataType_);
  int setVal(std::string str);
  int setType(DataType type_);
  int setVal(std::string str, DataType dtype);
  void* getDatum(DataType& dtype);

  static DataType convertIdToDataType(unsigned char dataType_);
  static unsigned char convertDataTypeToId(DataType dataType_);
};

class vkpCSVHandler {
public:
  vkpCSVHandler();
  vkpCSVHandler(const char* fieldDelimiter_, const char* stringDelimiter_);
  int Initialize(const char* fieldDelimiter_, const char* stringDelimiter_);
  int setHeaders(
    std::vector<std::string>& headerStrings_,
    std::vector<unsigned char>& dataTypes_
  );
  int setHeaders(
    std::vector<unsigned char>& dataTypes_
  );
  int loadFile(const char* fileName_, bool ignoreHeaders_ = false);
  int loadFile(std::string fileName_, bool ignoreHeaders_ = false);
  int saveFile(const char* fileName_, bool ignoreHeaders_ = false);
  int saveFile(std::string fileName_, bool ignoreHeaders_ = false);

  size_t rows();
  size_t columns();

  long long getInt(const char* headerName_, size_t row_);
  long long getInt(size_t column_, size_t row_);
  double getDouble(const char* headerName_, size_t row_);
  double getDouble(size_t column_, size_t row_);
  std::string getString(const char* headerName_, size_t row_);
  std::string getString(size_t column_, size_t row_);
  void* getValPtr(const char* headerName_, size_t row_, unsigned char& type_);
  void* getValPtr(size_t column_, size_t row_, unsigned char& type_);

  int getIntColumn(const char* headerName_, std::vector<long long>& out_);
  int getIntColumn(size_t column_, std::vector<long long>& out_);
  int getDoubleColumn(const char* headerName_, std::vector<double>& out_);
  int getDoubleColumn(size_t column_, std::vector<double>& out_);
  int getStringColumn(const char* headerName_, std::vector<std::string>& out_);
  int getStringColumn(size_t column_, std::vector<std::string>& out_);
  int getPtrColumn(const char* headerName_, std::vector<void*>& out_, unsigned char& type_);
  int getPtrColumn(size_t column_, std::vector<void*>& out_, unsigned char& type_);

  int getCSVHandlerData(const char* headerName_, std::vector<vkpCSVHandlerData>& out_, unsigned char& type_);
  int getCSVHandlerData(size_t column_, std::vector<vkpCSVHandlerData>& out_, unsigned char& type_);

  int addNewHeader(const char* headerName_, unsigned char type_);
  int setIntColumn(const char* headerName_, std::vector<long long>& inp_);
  int setIntColumn(size_t column_, std::vector<long long>& inp_);
  int setDoubleColumn(const char* headerName_, std::vector<double>& inp_);
  int setDoubleColumn(size_t column_, std::vector<double>& inp_);
  int setStringColumn(const char* headerName_, std::vector<std::string>& inp_);
  int setStringColumn(size_t column_, std::vector<std::string>& inp_);

  int addStringToColumn(std::string str_, size_t col_);
  int getHeaderNameByColumn(size_t col_, std::string& headerName_);
  int getColumnByHeaderName(const char* headerName_, size_t& col);

  std::string infoStr();

  std::string apiVersion();
  int checkIfAllColumnsHaveTheSameRows();
  std::string errString;

private:
  std::string fieldDelimiter;
  std::string stringDelimiter;
  std::vector<std::string> headers;
  std::vector<unsigned char> headersTypes;
  std::map<std::string,std::vector<vkpCSVHandlerData>> data;
  std::string loadedFileName;

  int parseStringLine(std::string str_);
  int checkIfHeaderExist(const char* headerName_);
};

}; // namespace vkp
