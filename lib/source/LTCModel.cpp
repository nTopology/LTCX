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
#include <tinyxml2.h>

using namespace tinyxml2;

namespace LTC {

  LTC_ERROR LTCModel::readFromXml(XMLDocument& doc) {
    XMLError err;

    //Get first lattice
    auto graphX = doc.FirstChildElement("graph");
    if ( !graphX ) {
      return LTC_ERROR::LTC_NO_LATTICE;
    }
    do {
      int id;
      err = graphX->QueryIntAttribute("id", &id); //get lattice id
      if ( err != 0 ) {
        return static_cast<LTC_ERROR>(err);
      }

      auto name = graphX->Attribute("name"); //get lattice name
      if ( name == nullptr ) {
        name = "no_name";
      }
      LTCUnits gUnits;
      auto unitsRead = graphX->Attribute("units");
      if ( unitsRead == nullptr ) {
        gUnits = LTCUnits::MM;
      }
      else {

        auto units = std::string(unitsRead);
        if ( units == "mm" ) {
          gUnits = LTCUnits::MM;
        }
        else if ( units == "m" ) {
          gUnits = LTCUnits::M;
        }
        else if ( units == "cm" ) {
          gUnits = LTCUnits::CM;
        }
        else if ( units == "in" ) {
          gUnits = LTCUnits::IN;
        }
        else if ( units == "ft" ) {
          gUnits = LTCUnits::FT;
        }
      }
      auto graph = LTCGraph::create(name, id, gUnits);

      auto nodes = graphX->FirstChildElement("nodegroup");
      if ( !nodes ) {
        return LTC_ERROR::LTC_NO_NODES;
      }


      //read in nodes:
      //
      auto currentNode = nodes->FirstChildElement("node");
      if ( !currentNode ) {
        return LTC_ERROR::LTC_NO_NODES;
      }

      double xVal, yVal, zVal, rad;
      double xS, yS, zS, xE, yE, zE;
      bool isOriented = false;
      do {
        rad = -1.0;
        xS = yS = zS = xE = yE = zE = -1;
        currentNode->QueryDoubleAttribute("x", &xVal);
        currentNode->QueryDoubleAttribute("y", &yVal);
        currentNode->QueryDoubleAttribute("z", &zVal);
        currentNode->QueryDoubleAttribute("r", &rad);

        currentNode->QueryDoubleAttribute("xs", &xS);
        currentNode->QueryDoubleAttribute("ys", &yS);
        currentNode->QueryDoubleAttribute("zs", &zS);
        currentNode->QueryDoubleAttribute("xe", &xE);
        currentNode->QueryDoubleAttribute("ye", &yE);
        currentNode->QueryDoubleAttribute("ze", &zE);
        if ( xS != xE ||
            yS != yE ||
            zS != zE ) {
          graph->addNode(xVal, yVal, zVal, rad, xS, yS, zS, xE, yE, zE);
          isOriented = true;
        }
        else {
          graph->addNode(xVal, yVal, zVal, rad);
        }
        currentNode = currentNode->NextSiblingElement("node");
      } while ( currentNode );


      //read in beams:
      //
      auto beams = nodes->NextSiblingElement("beamgroup");
      if ( beams ) { //beams might not exist!
                     // loop through beams
        auto currentBeam = beams->FirstChildElement("beam");
        if ( !currentBeam ) {
          return LTC_ERROR::LTC_NO_BEAMS;
        }

        int n1, n2;
        do {
          currentBeam->QueryIntAttribute("n1", &n1);
          currentBeam->QueryIntAttribute("n2", &n2);
          graph->addBeam(n1, n2);
          currentBeam = currentBeam->NextSiblingElement("beam");
        } while ( currentBeam );
      }
      //read in faces:
      //
      auto faces = nodes->NextSiblingElement("facegroup");
      if ( faces ) { //faces might not exist!
                     // loop through faces
        auto currentFace = faces->FirstChildElement("face");
        if ( currentFace ) {

          int n1, n2, n3, n4;
          do {
            n4 = -1; // set n4 too -1 in case it can't be loaded, remember, it
                     // is optional!
            currentFace->QueryIntAttribute("n1", &n1);
            currentFace->QueryIntAttribute("n2", &n2);
            currentFace->QueryIntAttribute("n3", &n3);
            currentFace->QueryIntAttribute("n4", &n4);
            graph->addFace(n1, n2, n3, n4);
            currentFace = currentFace->NextSiblingElement("face");
          } while ( currentFace );
        }
      }
      if ( !graph->getFaces().empty() || !graph->getBeams().empty() ) {
        mGraphs.push_back(graph);
      }
      graphX = graphX->NextSiblingElement("graph");
    } while ( graphX );
    return LTC_ERROR::OK;

  }

  LTC_ERROR LTCModel::readFromText(const char* text, size_t numOfBytes) {
    XMLError err;
    XMLDocument doc;
    doc.Parse(text, numOfBytes);
    err = doc.ErrorID();
    if ( err != 0 ) {
      return static_cast<LTC_ERROR>(doc.ErrorID());
    }

    return readFromXml(doc);
  }

  LTC::LTC_ERROR LTCModel::readFromFile(const char* path) {
    XMLError err;
    XMLDocument doc;
    doc.LoadFile(path);
    err = doc.ErrorID();
    if ( err != 0 ) {
      return static_cast<LTC_ERROR>(doc.ErrorID());
    }

    return readFromXml(doc);
  }

  LTC::LTC_ERROR LTCModel::writeToFile(const char* path,
                                       const std::string& comment) {
    //Make new XML Doc
    auto doc = std::make_unique<XMLDocument>();

    auto err1 = writeToXml(doc.get(), comment);
    if ( LTC_ERROR::OK != err1 ) {
      return err1;
    }

    auto err = doc->SaveFile(path);
    return static_cast<LTC_ERROR>(err);
  }

  LTC_ERROR LTCModel::writeToXml(tinyxml2::XMLDocument* doc,
                                 const std::string&     comment) {

    //Insert XML declaration header
    auto dec = doc->NewDeclaration();
    doc->InsertFirstChild(dec);
    auto commentX = doc->NewComment(comment.c_str());
    doc->InsertAfterChild(dec, dynamic_cast<XMLNode*>(commentX));
    //Loop through graph objects & insert graph elements
    for ( auto& graph : mGraphs ) {
      auto graphX = doc->NewElement("graph");
      graphX->SetAttribute("id", graph->getID());
      graphX->SetAttribute("name", graph->getName().c_str());
      auto gUnits = graph->getUnits();
      if ( gUnits == LTCUnits::MM ) {
        graphX->SetAttribute("units", "mm");
      }
      else if ( gUnits == LTCUnits::CM ) {
        graphX->SetAttribute("units", "mm");
      }
      else if ( gUnits == LTCUnits::M ) {
        graphX->SetAttribute("units", "m");
      }
      else if ( gUnits == LTCUnits::FT ) {
        graphX->SetAttribute("units", "ft");
      }
      else if ( gUnits == LTCUnits::IN ) {
        graphX->SetAttribute("units", "in");
      }

      //add element for nodegroup
      auto nodesX = graphX->InsertEndChild(doc->NewElement("nodegroup"));

      //Loop through nodes & insert node elements
      const auto& nodes = graph->getNodes();
      int count = 0;
      bool isOriented = false;
      for ( auto& n : nodes ) {
        auto newNode = doc->NewElement("node");
        newNode->SetAttribute("id", count);
        newNode->SetAttribute("x", n.mX);
        newNode->SetAttribute("y", n.mY);
        newNode->SetAttribute("z", n.mZ);
        if ( n.mRadius > 0.0 ) {
          newNode->SetAttribute("r", n.mRadius);
        }

        //If node has orientation data, write it:
        if ( n.mXS != n.mXE ||
            n.mYS != n.mYE ||
            n.mZS != n.mZE ) {
          newNode->SetAttribute("xs", n.mXS);
          newNode->SetAttribute("ys", n.mYS);
          newNode->SetAttribute("zs", n.mZS);

          newNode->SetAttribute("xe", n.mXE);
          newNode->SetAttribute("ye", n.mYE);
          newNode->SetAttribute("ze", n.mZE);
          isOriented = true;
        }
        nodesX->InsertEndChild(newNode);
        count++;
      }

      //Write type attribute:
      if ( isOriented ) {
        graphX->SetAttribute("type", "rib");
      }
      else {
        graphX->SetAttribute("type", "rnd");
      }

      if ( !graph->getBeams().empty() ) {
        //add element for beamgroup
        auto beamsX = graphX->InsertEndChild(doc->NewElement("beamgroup"));

        //Loop through beams & add beam elements
        const auto& beams = graph->getBeams();
        count = 0;
        for ( auto& b : beams ) {
          auto newBeam = doc->NewElement("beam");
          newBeam->SetAttribute("id", count);
          newBeam->SetAttribute("n1", b.mNode1Idx);
          newBeam->SetAttribute("n2", b.mNode2Idx);
          beamsX->InsertEndChild(newBeam);
          count++;
        }
      }

      if ( !graph->getFaces().empty() ) {
        //add element for facegroup
        auto facesX = graphX->InsertEndChild(doc->NewElement("facegroup"));

        //Loop through beams & add beam elements
        const auto& faces = graph->getFaces();
        count = 0;
        for ( auto& f : faces ) {
          auto newFace = doc->NewElement("face");
          newFace->SetAttribute("id", count);
          newFace->SetAttribute("n1", f.v0);
          newFace->SetAttribute("n2", f.v1);
          newFace->SetAttribute("n3", f.v2);
          if ( f.v3 != -1 ) { //only insert quad element, if face is a quad.
            newFace->SetAttribute("n4", f.v3);
          }
          facesX->InsertEndChild(newFace);
          count++;
        }
      }

      //add graph to document
      doc->InsertEndChild(graphX);
      count++;
    }

    return LTC_ERROR::OK;
  }

  LTC_ERROR LTCModel::getTypes(const char* path,
                               std::vector<GRAPH_TYPE>& types) {
    XMLError err;
    XMLDocument doc;
    doc.LoadFile(path);
    err = doc.ErrorID();
    if ( err != 0 ) {
      return static_cast<LTC_ERROR>(doc.ErrorID());
    }

    //Get first lattice
    auto graphX = doc.FirstChildElement("graph");
    if ( !graphX ) {
      return LTC_ERROR::LTC_NO_LATTICE;
    }
    do {
      int id;
      err = graphX->QueryIntAttribute("id", &id); //get lattice id
      if ( err != 0 ) {
        return static_cast<LTC_ERROR>(err);
      }
      auto name = graphX->Attribute("name"); //get lattice name
      if ( name == nullptr ) {
        name = "no_name";
      }
      auto type = graphX->Attribute("type"); //get lattice name
      if ( type == nullptr ) {
        types.push_back(ROUND);
      }
      else {
        std::string sType = std::string(type);
        if ( sType == "rnd" ) {
          types.push_back(ROUND);
        }
        else if ( sType == "rib" ) {
          types.push_back(RIB);
        }
        else {
          types.push_back(UNDEFINED);
        }
      }
      graphX = graphX->NextSiblingElement("graph");
    } while ( graphX );
    return LTC_ERROR::OK;
  }

  LTC::LTC_ERROR LTCModel::addGeometry(const std::vector<Node>& nodes,
                                       const std::vector<Beam>& beams,
                                       const std::string& name) {
    auto graph = LTCGraph::create((int)mGraphs.size());
    graph->setNodes(nodes);
    graph->setBeams(beams);
    graph->setName(name);
    mGraphs.push_back(graph);
    return LTC_ERROR::OK;
  }

  LTC::LTC_ERROR LTCModel::addGeometry(std::shared_ptr<LTCGraph> graph) {
    mGraphs.push_back(graph);
    return LTC_ERROR::OK;
  }

  LTC::LTC_ERROR LTCModel::addGeometry(const std::vector<Node>& nodes,
                                       const std::vector<Beam>& beams,
                                       const std::vector<Face>& faces,
                                       const std::string& name) {
    auto graph = LTCGraph::create((int)mGraphs.size());
    graph->setNodes(nodes);
    graph->setBeams(beams);
    graph->setFaces(faces);
    graph->setName(name);
    mGraphs.push_back(graph);
    return LTC_ERROR::OK;
  }

}//namespace LTC