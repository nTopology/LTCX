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

  LTC::LTC_ERROR LTCModel::write(const char* path)
  {
    auto doc = std::make_unique<XMLDocument>();
    auto dec = doc->NewDeclaration();
    doc->InsertFirstChild(dec);
    for (auto& graph : mGraphs) {
      auto latticeX = doc->NewElement("Lattice");
      latticeX->SetAttribute("id", graph->getID());
      latticeX->SetAttribute("name", graph->getName().c_str());
      auto nodesX = latticeX->InsertEndChild(doc->NewElement("nodes"));

      const auto& nodes = graph->getNodes();
      int count = 0;
      for (auto& n : nodes) {
        auto newNode = doc->NewElement("node");
        newNode->SetAttribute("id", count);
        newNode->SetAttribute("x", n.mX);
        newNode->SetAttribute("y", n.mY);
        newNode->SetAttribute("z", n.mZ);
        if (n.mRadius > 0.0) {
          newNode->SetAttribute("r", n.mRadius);
        }
        nodesX->InsertEndChild(newNode);
        count++;
      }

      auto beamsX = latticeX->InsertEndChild(doc->NewElement("beams"));

      const auto& beams = graph->getBeams();
      count = 0;
      for (auto& b : beams) {
        auto newBeam = doc->NewElement("beam");
        newBeam->SetAttribute("id", count);
        newBeam->SetAttribute("n1", b.mNode1Idx);
        newBeam->SetAttribute("n2", b.mNode2Idx);
        beamsX->InsertEndChild(newBeam);
      }
      doc->InsertEndChild(latticeX);
      count++;
    }

    auto err = doc->SaveFile(path);
    return static_cast<LTC_ERROR>(err);
  }

  LTC::LTC_ERROR LTCModel::addGeometry(const std::vector<Node>& nodes, 
                                       const std::vector<Beam>& beams, 
                                       const std::string& name)
  {
    auto graph = LTCGraph::create((int)mGraphs.size());
    graph->setNodes(nodes);
    graph->setBeams(beams);
    graph->setName(name);
    mGraphs.push_back(graph);
    return LTC_ERROR::OK;
  }

  LTC::LTC_ERROR LTCModel::addGeometry(std::shared_ptr<LTCGraph> graph)
  {
    mGraphs.push_back(graph);
    return LTC_ERROR::OK;
  }

}//namespace LTC