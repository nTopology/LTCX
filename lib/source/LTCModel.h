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
// furnished to do so, subject to the following conditioans :
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

#pragma once
#include "LTCGraph.h"

#include <tinyxml2.h>
#include <memory>
#include <vector>

namespace LTC {

  //! LTC_ERROR
  /*!
  Note, the first 20 match the error codes in tinyxml2.h, XMLError
  */
  enum class LTC_ERROR {
    OK = 0,
    XML_NO_ATTRIBUTE = 1,
    XML_WRONG_ATTRIBUTE_TYPE,
    XML_ERROR_FILE_NOT_FOUND,
    XML_ERROR_FILE_COULD_NOT_BE_OPENED,
    XML_ERROR_FILE_READ_ERROR,
    XML_ERROR_ELEMENT_MISMATCH,
    XML_ERROR_PARSING_ELEMENT,
    XML_ERROR_PARSING_ATTRIBUTE,
    XML_ERROR_IDENTIFYING_TAG,
    XML_ERROR_PARSING_TEXT,
    XML_ERROR_PARSING_CDATA,
    XML_ERROR_PARSING_COMMENT,
    XML_ERROR_PARSING_DECLARATION,
    XML_ERROR_PARSING_UNKNOWN,
    XML_ERROR_EMPTY_DOCUMENT,
    XML_ERROR_MISMATCHED_ELEMENT,
    XML_ERROR_PARSING,
    XML_CAN_NOT_CONVERT_TEXT,
    XML_NO_TEXT_NODE,

    XML_ERROR_COUNT,

    LTC_NO_LATTICE = 21,
    LTC_NO_NODES = 22,
    LTC_NO_BEAMS = 23,

  };


  //! LTCModel
  /*!
  LTCModel is the parent interface for reading & writing Lattice Graph Objects.

  This includes an in-memory reference of Lattice Graph Objects.

  Example Use:
  auto ltcFile = "..//sample//cube.ltcx";
  auto model = LTCModel::create();
  */
  class LTCModel {
    typedef std::shared_ptr<LTCGraph> LTCGraphP;
  public:
    enum GRAPH_TYPE {
      UNDEFINED = 0,
      ROUND = 1,
      RIB = 2
    };

    static std::shared_ptr<LTCModel> create() {
      return std::make_shared<LTCModel>();
    }
  public:
    LTCModel() {}

    LTC_ERROR readFromXml(tinyxml2::XMLDocument& doc);
    LTC_ERROR readFromText(const char* text, size_t numOfBytes);
    LTC_ERROR readFromFile(const char* path);

    LTC_ERROR writeToXml(tinyxml2::XMLDocument* doc,
                         const std::string&     comment);
    LTC_ERROR writeToFile(const char* path,
                          const std::string& comment);

    LTC_ERROR getTypes(const char* path, std::vector<GRAPH_TYPE>& types);

    //a few ways to add graphs to the model -- for writing out.
    LTC_ERROR addGeometry(const std::vector<Node>& nodes,
                          const std::vector<Beam>& beams,
                          const std::string& name);
    LTC_ERROR addGeometry(const std::vector<Node>& nodes,
                          const std::vector<Beam>& beams,
                          const std::vector<Face>& faces,
                          const std::string& name);

    LTC_ERROR addGeometry(std::shared_ptr<LTCGraph> graph);

    const std::vector<LTCGraphP>& getGraphs()const { return mGraphs; }

  private:
    std::vector<LTCGraphP> mGraphs;
  };




}//namespace LTC


