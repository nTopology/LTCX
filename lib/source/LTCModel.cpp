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

#include "LTCModel.h"
#include "tinyxml2.h"
#include "LTCGraph.h"

using namespace tinyxml2;

namespace LTC {
  LTC_ERROR LTCModel::read(const char* path)
  {
    XMLError err;
    XMLDocument doc;
    doc.LoadFile(path);
    err = doc.ErrorID();
    if (err != 0) {
      return static_cast<LTC_ERROR>(doc.ErrorID());
    }

    //Get first lattice
    auto root = doc.FirstChildElement("lattice");
    if (!root) {
      return LTC_ERROR::LTC_NO_LATTICE;
    }
    int id;
    err = root->QueryIntAttribute("id", &id); //get lattice id
    if (err != 0) {
      return static_cast<LTC_ERROR>(err);
    }
    
    auto name = root->Attribute("name"); //get lattice name
    if (name == nullptr) {
      name = "no_name";
    }
    auto graph = LTCGraph::create(name, id);

    auto nodes = root->FirstChildElement("nodes");
    if (!nodes) {
      return LTC_ERROR::LTC_NO_NODES;
    }

    // loop through nodes
    auto currentNode = nodes->FirstChildElement("node");
    if (!currentNode) {
      return LTC_ERROR::LTC_NO_NODES;
    }

    double xVal, yVal, zVal, rad;
    do {
      rad = -1.0;
      currentNode->QueryDoubleAttribute("x", &xVal);
      currentNode->QueryDoubleAttribute("y", &yVal);
      currentNode->QueryDoubleAttribute("z", &zVal);
      currentNode->QueryDoubleAttribute("rad", &rad);
      graph->addNode(xVal, yVal, zVal, rad);
      currentNode = currentNode->NextSiblingElement("node");
    } while (currentNode);
    
    auto beams = nodes->NextSiblingElement("beams");
    if (!beams) {
      return LTC_ERROR::LTC_NO_BEAMS;
    }
    // loop through beams
    auto currentBeam = beams->FirstChildElement("beam");
    if (!currentBeam) {
      return LTC_ERROR::LTC_NO_BEAMS;
    }

    int n1, n2;
    do {
      currentBeam->QueryIntAttribute("n1", &n1);
      currentBeam->QueryIntAttribute("n2", &n2);
      graph->addBeam(n1,n2);
      currentBeam = currentBeam->NextSiblingElement("beam");
    } while (currentBeam);

    mGraphs.push_back(graph);
  }
}//namespace LTC