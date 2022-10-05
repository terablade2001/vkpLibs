// MIT License

// Copyright (c) 2017 - 2022 Vasileios Kon. Pothos (terablade2001)
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

#include "../../vkpVersion.hpp"
#include "vkpConfigReader.hpp"

#ifdef __ECSOBJ__
#include <CECS.hpp>
CECS_MODULE("vkpConfigReader")
#endif

using namespace std;

namespace vkpConfigReader {


std::string cfg_apiVersion() {
  return std::string(__VKPLIBS__VERSION);
}

int cfg_LoadFile(const char* cfgfile, cfg_type& cfg_data) {
#ifdef __ECSOBJ__
  _ERRI(cfgfile == nullptr, "cfgfile == nullptr")
#else
  if (cfgfile == nullptr) {
    std::cout << "LoadConfigFile: 1rst arg = NULL. Aborting..." << std::endl;
    return -1;
  }
#endif

  cfg_data.clear();

  std::string read_string;
  std::fstream infile;
  infile.open(cfgfile);
  #ifdef __ECSOBJ__
    _ERRI(!infile.is_open(),"Failed to load [%s] file.",cfgfile)
  #else
    if (!infile.is_open()) {
      std::cout << "cfg_LoadFile(): Failed to load [" << cfgfile << "] file." << std::endl;
      return -1;
    }
  #endif
  while(!infile.eof()) {
    size_t pos;
    std::getline(infile, read_string);
    pos = read_string.find("#",0);
    if (pos == 0) continue;
    pos = read_string.find(" ",0);
    #ifdef __ECSOBJ__
      _ERRO(pos==0,{infile.close(); return-1;}, "Error: [%s] contains first character space at line [%s]",cfgfile, read_string.c_str())
    #else
      if (pos == 0) {
        std::cout << "Error: " << std::string(cfgfile) << " contain space!" << std::endl;
        infile.close();
        return -1;
      }
    #endif

    pos = read_string.find(": ",0,2);
    int spacebar=2;
    if (pos == std::string::npos) { pos = read_string.find(":",0); spacebar--; }
    if (pos == std::string::npos) continue;
    if (pos == 0) continue;

    cfg_data.emplace_back(std::make_pair(
        read_string.substr(0, pos),
        read_string.substr(pos+spacebar)
      )
    );
  }
  #ifdef __ECSOBJ__
    _ERRI(infile.bad(), "Error while reading [%s] file.",cfgfile)
  #else
    if (infile.bad()) {
      std::cout << "cfg_LoadFile(): Error while reading [" << cfgfile << "] file." << std::endl;
      return -1;
    }
  #endif
  infile.close();
  return 0;
}


int cfg_readCommand(std::string& command_, cfg_type& cfg_data) {
  // Contains CLA's type and name (see last line)
  cfg_data.clear();

  // Note:: Code to identify CLAs is based on -> vkpCSVHandler::parseStringLine()

  // The character used for seperating the command line arguments - (CLA)s
  const std::string fieldDelimiter(" ");

  // The character used to define a CLA which includes fieldDelimiter characters.
  const std::string stringDelimiter("\"");

  size_t fieldPos;
  size_t stringPosStart;
  size_t stringPosEnd;

  while(!command_.empty()) {
    // CLA = Command Line Argument (i.e. argv)
    std::string CLA;

    // Search for a string delimiter position.
    stringPosStart = command_.find(stringDelimiter);

    // If there is a character (!="") delimiter for strings at the first
    // position of the remaining processing string, then we have a string value
    // inside two placements of the character position that must be identified.
    if ((stringPosStart==0) && (0!=stringDelimiter.compare(""))) {

      // closeStringPattern: If we start with '"' for instance, the cell string
      // value is expected to close with the '" ' pattern.
      string closeStringPattern = stringDelimiter+fieldDelimiter;

      // Remove the string delimiter at 0 position from the processing string.
      command_ = command_.substr(1);

      // The following code is used to handle cases where there is a pattern
      // of i.e. '"",' which denotes that the '",' subpattern is not a CSV
      // closure. For instance, imagine the processing string:
      // "[{""text"":""This is a text\n"",""by"":""me@me.gr""}]",data
      // There are two field delimiters ',' but only the second is an actual,
      // field delimiter, while the first is not! The first one is 'fake'.
      size_t subpos = 0;
      do {
        // Search for the closing pattern, i.e. '" '.
        stringPosEnd = command_.find(closeStringPattern, subpos);
        // If that pattern doesn't exist, or is at the first position, then
        // there is no case to find a 'fake' field delimiter. Thus break.
        if ((stringPosEnd==string::npos) || (stringPosEnd==0)) break;
        // In any other case check if the detected pattern '" ' is a pattern
        // that should be ignored like '"" ' in the specific position where the
        // '",' pattern has been found.
        auto notPosEnd = command_.find(stringDelimiter+closeStringPattern, stringPosEnd-1);
        // If no such pattern exist, or exist at different position then there
        // is no interference with the current cell string.
        if ((notPosEnd==string::npos) || (notPosEnd != stringPosEnd-1)) break;
        // In any other case, ignore it and repeat the search loop for the
        // closeStringPattern '" ' after the detected one '"" '.
        subpos = stringPosEnd+2;
      } while (1);

      // In case we haven't find a '" ' pattern, then check for a '"' pattern
      // without the field delimiter (i.e, the final cell string in the row).
      // patternSize is 2 for '" ' pattern or 1 for '"' pattern.
      int patternSize = closeStringPattern.length();
      if (stringPosEnd==string::npos) {
        stringPosEnd = command_.find(stringDelimiter);
        patternSize = stringDelimiter.length();
      }
      #ifdef __ECSOBJ__
        _ERRI(stringPosEnd==string::npos,"Failed to detect ending position of string delimiter [%s]",stringDelimiter.c_str())
      #else
        if (stringPosEnd==string::npos) {
          std::cout << "Failed to detect ending position of string delimiter ["<<stringDelimiter<<"]"<<std::endl; return -1;
        }
      #endif

      // Now capture the CLA.
      CLA = command_.substr(0,stringPosEnd);

      // Remove the used data from the processing string, and repeat.
      command_ = command_.substr(stringPosEnd+patternSize);
    }

    else

    // We don't have a string delimiter at first position of the remaining
    // processing string. So search for field delimiter position (i.e. ' ').
    {
      fieldPos = command_.find(fieldDelimiter);
      // If field delimiter exist, then capture the cell string value (or
      // leave it blank if there is two delimiters consecutive like '  ')
      // and reduce processing string to the next search position.
      if (fieldPos!=string::npos) {
        if (fieldPos > 0) { CLA = command_.substr(0,fieldPos); }
        command_ = command_.substr(fieldPos + fieldDelimiter.size());
      }
      else
      // If the field delimiter doesn't exist, keep the all the rest processing
      // string as a cell string value, and clear the processing string.
      {
        CLA = command_; command_.clear();
      }
    }

    // Now check if the CLA contains a) normal b) directive (-) or c) boolean
    // a) Normal: config.cfg (meaning: argv[i] = "config.cfg")
    // b) Directive: -timemsec 100 (meaning: timemsec = 100)
    // c) boolean: --enabled (meaning: enabled = true)
    // dbg_(63,"CLA: ["<<CLA<<"]")
    string CLAType, CLAName;
    size_t booleanPos = CLA.find(string("--"));
    if (booleanPos == string::npos) { // Case a or b
      size_t directivePos = CLA.find(string("-"));
      if (directivePos != string::npos) { // Case b
        if (directivePos == 0) {
          // i.e.: -timemsec
          CLAType=string("-"); // CLAType="-"
          CLAName=CLA.substr(1); // CLAName="timemsec"
        } else {
          CLAName=CLA;
        }
      } else { // Case a:
        // i.e.: config.cfg
        CLAName=CLA; // CLAName="config.cfg" | CLAType = {empty}
      }
    } else { // Case b
      // i.e.: --enabled
      CLAType=string("--"); // CLAType="--"
      CLAName=CLA.substr(2); // CLAName="enabled"
    }
    // Update the cfgData:
    cfg_data.emplace_back(CLAType, CLAName);
  }
  return 0;
}


int cfg_ValueConvert(std::string& string_value, std::string& value) {
  value.assign(string_value);
  return 0;
}


int cfg_CheckParams(
  cfg_type& cfg_data,
  std::vector<std::string>& checklist,
  std::string& missing_params,
  bool isCLI
) {
  // dbg_(63,"cfg_CheckParams:: isCLI = "<<isCLI)
  missing_params.clear();
  int r = 0;
  for (auto const& i : checklist) {
    auto it = std::find_if(cfg_data.begin(), cfg_data.end(),
      [i, isCLI](const std::pair<std::string, std::string>& el) -> bool {
        if (!isCLI) return el.first == i;
        else {
          // dbg_(63,"i: "<<i<<", el.first: ["<<el.first<<"], el.second: ["<<el.second<<"]")
          if (el.first.length() == 0) return false;
          else if (el.first == "-") return el.second == i;
          else if (el.first == "--") return el.second == i;
          return false;
        }
      }
    );
    if (it == cfg_data.end()) {
      r=1;
      if (missing_params.length() != 0)
        missing_params.append(", ");
      missing_params.append(i);
    }
  }
  return r;
}


int _baseDataLoader::loadConfigFile(std::string file) {
  cfg_type cfgData;
  std::string NotExistingParams;
  int r = 0;
  #ifdef __ECSOBJ__
    _ERRINF(1,"config file: [%s]", file.c_str())
  #endif

  r = cfg_LoadFile(file.c_str(), cfgData);
  #ifdef __ECSOBJ__
    _ERRI(r!=0,"Failed to load config file")
  #else 
    if (r!=0) { std::cout <<"Failed to load config file"<<std::endl; return -1; }
  #endif

  
  r = cfg_CheckParams(cfgData, getCheckParamList(), NotExistingParams);
  #ifdef __ECSOBJ__
    _ERRSTR(r!=0,{ ss << "The following parameters were not found: \n" << "[" << NotExistingParams << "]"; })
    _ERRI(r!=0,"Missing parameter in configuration file.")
  #else
    if (r!=0) {
      std::stringstream ss; ss << "The following parameters were not found: \n" << "[" << NotExistingParams << "]";
      std::cout << ss.str() << std::endl;
      return -1;
    }
  #endif

  r=loadDataSection(cfgData);
  #ifdef __ECSOBJ__
    _ERRI(r!=0,"Failed to load data")  
    _ECSCLS_
  #else
    if (r!=0) { std::cout << "Failed to load data" << std::endl; return -1; }
  #endif

  return 0;
}
int _baseDataLoader::loadConfigFile(char* file) {
  #ifdef __ECSOBJ__
    _ERRI(file == nullptr,"file == nullptr")
  #else
    if (file==nullptr) { std::cout << "file==nullptr" << std::endl; return -1; }
  #endif
  return loadConfigFile(string(file));
}
int _baseDataLoader::loadConfigFile(const char* file) {
  return loadConfigFile(const_cast<char*>(file));
}

int _baseDataLoader::readCommandLine(std::string cmdLine) {
  cfg_type cfgDataCLAs;
  string NotExistingParams;
  int r = 0;
  #ifdef __ECSOBJ__
    _ERRINF(1,"CLI input: [%s]", cmdLine.c_str())
  #endif

  r = cfg_readCommand(cmdLine, cfgDataCLAs);
  #ifdef __ECSOBJ__
    _ERRI(r!=0,"Failed to read CLI input: [%s]",cmdLine.c_str())
  #else
    if(r!=0) { std::cout << "Failed to read CLI input: ["<<cmdLine<<"]"<<std::endl; return -1; }
  #endif

  r = cfg_CheckParams(cfgDataCLAs, getCheckParamList(), NotExistingParams, true);
  #ifdef __ECSOBJ__
    _ERRSTR(r!=0,{ ss << "The following CLI parameters were not found: \n" << "[" << NotExistingParams << "]"; })
    _ERRI(r!=0,"Missing parameter in the provided CLI input.")
  #else
    if (r!=0) {
      std::stringstream ss; ss << "The following CLI parameters were not found: \n" << "[" << NotExistingParams << "]";
      std::cout << ss.str() << std::endl;
      return -1;
    }
  #endif

  // for (auto& v:cfgDataCLAs) dbg_(63," * CLAs:: ["<<v.first<<"]:("<<v.second<<")")

  _argv.clear();
  cfg_type cfgData;
  for (size_t i = 0; i < cfgDataCLAs.size(); i++) {
    const auto& CLA = cfgDataCLAs[i];
    const auto& CLAType = CLA.first;
    const auto& CLAName = CLA.second;
    if (CLAType.length()==0) {
      _argv.push_back(CLAName);
    } else if (CLAType == "--") {
      cfgData.emplace_back(CLAName,"true");
    } else if (CLAType == "-") {
      // dbg_(63,"["<<CLAName<<"], i: "<<i<<", cfgDataCLAs.size(): "<<cfgDataCLAs.size())
      #ifdef __ECSOBJ__
        _ERRI((((size_t)i)+1) >= cfgDataCLAs.size(),"CLA directive [-%s] is at the end of command line (has no following up value)!",CLAName.c_str())
      #else
        if ((((size_t)i)+1) >= cfgDataCLAs.size()) {
          std::cout << "CLA directive [-"<<CLAName<<"] is at the end of command line (has no following up value)!" << std::endl;
          return -1;
        }
      #endif
      const auto& CLA2 = cfgDataCLAs[++i];
      const auto& CLA2Type = CLA2.first;
      const auto& CLA2Name = CLA2.second;
      #ifdef __ECSOBJ__
        _ERRI(CLA2Type.length()!=0,"CLA directive [-%s] followed by other tag-name: [%s%s]!",CLAName.c_str(),CLA2Type.c_str(),CLA2Name.c_str())
      #else
        if (CLA2Type.length()!=0) {
          std::cout << "CLA directive [-"<<CLAName<<"] followed by other tag-name: ["<<CLA2Type<<CLA2Name<<"]!" << std::endl;
          return -1;
        }
      #endif
      if (CLA2Type.length()==0) {
        cfgData.emplace_back(CLAName,CLA2Name);
      }
    }
  }

  r = loadDataSection(cfgData);
  #ifdef __ECSOBJ__
    _ERRI(r!=0,"Failed to load data")
    _ECSCLS_
  #else
    if (r!=0) { std::cout << "Failed to load data" << std::endl; return -1; }
  #endif
  return 0;
}
int _baseDataLoader::readCommandLine(char* file) {
  #ifdef __ECSOBJ__
    _ERRI(file == nullptr,"file == nullptr")
  #else
    if (file==nullptr) { std::cout << "file==nullptr" << std::endl; return -1; }
  #endif
  return readCommandLine(string(file));
}
int _baseDataLoader::readCommandLine(const char* file) {
  return readCommandLine(const_cast<char*>(file));
}
int _baseDataLoader::readCommandLine(int argc, char** argv) {
  #ifdef __ECSOBJ__
    _ERRI(argc <= 0,"argc (=%i) <= 0",argc)
    _ERRI(argv == nullptr,"argv == nullptr")
  #else
    if (argc <= 0) { std::cout << "argc (="<<argc<<") <= 0" << std::endl; return -1; }
    if (argv == nullptr) { std::cout << "argv == nullptr" << std::endl; return -1; }
  #endif
  stringstream ss; ss << "";
  for (int i=0; i < argc-1; i++) ss << argv[i] << " ";
  ss << argv[argc-1];
  return readCommandLine(ss.str());
}

}; // namespace vkpConfigReader
