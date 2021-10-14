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

#include "../vkpVersion.hpp"
#include "vkpCSVHandler.hpp"

#include <CECS.hpp>
static CECS __ECSOBJ__("vkpCSVHandler");

using namespace std;

namespace vkp {

vkpCSVHandlerData::vkpCSVHandlerData():
type(DataType::NONE), i(0), d(0), s() {}

vkpCSVHandlerData::vkpCSVHandlerData(long long i_):
type(DataType::INT), i(i_), d(0), s() {}

vkpCSVHandlerData::vkpCSVHandlerData(double d_):
type(DataType::DOUBLE), i(0), d(d_), s() {}

vkpCSVHandlerData::vkpCSVHandlerData(std::string s_):
type(DataType::STRING), i(0), d(0), s(s_) {}

vkpCSVHandlerData::vkpCSVHandlerData(DataType dataType_):
type(dataType_), i(0), d(0), s() {}

vkpCSVHandlerData::vkpCSVHandlerData(unsigned char dataType_):
type(DataType::NONE), i(0), d(0), s() {
  type = convertIdToDataType(dataType_); _CHECKR_
}

vkpCSVHandlerData::vkpCSVHandlerData(std::string text_, DataType dataType_):
type(dataType_), i(0), d(0), s() {
  setVal(text_);
}


int vkpCSVHandlerData::setVal(std::string str) {
  _ERRI(type==DataType::NONE,"setVal(): DataType is NONE and not provided!")
  switch (type) {
    case (DataType::INT): {
      try { i = std::stoll(str); } catch (std::exception& e) { _ERRI(1,"Failed to convert [%s] to integer.\nException: [%s]",str.c_str(), e.what()) }
      break;
    }
    case (DataType::DOUBLE): {
      try { d = std::stod(str); } catch (std::exception& e) { _ERRI(1,"Failed to convert [%s] to double.\nException: [%s]",str.c_str(), e.what()) }
      break;
    }
    case (DataType::STRING): { s = str; break; }
    default: { _ERRI(1,"Unexpected code reach.") }
  }
  return 0;
}

int vkpCSVHandlerData::setType(DataType type_) {
  type = type_;
  return 0;
}

int vkpCSVHandlerData::setVal(std::string str, DataType dtype) {
  setType(dtype);
  _ERRI(0!=setVal(str),"Error during setVal().")
  return 0;
}

void* vkpCSVHandlerData::getDatum(DataType& dtype) {
  void* retPtr = nullptr;
  _ERRN(type==DataType::NONE,"getDatum(): Error. DataType = NONE (Datum has not been initialized)")
  switch (type) {
    case (DataType::INT): { retPtr = &i; break; }
    case (DataType::DOUBLE): { retPtr = &d; break; }
    case (DataType::STRING): { retPtr = &s; break; }
    default: { _ERRN(1,"Unexpected code reach.") }
  }
  return retPtr;
}


vkpCSVHandlerData::DataType vkpCSVHandlerData::convertIdToDataType(unsigned char dataType_) {
  switch (dataType_) {
    case (0): { return DataType::INT; }
    case (1): { return DataType::DOUBLE; }
    case (2): { return DataType::STRING; }
    default: {
      _ERRO(1,{return DataType::NONE;},"Unknown dataType number = [%u]. It must be only 0, 1 or 2.", dataType_)
    }
  }
  return DataType::NONE;
}

unsigned char vkpCSVHandlerData::convertDataTypeToId(DataType dataType_) {
  switch (dataType_) {
    case (DataType::INT): { return 0; }
    case (DataType::DOUBLE): { return 1; }
    case (DataType::STRING): { return 2; }
    default: {
      _ERRO(1,{return 255;},"Unknown dataType value. It must be only DataType:INT,DOUBLE or STRING")
    }
  }
  return 255;
}


























vkpCSVHandler::vkpCSVHandler():
fieldDelimiter(" "),stringDelimiter("\n") {}

vkpCSVHandler::vkpCSVHandler(const char* fieldDelimiter_, const char* stringDelimiter_):
fieldDelimiter(),stringDelimiter() {
  _ERR(0!=Initialize(fieldDelimiter_, stringDelimiter_),"vkpCSVHandler-constructor:: Failed to initialize!")
}

int vkpCSVHandler::Initialize(const char* fieldDelimiter_, const char* stringDelimiter_) {
  _ERRI(nullptr == fieldDelimiter_, "fieldDelimiter_ is nullptr")
  _ERRI(nullptr == stringDelimiter_, "stringDelimiter_ is nullptr")
  fieldDelimiter = string(fieldDelimiter_);
  stringDelimiter = string(stringDelimiter_);
  return 0;
}


int vkpCSVHandler::setHeaders(
  std::vector<std::string>& headerStrings_,
  std::vector<unsigned char>& dataTypes_
) {
  _ERRI(headerStrings_.size() != dataTypes_.size(), "headerStrings_.size() != dataTypes_.size() (%zu != %zu)", headerStrings_.size(),  dataTypes_.size())
  _ERRI(headerStrings_.size() == 0, "headerStrings_.size() == 0")

  _ERRI(0!=setHeaders(dataTypes_),"Failed to set headers data types.")
  for (size_t i=0; i < headerStrings_.size(); i++) {
    headers.push_back(headerStrings_[i]);
    data[headerStrings_[i]] = vector<vkpCSVHandlerData>();
    data[headerStrings_[i]].clear();
    _CHECKRI_
  }
  return 0;
}

int vkpCSVHandler::setHeaders(
  std::vector<unsigned char>& dataTypes_
) {
  _ERRI(dataTypes_.size() == 0, "dataTypes_.size() == 0")

  headers.clear();
  headersTypes.clear();
  data.clear();
  for (auto& type : dataTypes_) {
    headersTypes.push_back(type);
  }
  return 0;
}



int vkpCSVHandler::loadFile(const char* fileName_, bool ignoreHeaders_) {
  _ERRI(nullptr == fileName_,"fileName_ is nullptr")
  return loadFile(string(fileName_), ignoreHeaders_);
}

int vkpCSVHandler::loadFile(std::string fileName_, bool ignoreHeaders_) {
  std::string readLine;
  std::fstream infile;
  _ERRI(fileName_.empty(), "loadFile(): Empty CSV filename provided.")
  _ERRINF(1,"CSV loading file [%s]",fileName_.c_str())
  try {
    infile.open(fileName_);
    _ERRI(!infile.is_open(), "Failed to load CSV file.")
    size_t countLines = 0;
    while(!infile.eof()) {
      std::getline(infile, readLine);
      if (ignoreHeaders_ == true) {
        _ERRO(headers.size()==0, { infile.close(); return -1; },"loadFile(): ignoring headers of CSV file without having use setHeaders() is not allowed.")
        ignoreHeaders_ = false; continue;
      } // skip 1rst line
      _ERRO(0!=parseStringLine(readLine), { infile.close(); return -1; }, "Failed to parse string line: [%s]", readLine.c_str())
      countLines++;
    }
    _ERRI(countLines == 0,"At least 1 row - Headers - is required in the CSV file.")
    infile.close();
  } catch (std::exception& e) { _ERRI(1,"Exception:\n[%s]",e.what()) }

  loadedFileName = fileName_;
  _ECSCLS(1)
  return 0;
}

int vkpCSVHandler::parseStringLine(std::string str_) {
  size_t fieldDelimiterSize = fieldDelimiter.length();
  size_t fieldPos;
  size_t stringPosStart;
  size_t stringPosEnd;
  string cellString;
  size_t column = 0;

  vector<string> vHeaders;
  vector<unsigned char> vTypes;
  const bool autoGenerateStringHeaders = (headers.size() == 0) ? true : false;

  while(!str_.empty()) {
    stringPosStart = str_.find(stringDelimiter);
    if (stringPosStart!=string::npos) {
      if (stringPosStart == 0) {
        str_ = str_.substr(1);
        stringPosEnd = str_.find(stringDelimiter);
        _ERRI(stringPosEnd==string::npos,"Failed to detect ending position of string delimiter([%s])",stringDelimiter.c_str())

        cellString = str_.substr(0,stringPosEnd);
        if (autoGenerateStringHeaders) {
          vHeaders.push_back(cellString);
        } else {
          _ERRI(0!=addStringToColumn(cellString, column++),"Failed to add [%s] at row[%zu].",cellString.c_str(), column)
        }

        str_ = str_.substr(stringPosEnd+1);
        if (!str_.empty()) {
          fieldPos = str_.find(fieldDelimiter);
          if (fieldPos!=string::npos) {
            _ERRI(fieldPos!=0,"Unexpected starting position of fieldPos delimiter([%s])",fieldDelimiter.c_str())
            str_ = str_.substr(fieldDelimiterSize);
          }
        }
        continue;
      }
    }

    fieldPos = str_.find(fieldDelimiter);
    if (fieldPos!=string::npos) {
      cellString.clear();
      if (fieldPos > 0) { cellString = str_.substr(0,fieldPos); }
      str_ = str_.substr(fieldPos+fieldDelimiterSize);
    } else { cellString = str_; str_.clear(); }

    if (autoGenerateStringHeaders) {
      vHeaders.push_back(cellString);
    } else {
      _ERRI(0!=addStringToColumn(cellString, column++),"Failed to add [%s] at row[%zu].",cellString.c_str(), column)
    }
  }

  if (autoGenerateStringHeaders) {
    _ERRI((headersTypes.size() > 0) && (vHeaders.size() != headersTypes.size()), "[%zu]-Header types have been set, but found [%zu]-headers in the csv file.",headersTypes.size(), vHeaders.size())
    if (headersTypes.size() == 0) {
      vTypes.resize(vHeaders.size(), vkpCSVHandlerData::convertDataTypeToId(vkpCSVHandlerData::DataType::STRING));
      _ERRI(0!=setHeaders(vHeaders, vTypes),"Failed to autoset headers.")
    } else {
      _ERRI(0!=setHeaders(vHeaders, headersTypes),"Failed to autoset headers.")
    }
  }
  return 0;
}

int vkpCSVHandler::getHeaderNameByColumn(size_t col_, std::string& headerName_) {
  _ERRI(col_ >= headers.size(),"Requested column [=%zu] is more than number of existing headers [=%zu].", col_, headers.size())
  headerName_ = headers[col_];
  return 0;
}
int vkpCSVHandler::getColumnByHeaderName(const char* headerName_, size_t& col) {
  _ERRI(nullptr == headerName_, "headerName_ is nullptr")
  for (size_t i = 0; i < headers.size(); i++) {
    if (0 == headers[i].compare(headerName_)) {
      col = i; return 0;
    }
  }
  _ERRI(1,"Header name [%s] does not exist in headers (of size [%zu])",headerName_,headers.size())
  return -1;
}


int vkpCSVHandler::vkpCSVHandler::addIntToColumn(long long val_, const char* headerName_) {
  _ERRI(headerName_ == nullptr,"headerName_ is nullptr")
  string headerName(headerName_);
  size_t col;
  _ERRI(0!=getColumnByHeaderName(headerName.c_str(), col),"Failed to get column of header [%s]",headerName_);
  try {
    data[headerName].emplace_back(val_);
  } catch (std::exception& e) {
    _ERRI(1,"Exception during adding [%lli] to [%s]:\n[%s]",val_,headerName.c_str(), e.what())
  }
  return 0;
}
int vkpCSVHandler::vkpCSVHandler::addIntToColumn(long long val_, size_t col_) {
  string headerName;
  _ERRI(0!=getHeaderNameByColumn(col_,headerName),"Failed to get header name of column [%zu]", col_)
  _ERRI(0!=addIntToColumn(val_,headerName.c_str()),"Failed to add INT value to columnt [%s]", headerName.c_str())
  return 0;
}

int vkpCSVHandler::vkpCSVHandler::addDoubleToColumn(double val_, const char* headerName_) {
  _ERRI(headerName_ == nullptr,"headerName_ is nullptr")
  string headerName(headerName_);
  size_t col;
  _ERRI(0!=getColumnByHeaderName(headerName.c_str(), col),"Failed to get column of header [%s]",headerName_);
  try {
    data[headerName].emplace_back(val_);
  } catch (std::exception& e) {
    _ERRI(1,"Exception during adding [%lli] to [%s]:\n[%s]",val_,headerName.c_str(), e.what())
  }
  return 0;
}
int vkpCSVHandler::vkpCSVHandler::addDoubleToColumn(double val_, size_t col_) {
  string headerName;
  _ERRI(0!=getHeaderNameByColumn(col_,headerName),"Failed to get header name of column [%zu]", col_)
  _ERRI(0!=addDoubleToColumn(val_,headerName.c_str()),"Failed to add DOUBLE value to columnt [%s]", headerName.c_str())
  return 0;
}

int vkpCSVHandler::vkpCSVHandler::addStringToColumn(std::string val_, const char* headerName_) {
  _ERRI(headerName_ == nullptr,"headerName_ is nullptr")
  string headerName(headerName_);
  size_t col;
  _ERRI(0!=getColumnByHeaderName(headerName.c_str(), col),"Failed to get column of header [%s]",headerName_);
  try {
    data[headerName].emplace_back(val_, vkpCSVHandlerData::convertIdToDataType(headersTypes[col]));
  } catch (std::exception& e) {
    _ERRI(1,"Exception during adding [%lli] to [%s]:\n[%s]",val_,headerName.c_str(), e.what())
  }
  return 0;
}
int vkpCSVHandler::vkpCSVHandler::addStringToColumn(std::string val_, size_t col_) {
  string headerName;
  _ERRI(0!=getHeaderNameByColumn(col_,headerName),"Failed to get header name of column [%zu]", col_)
  _ERRI(0!=addStringToColumn(val_,headerName.c_str()),"Failed to add STRING value to columnt [%s]", headerName.c_str())
  return 0;
}

int vkpCSVHandler::vkpCSVHandler::convertStringAddToColumn(std::string val_, const char* headerName_) {
  _ERRI(headerName_ == nullptr,"headerName_ is nullptr")
  string headerName(headerName_);
  size_t col;
  _ERRI(0!=getColumnByHeaderName(headerName.c_str(), col),"Failed to get column of header [%s]",headerName_);
  try {
    data[headerName].emplace_back(val_, vkpCSVHandlerData::convertIdToDataType(headersTypes[col]));
  } catch (std::exception& e) {
    _ERRI(1,"Exception during adding [%lli] to [%s]:\n[%s]",val_,headerName.c_str(), e.what())
  }
  return 0;
}
int vkpCSVHandler::vkpCSVHandler::convertStringAddToColumn(std::string val_, size_t col_) {
  string headerName;
  _ERRI(0!=getHeaderNameByColumn(col_,headerName),"Failed to get header name of column [%zu]", col_)
  _ERRI(0!=convertStringAddToColumn(val_,headerName.c_str()),"Failed to add STRING value to columnt [%s]", headerName.c_str())
  return 0;
}


size_t vkpCSVHandler::rows() {
  _ERRO(headers.size() == 0, { return (size_t)-1; }, "No headers has been loaded.")
  _ERRO(0!=checkIfHeaderExist(headers[0].c_str()), { return (size_t)-1; },"Header check failed")
  return data[headers[0]].size();
}
size_t vkpCSVHandler::columns() {
  _ERRO(headers.size() == 0, { return (size_t)-1; }, "No headers has been loaded.")
  return headers.size();
}

std::string vkpCSVHandler::infoStr() {
  stringstream ss;
  ss << "--- CSV Info ---";
  if (!loadedFileName.empty()) ss << " : Loaded by ["<<loadedFileName<<"] ...";
  ss << "\n";
  if (headers.size() == 0) {
    ss << " -- The object has no headers. Uninitialized, or failed to load.";
    return ss.str();
  }
  ss << "Headers: ";
  for (size_t i=0; i < headers.size(); i++) {
    ss << "\"" << headers[i] <<"\"-("<<(int)headersTypes[i]<<") ";
  }
  ss << "\nData   : Rows: " << rows() << ", Columns: " << columns();

  if (0!=checkIfAllColumnsHaveTheSameRows()) {
    _ECSCLS_
    ss << "\n*** WARNING ***:: "<<errString;
  }

  return ss.str();
}


long long vkpCSVHandler::getInt(const char* headerName_, size_t row_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  _ERRI(row_ >= rows(),"Requested row[=%zu] >= data rows [=%zu]",row_, rows());
  long long retVal = 0;
  size_t col_;
  _ERRI(0!=getColumnByHeaderName(headerName_, col_),"Failed to get column by header name")
  _ERRI(vkpCSVHandlerData::convertIdToDataType(headersTypes[col_]) != vkpCSVHandlerData::DataType::INT,
    "Requested type INT but column [%s] is of type (%i)",headerName_,headersTypes[col_])
  retVal = data[string(headerName_)][row_].i;
  return retVal;
}
long long vkpCSVHandler::getInt(size_t column_, size_t row_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  long long retVal = getInt(headers[column_].c_str(),row_);
  _CERRI("Failed to get INT value...")
  return retVal;
}


double vkpCSVHandler::getDouble(const char* headerName_, size_t row_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  _ERRI(row_ >= rows(),"Requested row[=%zu] >= data rows [=%zu]",row_, rows());
  double retVal = 0;
  size_t col_;
  _ERRI(0!=getColumnByHeaderName(headerName_, col_),"Failed to get column by header name")
  _ERRI(vkpCSVHandlerData::convertIdToDataType(headersTypes[col_]) != vkpCSVHandlerData::DataType::DOUBLE,
    "Requested type DOUBLE but column [%s] is of type (%i)",headerName_,headersTypes[col_])
  retVal = data[string(headerName_)][row_].d;
  return retVal;
}
double vkpCSVHandler::getDouble(size_t column_, size_t row_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  double retVal = getInt(headers[column_].c_str(),row_);
  _CERRI("Failed to get DOUBLE value...")
  return retVal;
}


string vkpCSVHandler::getString(const char* headerName_, size_t row_) {
  _ERRO(0!=checkIfHeaderExist(headerName_), { return string(""); },"Header check failed")
  _ERRO(row_ >= rows(), { return string(""); },"Requested row[=%zu] >= data rows [=%zu]",row_, rows());
  string retVal;
  size_t col_;
  _ERRO(0!=getColumnByHeaderName(headerName_, col_), { return string(""); },"Failed to get column by header name")
  _ERRO(vkpCSVHandlerData::convertIdToDataType(headersTypes[col_]) != vkpCSVHandlerData::DataType::STRING, { return string(""); },
    "Requested type STRING but column [%s] is of type (%i)",headerName_,headersTypes[col_])
  retVal = data[string(headerName_)][row_].s;
  return retVal;
}
string vkpCSVHandler::getString(size_t column_, size_t row_) {
  _ERRO(column_ >= columns(), { return string(""); },"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  string retVal = getString(headers[column_].c_str(),row_);
  _CERRO({ return string(""); }, "Failed to get STRING value...")
  return retVal;
}


void* vkpCSVHandler::getValPtr(const char* headerName_, size_t row_, unsigned char& type_) {
  _ERRN(0!=checkIfHeaderExist(headerName_),"Header check failed")
  _ERRN(row_ >= rows(),"Requested row[=%zu] >= data rows [=%zu]",row_, rows());
  void* retVal = 0;
  size_t col_;
  _ERRN(0!=getColumnByHeaderName(headerName_, col_),"Failed to get column by header name")
  type_ = headersTypes[col_];
  vkpCSVHandlerData::DataType typeData_ = vkpCSVHandlerData::convertIdToDataType(type_);
  retVal = data[string(headerName_)][row_].getDatum(typeData_);
  _ERRN(retVal==nullptr,"getValPtr() should not return nullptr value!")
  return retVal;
}
void* vkpCSVHandler::getValPtr(size_t column_, size_t row_, unsigned char& type_) {
  _ERRN(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  void* retVal = getValPtr(headers[column_].c_str(),row_, type_);
  _CERRN("Failed to get VOID* value of dataType [%i]...",type_)
  return retVal;
}


  int vkpCSVHandler::getDataColumnPtr(const char* headerName_, std::vector<vkp::vkpCSVHandlerData>*& outPtr_) {
    outPtr_ = nullptr;
    _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
    try { outPtr_ = &data[string(headerName_)]; }
    catch (std::exception& e) { _ERRL(1,"std::map exception:\n[%s]",e.what()) }
    _ERRI(outPtr_ == nullptr,"output pointer is null. Failed to get Data Column [%s]",headerName_)
    return 0;
  }
  int vkpCSVHandler::getDataColumnPtr(size_t column_, std::vector<vkp::vkpCSVHandlerData>*& outPtr_) {
    _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
    _ERRI(0!=getDataColumnPtr(headers[column_].c_str(), outPtr_), "Failed to get INT vector data.")
    return 0;
  }

int vkpCSVHandler::getIntColumn(const char* headerName_, std::vector<long long>& out_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  auto& vData = data[string(headerName_)];
  out_.clear();
  for (auto& v : vData) out_.push_back(v.i);
  return 0;
}
int vkpCSVHandler::getIntColumn(size_t column_, std::vector<long long>& out_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=getIntColumn(headers[column_].c_str(), out_), "Failed to get INT vector data.")
  return 0;
}

int vkpCSVHandler::getDoubleColumn(const char* headerName_, std::vector<double>& out_){
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  auto& vData = data[string(headerName_)];
  out_.clear();
  for (auto& v : vData) out_.push_back(v.d);
  return 0;
}
int vkpCSVHandler::getDoubleColumn(size_t column_, std::vector<double>& out_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=getDoubleColumn(headers[column_].c_str(), out_), "Failed to get DOUBLE vector data.")
  return 0;
}

int vkpCSVHandler::getStringColumn(const char* headerName_, std::vector<std::string>& out_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  auto& vData = data[string(headerName_)];
  out_.clear();
  for (auto& v : vData) out_.push_back(v.s);
  return 0;
}
int vkpCSVHandler::getStringColumn(size_t column_, std::vector<std::string>& out_){
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=getStringColumn(headers[column_].c_str(), out_), "Failed to get DOUBLE vector data.")
  return 0;
}

int vkpCSVHandler::getPtrColumn(const char* headerName_, std::vector<void*>& out_, unsigned char& type_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  size_t col_;
  _ERRI(0!=getColumnByHeaderName(headerName_, col_),"Failed to get column by header name")
  auto& vData = data[string(headerName_)];
  type_ = headersTypes[col_];
  out_.clear();
  switch (type_) {
    case (0):  for (auto& v : vData) { out_.push_back(&v.i); break; }
    case (1):  for (auto& v : vData) { out_.push_back(&v.d); break; }
    case (2):  for (auto& v : vData) { out_.push_back(&v.s); break; }
    default: { _ERRI(1, "Unexpeced code reach.") }
  }
  return 0;
}
int vkpCSVHandler::getPtrColumn(size_t column_, std::vector<void*>& out_, unsigned char& type_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=getPtrColumn(headers[column_].c_str(), out_, type_), "Failed to get VOID* vector data.")
  return 0;
}

int vkpCSVHandler::getCSVHandlerData(const char* headerName_, std::vector<vkpCSVHandlerData>& out_, unsigned char& type_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  size_t col_;
  _ERRI(0!=getColumnByHeaderName(headerName_, col_),"Failed to get column by header name")
  type_ = headersTypes[col_];
  out_ = data[string(headerName_)];
  return 0;
}
int vkpCSVHandler::getCSVHandlerData(size_t column_, std::vector<vkpCSVHandlerData>& out_, unsigned char& type_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=getCSVHandlerData(headers[column_].c_str(), out_, type_),"Failed to get CSVHanderData for column [%zu]",column_)
  return 0;
}






int vkpCSVHandler::addNewHeader(const char* headerName_, unsigned char type_) {
  _CHECKRI_
  _ERRI(0==checkIfHeaderExist(headerName_),"addNewHeader(): Header [%s] already exist in data!", headerName_)
  _ECSCLS_
  for (auto& h : headers) { _ERRI(0 == h.compare(headerName_),"addNewHeader(): Header [%s] already exist in data!", headerName_) }
  headers.push_back(string(headerName_));
  headersTypes.push_back(type_);
  data[string(headerName_)] = vector<vkpCSVHandlerData>();
  return 0;
}

int vkpCSVHandler::setIntColumn(const char* headerName_, std::vector<long long>& inp_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  auto& vData = data[string(headerName_)];
  vData.clear();
  for (auto& i : inp_) vData.push_back(i);
  return 0;
}
int vkpCSVHandler::setIntColumn(size_t column_, std::vector<long long>& inp_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=setIntColumn(headers[column_].c_str(), inp_), "Failed to set INT vector data.")
  return 0;
}

int vkpCSVHandler::setDoubleColumn(const char* headerName_, std::vector<double>& inp_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  auto& vData = data[string(headerName_)];
  vData.clear();
  for (auto& i : inp_) vData.push_back(i);
  return 0;
}
int vkpCSVHandler::setDoubleColumn(size_t column_, std::vector<double>& inp_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=setDoubleColumn(headers[column_].c_str(), inp_), "Failed to set DOUBLE vector data.")
  return 0;
}

int vkpCSVHandler::setStringColumn(const char* headerName_, std::vector<std::string>& inp_) {
  _ERRI(0!=checkIfHeaderExist(headerName_),"Header check failed")
  auto& vData = data[string(headerName_)];
  vData.clear();
  for (auto& i : inp_) vData.push_back(i);
  return 0;
}
int vkpCSVHandler::setStringColumn(size_t column_, std::vector<std::string>& inp_) {
  _ERRI(column_ >= columns(),"Requested column[=%zu] >= data columns [=%zu]",column_, columns());
  _ERRI(0!=setStringColumn(headers[column_].c_str(), inp_), "Failed to set STRING vector data.")
  return 0;
}




int vkpCSVHandler::checkIfHeaderExist(const char* headerName_) {
  _ERRI(nullptr == headerName_, "headerName_ is nullptr")
  _ERRI(data.find(string(headerName_)) == data.end(), "Header [%s] does not exist!",headerName_)
  return 0;
}

int vkpCSVHandler::checkIfAllColumnsHaveTheSameRows() {
  _ERRI(headers.size()==0,"No headers has been set!")
  std::vector<size_t> sizes;
  for (auto& h : headers) {
    _ERRI(0!=checkIfHeaderExist(h.c_str()),"Header check failed")
    sizes.push_back(data[h].size());
  }
  size_t commonSize = sizes[0];
  for (auto& s : sizes) {
    if (commonSize != s) {
      stringstream ss;
      ss << "Not all columns have the same rows!\n";
      for (auto& h : headers) {
        ss << " - Header: ["<<h<<"], Columns: [" << data[h].size() << "]";
        if (data[h].size() != commonSize) { ss << " <--- "; }
        ss << std::endl;
      }
      errString = ss.str();
    }
    _ERRI(commonSize != s,"%s",errString.c_str())
  }
  return 0;
}



int vkpCSVHandler::saveFile(const char* fileName_, bool ignoreHeaders_, bool append_) {
  _ERRI(nullptr==fileName_,"fileName_ is nullptr")
  _ERRI(0!=saveFile(string(fileName_), ignoreHeaders_, append_),"failed to save.")
  return 0;
}
int vkpCSVHandler::saveFile(std::string fileName_, bool ignoreHeaders_, bool append_) {
  _CHECKRI_
  _ERRINF(1,"CSV writting file [%s]",fileName_.c_str())

  int saveCheck = checkIfAllColumnsHaveTheSameRows();
  if (0!=saveCheck) { _ECSCLS_ }
  _ERRSTR(0!=saveCheck,{ ss << "Check failed during saving.\n\n"; ss << infoStr(); })
  _ERRI(0!=saveCheck, "Not all columns have the same number of data. Aborting!")

  _ERRI(fileName_.empty(), "loadFile(): Empty CSV filename provided.")
  _ERRI(headers.size() != headersTypes.size(),"headers and headersTypes have different sizes (%zu != %zu)", headers.size(), headersTypes.size())
  _ERRI(headers.size() == 0,"headers size is 0!")

  std::ofstream outfile;
  if (!append_) {
    outfile.open(fileName_,std::ios_base::out);
  } else {
    outfile.open(fileName_,std::ios_base::app);
  }
  _ERRI(!outfile.is_open(), "Failed to open CSV file for writting.")

  const size_t cols = columns(); _CHECKRI_
  const size_t rows = this->rows(); _CHECKRI_
  try {
    if ((!ignoreHeaders_) && (!append_)){
      stringstream ss;
      size_t headerSize = headers.size();
      for (size_t c = 0; c < headerSize; c++) {
        auto p = headers[c].find(" ");
        if (p != string::npos) ss << stringDelimiter;
        ss << headers[c];
        if (p != string::npos) ss << stringDelimiter;
        if (c < headerSize-1) ss << fieldDelimiter;
      }
      ss << std::endl;
      outfile.write(ss.str().c_str(),ss.str().length());
    }
    if (append_) {
      outfile.seekp(0,std::ios_base::end);
    }

    for (size_t r = 0; r < rows; r++) {
      stringstream ss;
      for (size_t c = 0; c < cols; c++) {
        const unsigned char type = headersTypes[c];
        stringstream sval;
        switch (type) {
          case (0): { sval << data[string(headers[c])][r].i; break; }
          case (1): { sval << data[string(headers[c])][r].d; break; }
          case (2): { sval << data[string(headers[c])][r].s; break; }
          default: { _ERRO(1, {outfile.close(); return -1;},"Unexpected code reach") }
        }
        auto p = sval.str().find(" ");
        if (p != string::npos) ss << stringDelimiter;
        ss << sval.str();
        if (p != string::npos) ss << stringDelimiter;
        if (c < cols-1) ss << fieldDelimiter;
      }
      ss << std::endl;
      outfile.write(ss.str().c_str(),ss.str().length());
    }

    outfile.close();
  } catch (std::exception& e) { _ERRI(1,"Exception:\n[%s]",e.what()) }

  _ECSCLS(1)
  return 0;
}

int vkpCSVHandler::appendFile(const char* fileName_) {
  return saveFile(fileName_, true, true);
}
int vkpCSVHandler::appendFile(std::string fileName_) {
  return saveFile(fileName_, true, true);
}

int vkpCSVHandler::copyHeadersFrom(vkpCSVHandler& src_) {
  clear(); _CHECKRI_

  for (auto& header : src_.headers) {
    headers.push_back(header);
  }
  for (auto& headerType : src_.headersTypes) {
    headersTypes.push_back(headerType);
  }
  return 0;
}

int vkpCSVHandler::createFrom(vkpCSVHandler& src_, size_t rowStartIdx_, size_t rowEndIdx_) {
  _ERRI(rowStartIdx_ > rowEndIdx_, "rowStartIdx_ [%zu] > rowEndIdx_ [%zu]", rowStartIdx_, rowEndIdx_)
  _ERRI(0!=copyHeadersFrom(src_),"Failed to copy headers")

  size_t cols = src_.columns(); _CHECKRI_
  size_t rows = src_.rows(); _CHECKRI_
  if (rowEndIdx_ == (size_t)-1) { rowEndIdx_ = rows; }
  _ERRI(rowStartIdx_ >= rows,"Can't copy from row index [%zu] at destination with [%zu] rows", rowStartIdx_, rows);
  _ERRI(rowEndIdx_ > rows,"Can't copy up to row index [%zu] at destination with [%zu] rows", rowEndIdx_, rows);

  try {
    for (size_t colIdx=0; colIdx < cols; colIdx++) {
      std::vector<vkp::vkpCSVHandlerData>& dst = data[headers[colIdx]];
      std::vector<vkp::vkpCSVHandlerData>& src = src_.data[headers[colIdx]];
      dst = std::vector<vkp::vkpCSVHandlerData>(std::next(src.begin(),rowStartIdx_),std::next(src.begin(),rowEndIdx_));
    }
  } catch(std::exception& e) { _ERRI(1, "Exception:\n[%s]",e.what()) }

  return 0;
}

int vkpCSVHandler::removeRow(size_t rowIdx_) {
  const size_t totalRows = rows(); _CHECKRI_
  _ERRI(rowIdx_ >= totalRows,"Can't remove invalid row index [%zu]. Total rows [%zu].",rowIdx_, totalRows)
  size_t cols = columns(); _CHECKRI_
  try {
    for (size_t colIdx=0; colIdx < cols; colIdx++) {
      std::vector<vkp::vkpCSVHandlerData>& colData = data[headers[colIdx]];
      _ERRI(rowIdx_ >= colData.size(),"Unexpected Row Index %zu >= %zu, at column index %zu",rowIdx_,colData.size(), colIdx)
      colData.erase(colData.begin()+rowIdx_);
    }
  } catch(std::exception& e) { _ERRI(1, "Exception:\n[%s]",e.what()) }
  return 0;
}


int vkpCSVHandler::removeRows(std::vector<size_t> rowIdxs_) {
  sort(rowIdxs_.begin(), rowIdxs_.end(), greater<size_t>());
  for (auto& r : rowIdxs_) {
    _ERRI(0 != removeRow(r),"Failed to remove row %zu",r)
  }
  return 0;
}


int vkpCSVHandler::removeRows(std::vector<int> rowIdxs_) {
  sort(rowIdxs_.begin(), rowIdxs_.end(), greater<size_t>());
  for (auto& r : rowIdxs_) {
    _ERRI(r < 0,"row index %i < 0",r)
    _ERRI(0 != removeRow((size_t)r),"Failed to remove row %i",r)
  }
  return 0;
}


int vkpCSVHandler::clear() {
  headers.clear();
  headersTypes.clear();
  data.clear();
  loadedFileName = string("");
  errString = string("");
  return 0;
}

}; // namespace vkp
