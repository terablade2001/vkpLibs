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

// Test with:
// cd test
// g++ main.cpp ../src/vkpConfigReader.cpp -o main
// ./main -testId 0 argv_1 -configFile DefaultConfig.cfg argv_2 --cli

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "../src/vkpConfigReader.hpp"

using namespace vkpConfigReader;

// Create your custom CLI parser using vkpConfigReader from
// vp-cpp-template → https://github.com/terablade2001/vp-cpp-template
class CMainCLIReader : public vkpConfigReader::_baseDataLoader {
  public:
  bool cli; // CLI: Argument '--cli' will set this to 'true'.
  int testId;
  std::string infoText;
  std::string configFile;
  CMainCLIReader() {
    cli=false; // cli must be 'false' by default.
    // Default text of infoText if no CLI of type: '-infoText <text>' is used.
    infoText = std::string("Default Text!");
  }
  std::vector<std::string>& getCheckParamList() {
    // CLI must contain a directive of: '-testId <value>'
    static std::vector<std::string> CheckParamList = { "testId", "configFile" };
    return CheckParamList;
  }
  int loadDataSection(cfg_type& cfgData) {
    vkpConfigReaderLOADPARAM(cli)
    vkpConfigReaderLOADPARAM(testId)
    vkpConfigReaderLOADPARAM(infoText)
    vkpConfigReaderLOADPARAM(configFile)
    return 0;
  }
};

// Create your custom config-file reader using vkpConfigReader from
// vp-cpp-template → https://github.com/terablade2001/vp-cpp-template
class CMainConfigReader : public vkpConfigReader::_baseDataLoader {
  public:
  int Mode;
  std::string FileA;
  std::string FileB;
  float Rate;
  std::vector<int> Ints;
  std::vector<float> Floats;
  std::vector<std::string> Strings;
  std::vector<std::string>& getCheckParamList() {
    static std::vector<std::string> CheckParamList = {
      "Mode","FileA","FileB","Rate","Ints","Floats","Strings"
    };
    return CheckParamList;
  }
  int loadDataSection(cfg_type& cfgData) {
    vkpConfigReaderLOADPARAM(Mode)
    vkpConfigReaderLOADPARAM(FileA)
    vkpConfigReaderLOADPARAM(FileB)
    vkpConfigReaderLOADPARAM(Rate)
    vkpConfigReaderLOADPARAM(Ints)
    vkpConfigReaderLOADPARAM(Floats)
    vkpConfigReaderLOADPARAM(Strings)
    return 0;
  }
};


int main(int argc, char **argv) {
  int r;
  std::cout << "Using vkpConfigReader version "<< cfg_apiVersion() << std::endl;

  std::cout<< "------------ CLI Reader ------------" << std::endl;
  CMainCLIReader CLIData;
  r = CLIData.readCommandLine(argc,argv);
  if (r!=0) { std::cout << "Failed to read CLI input" << std::endl; return -1; }
  // Display the parser's custom variables as acquired from the CLI input.
  std::cout << "cli: " << CLIData.cli << std::endl;
  std::cout << "testId: " << CLIData.testId << std::endl;
  std::cout << "infoText: " << CLIData.infoText << std::endl;
  std::cout << "configFile: " << CLIData.configFile << std::endl;
  // Display the rest user arguments as captured from the CLI input.
  int i=0; for (const auto& argv : CLIData._argv)
    std::cout << " - argument["<<(i++)<<"]: " << argv << std::endl;
  std::cout<< "------------------------------------" << std::endl;
  
  std::cout<< "------------ Config File Reader ------------" << std::endl;
  CMainConfigReader ConfData;
  r = ConfData.loadConfigFile(CLIData.configFile);
  if (r!=0) { std::cout << "Failed to load config file." << std::endl; return -1; }   
  std::cout << "Mode = [" << ConfData.Mode << "]" << std::endl;
  std::cout << "FileA = [" << ConfData.FileA << "]" << std::endl;
  std::cout << "FileB = [" << ConfData.FileB << "]" << std::endl;
  std::cout << "Rate = [" << ConfData.Rate << "]" << std::endl;
  std::cout << "Ints: "; for (auto& i : ConfData.Ints) { std::cout <<"["<< i <<"] "; } std::cout << std::endl;
  std::cout << "Floats: "; for (auto& i : ConfData.Floats) { std::cout <<"["<< i <<"] "; } std::cout << std::endl;
  std::cout << "Strings: "; for (auto& i : ConfData.Strings) { std::cout <<"["<< i <<"] "; } std::cout << std::endl;
  std::cout<< "--------------------------------------------" << std::endl;
  std::cout << std::endl;

  return 0;
}
