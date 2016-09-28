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
    double mX, mY, mZ;
    double mRadius;
  };

  //! Beam
  /*!
  Represents a beam, additional properties can be added here.
  */
  struct Beam {
    int mNode1Idx, mNode2Idx;
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
      return std::make_shared<LTCGraph>(name,id,units);
    }
    static std::shared_ptr<LTCGraph> create(int id) {
      return std::make_shared<LTCGraph>(id);
    }

  public:
    LTCGraph(int id):
      mID{ id } {}
    LTCGraph(const std::string& name, int id, LTCUnits units = LTCUnits::MM):
      mName{ name },
      mID{ id },
      mUnits{units}
    {}
    
    void setName(const std::string& name) { mName = name; }
    void setUnits(LTCUnits units) { mUnits = units; }

    void addNode(double x, double y, double z, double radius = -1.0);
    void addBeam(int idx1, int idx2);

    const std::vector<Node>& getNodes()const { return mNodes; }
    const std::vector<Beam>& getBeams()const { return mBeams; }
    LTCUnits getUnits()const { return mUnits; }

    const std::string& getName()const { return mName; }
    int getID()const { return mID; }

    void setNodes(const std::vector<Node>& nodes) { mNodes = nodes; }
    void setBeams(const std::vector<Beam>& beams) { mBeams = beams; }
  private:
    std::string mName;
    LTCUnits mUnits;
    int mID;
    std::vector<Node> mNodes;
    std::vector<Beam> mBeams;
  };


}//namespace LTC


