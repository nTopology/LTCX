// This file is part of libNTLatticeGraph, a lightweight C++ library
// for reading/writing XML NTLatticeGraph (.ltcx) files.
//
// Copyright (C) 2016 nTopology inc. <www.ntopology.com>
// All rights reserved.
// The MIT License(MIT)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "libNTLG.h"
#include "LTCModel.h"
#include "tinyxml2.h"

#include <iostream>
#include <string>

#include <stdio.h>

#include <chrono>


long getFileSize(const char* path)
{
  FILE * pFile;
  long size;

  fopen_s(&pFile,path, "rb");
  if (pFile == NULL) perror("Error opening file");
  else {
    fseek(pFile, 0, SEEK_END);   // non-portable
    size = ftell(pFile);
    fclose(pFile);
  }
  return size;
}

int main(int argc, const char ** argv)
{
  if (argc > 1) {
    auto fileSize = getFileSize(argv[1]);
    std::cout << "Size of "<< argv[1] << ": "<< std::to_string(fileSize) << " bytes"<< std::endl;

    auto model = LTC::LTCModel::create();
    
    auto time1 = std::chrono::high_resolution_clock::now();
    model->readFromFile(argv[1]);
    auto time2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    std::cout << "File read in: " << duration << " ms"<<std::endl;

    time1 = std::chrono::high_resolution_clock::now();
    if (argc > 2) {
      model->writeToFile(argv[2],"");
    }
    else {
      model->writeToFile("..//..//samples//Sample_Out.ltcx","");
    }
    time2 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    std::cout << "File written in: " << duration << " ms" << std::endl;
  }
  else {
    std::string file1 = "..//..//samples//SampleCube.ltcx";
    std::string file1Srf = "..//..//samples//SampleCube_SRF.ltcx";

    std::string file1Out = "..//..//samples//SampleCube_out.ltcx";
    std::string file1SrfOut = "..//..//samples//SampleCube_SRF_out.ltcx";

    // Read file1, includes Surfaces
    {
      auto fileSize = getFileSize(file1.c_str());
      std::cout << "Size of " << file1 << ": " << std::to_string(fileSize) << " bytes" << std::endl;

      auto model = LTC::LTCModel::create();

      auto time1 = std::chrono::high_resolution_clock::now();
      model->readFromFile(file1.c_str());
      auto time2 = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
      std::cout << "File read in: " << duration << " ms" << std::endl;

      time1 = std::chrono::high_resolution_clock::now();
      model->writeToFile(file1Out.c_str(), "");
      time2 = std::chrono::high_resolution_clock::now();

      duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
      std::cout << "File written in: " << duration << " ms" << std::endl;
  }

    //Read file1Srf, includes Surfaces
    {
      auto fileSize = getFileSize(file1Srf.c_str());
      std::cout << "Size of " << file1Srf << ": " << std::to_string(fileSize) << " bytes" << std::endl;

      auto model = LTC::LTCModel::create();

      auto time1 = std::chrono::high_resolution_clock::now();
      model->readFromFile(file1Srf.c_str());
      auto time2 = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
      std::cout << "File read in: " << duration << " ms" << std::endl;

      time1 = std::chrono::high_resolution_clock::now();
      model->writeToFile(file1SrfOut.c_str(), "");
      time2 = std::chrono::high_resolution_clock::now();

      duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
      std::cout << "File written in: " << duration << " ms" << std::endl;
    }
  }



  do {
    std::cout << '\n' << "Press Enter to continue...";
  } while (std::cin.get() != '\n');

  return 0;
}

