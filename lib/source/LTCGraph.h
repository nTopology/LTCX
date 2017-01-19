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

#pragma once
#include <memory>
#include <vector>
#include <string>

namespace LTC {
  enum class LTCUnits {
    MM = 0,
    CM = 1,
    M = 2,
    IN = 3,
    FT = 4,
  };

  //! Node
  /*!
  Represents a node, additional properties can be added here.
  note: not sure it makes sense to include vector classes for this?
  */
  struct Node {
    Node() :
      mXS(-1),
      mYS(-1),
      mZS(-1),
      mXE(-1),
      mYE(-1),
      mZE(-1),
      mRadius(-1) {}
    double mX, mY, mZ;
    double mXS, mYS, mZS;
    double mXE, mYE, mZE;

    double mRadius;
  };

  //! Beam
  /*!
  Represents a beam, additional properties can be added here.
  */
  struct Beam {
    int mNode1Idx, mNode2Idx;
  };

  //! Face
  /*!
  Represents a face, (max 4 sides). Additional properties can be added here.
  */
  struct Face {
    int v0, v1, v2, v3;
  };


  //! LTCGraph
  /*!
  Represents a Lattice Graph.
  */
  class LTCGraph {
  public:
    static std::shared_ptr<LTCGraph> create(const std::string& name,
                                            int id,
                                            LTCUnits units = LTCUnits::MM) {
      return std::make_shared<LTCGraph>(name, id, units);
    }
    static std::shared_ptr<LTCGraph> create(int id) {
      return std::make_shared<LTCGraph>(id);
    }

  public:
    LTCGraph(int id) :
      mID{ id },
      mUnits{ LTCUnits::MM } {}
    LTCGraph(const std::string& name, int id, LTCUnits units = LTCUnits::MM) :
      mName{ name },
      mID{ id },
      mUnits{ units } {}

    void setName(const std::string& name) { mName = name; }
    void setUnits(LTCUnits units) { mUnits = units; }

    void addNode(double x, double y, double z, double radius = -1.0);
    void addNode(double x, double y, double z, double radius,
                 double xS, double yS, double zS,
                 double xE, double yE, double zE);

    void addBeam(int idx1, int idx2);
    void addFace(int n0, int n1, int n2, int n3 = -1);

    const std::vector<Node>& getNodes()const { return mNodes; }
    const std::vector<Beam>& getBeams()const { return mBeams; }
    const std::vector<Face>& getFaces()const { return mFaces; }

    const std::string& getName()const { return mName; }
    int getID()const { return mID; }
    LTCUnits getUnits()const { return mUnits; }

    void setNodes(const std::vector<Node>& nodes) { mNodes = nodes; }
    void setBeams(const std::vector<Beam>& beams) { mBeams = beams; }
    void setFaces(const std::vector<Face>& faces) { mFaces = faces; }

  private:
    std::string mName;
    LTCUnits mUnits;

    int mID;
    std::vector<Node> mNodes;
    std::vector<Beam> mBeams;
    std::vector<Face> mFaces;

  };


}//namespace LTC


