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

#include "LTCGraph.h"

namespace LTC {
  void LTCGraph::addNode(double x, double y, double z, double radius /*= -1.0*/) {
    auto newNode = Node();

    //Always convert the graph to mm:

    if ( mUnits == LTCUnits::MM ) {
      newNode.mX = x;
      newNode.mY = y;
      newNode.mZ = z;
      newNode.mRadius = radius;
    }
    else if ( mUnits == LTCUnits::CM ) {
      newNode.mX = x*10.0;
      newNode.mY = y*10.0;
      newNode.mZ = z*10.0;
      newNode.mRadius = radius*10.0;
    }
    else if ( mUnits == LTCUnits::M ) {
      newNode.mX = x*100.0;
      newNode.mY = y*100.0;
      newNode.mZ = z*100.0;
      newNode.mRadius = radius*100.0;
    }
    else if ( mUnits == LTCUnits::FT ) {
      newNode.mX = x*304.8;
      newNode.mY = y*304.8;
      newNode.mZ = z*304.8;
      newNode.mRadius = radius*304.8;
    }
    else if ( mUnits == LTCUnits::IN ) {
      newNode.mX = x*25.4;
      newNode.mY = y*25.4;
      newNode.mZ = z*25.4;
      newNode.mRadius = radius*25.4;
    }
    if ( radius == -1.0 ) {
      newNode.mRadius = -1.0;
    }
    mNodes.push_back(std::move(newNode));
  }

  void LTCGraph::addNode(double x, double y, double z, double radius, double xS, double yS, double zS, double xE, double yE, double zE) {
    auto newNode = Node();

    //Always convert the graph to mm:
    if ( mUnits == LTCUnits::MM ) {
      newNode.mX = x;
      newNode.mY = y;
      newNode.mZ = z;
      newNode.mRadius = radius;

      newNode.mXS = xS;
      newNode.mYS = yS;
      newNode.mZS = zS;
      newNode.mXE = xE;
      newNode.mYE = yE;
      newNode.mZE = zE;
    }
    else if ( mUnits == LTCUnits::CM ) {
      newNode.mX = x*10.0;
      newNode.mY = y*10.0;
      newNode.mZ = z*10.0;
      newNode.mRadius = radius*10.0;

      newNode.mXS = xS*10.0;
      newNode.mYS = yS*10.0;
      newNode.mZS = zS*10.0;
      newNode.mXE = xE*10.0;
      newNode.mYE = yE*10.0;
      newNode.mZE = zE*10.0;
    }
    else if ( mUnits == LTCUnits::M ) {
      newNode.mX = x*100.0;
      newNode.mY = y*100.0;
      newNode.mZ = z*100.0;
      newNode.mRadius = radius*100.0;

      newNode.mXS = xS*100.0;
      newNode.mYS = yS*100.0;
      newNode.mZS = zS*100.0;
      newNode.mXE = xE*100.0;
      newNode.mYE = yE*100.0;
      newNode.mZE = zE*100.0;
    }
    else if ( mUnits == LTCUnits::FT ) {
      newNode.mX = x*304.8;
      newNode.mY = y*304.8;
      newNode.mZ = z*304.8;
      newNode.mRadius = radius*304.8;

      newNode.mXS = xS*304.8;
      newNode.mYS = yS*304.8;
      newNode.mZS = zS*304.8;
      newNode.mXE = xE*304.8;
      newNode.mYE = yE*304.8;
      newNode.mZE = zE*304.8;
    }
    else if ( mUnits == LTCUnits::IN ) {
      newNode.mX = x*25.4;
      newNode.mY = y*25.4;
      newNode.mZ = z*25.4;
      newNode.mRadius = radius*25.4;

      newNode.mXS = xS*25.4;
      newNode.mYS = yS*25.4;
      newNode.mZS = zS*25.4;
      newNode.mXE = xE*25.4;
      newNode.mYE = yE*25.4;
      newNode.mZE = zE*25.4;
    }
    if ( radius == -1.0 ) {
      newNode.mRadius = -1.0;
    }
    mNodes.push_back(std::move(newNode));
  }

  void LTCGraph::addBeam(int idx1, int idx2) {
    auto newBeam = Beam();
    newBeam.mNode1Idx = idx1;
    newBeam.mNode2Idx = idx2;

    mBeams.push_back(std::move(newBeam));
  }

  void LTCGraph::addFace(int n0, int n1, int n2, int n3 /*= -1*/) {
    auto newFace = Face();
    newFace.v0 = n0;
    newFace.v1 = n1;
    newFace.v2 = n2;
    newFace.v3 = n3;

    mFaces.push_back(std::move(newFace));
  }

} //namespace LTC