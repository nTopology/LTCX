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
    auto graphX = doc.FirstChildElement("graph");
    if (!graphX) {
      return LTC_ERROR::LTC_NO_LATTICE;
    }
    do {
      int id;
      err = graphX->QueryIntAttribute("id", &id); //get lattice id
      if (err != 0) {
        return static_cast<LTC_ERROR>(err);
      }

      auto name = graphX->Attribute("name"); //get lattice name
      if (name == nullptr) {
        name = "no_name";
      }
      LTCUnits gUnits;
      auto units = graphX->Attribute("units");
      if (units == nullptr || units == "mm") {
        gUnits = LTCUnits::MM;
      }
      else if (units == "m") {
        gUnits = LTCUnits::M;
      }
      else if (units == "cm") {
        gUnits = LTCUnits::CM;
      }
      else if (units == "in") {
        gUnits = LTCUnits::IN;
      }
      else if (units == "ft") {
        gUnits = LTCUnits::FT;
      }

      auto graph = LTCGraph::create(name, id, gUnits);

      auto nodes = graphX->FirstChildElement("nodegroup");
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

      auto beams = nodes->NextSiblingElement("beamgroup");
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
        graph->addBeam(n1, n2);
        currentBeam = currentBeam->NextSiblingElement("beam");
      } while (currentBeam);

      mGraphs.push_back(graph);

      graphX = graphX->NextSiblingElement("graph");
    } while (graphX);
    return LTC_ERROR::OK;
  }

  LTC::LTC_ERROR LTCModel::write(const char* path)
  {
    //Make new XML Doc
    auto doc = std::make_unique<XMLDocument>();
    //Insert XML declaration header
    auto dec = doc->NewDeclaration();
    doc->InsertFirstChild(dec);

    //Loop through graph objects & insert graph elements
    for (auto& graph : mGraphs) {
      auto graphX = doc->NewElement("graph");
      graphX->SetAttribute("id", graph->getID());
      graphX->SetAttribute("name", graph->getName().c_str());

      auto gUnits = graph->getUnits();
      if (gUnits == LTCUnits::MM) {
        graphX->SetAttribute("units", "mm");
      }
      else if (gUnits == LTCUnits::CM) {
        graphX->SetAttribute("units", "mm");
      }
      else if (gUnits == LTCUnits::M) {
        graphX->SetAttribute("units", "m");
      }
      else if (gUnits == LTCUnits::FT) {
        graphX->SetAttribute("units", "ft");
      }
      else if (gUnits == LTCUnits::IN) {
        graphX->SetAttribute("units", "in");
      }
      //add element for nodegroup
      auto nodesX = graphX->InsertEndChild(doc->NewElement("nodegroup"));

      //Loop through nodes & insert node elements
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

      //add element for bamgroup
      auto beamsX = graphX->InsertEndChild(doc->NewElement("beamgroup"));

      //Loop through beams & add beam elements
      const auto& beams = graph->getBeams();
      count = 0;
      for (auto& b : beams) {
        auto newBeam = doc->NewElement("beam");
        newBeam->SetAttribute("id", count);
        newBeam->SetAttribute("n1", b.mNode1Idx);
        newBeam->SetAttribute("n2", b.mNode2Idx);
        beamsX->InsertEndChild(newBeam);
        count++;
      }

      //add graph to document
      doc->InsertEndChild(graphX);
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