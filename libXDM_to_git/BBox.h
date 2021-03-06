/****************************************************************

  W A R N I N G:  

  Code within this file is DERIVED from the companion CD to 
Physically Based Rendering: From Theory to Implementation, by 
Matt Pharr and Greg Humphreys.  All content is Copyright (c)2004 
Matt Pharr and Greg Humphreys. 


From the Software License agreement:


Software License Agreement

This Software License Agreement is a legal agreement between you and the
author of this software. The Software includes computer software, the
associated media, any printed materials, and any "online" or electronic
documentation. By installing, copying or otherwise using the Software, you
agree to be bound by the terms of this license. If you do not agree to the
terms of this license, you may not use the Software, and you should remove
the software from your computer. The Software is protected by copyright
laws and international copyright treaties, as well as other intellectual
property laws and treaties. The Software is licensed, not sold.

1. Grant of License

You may use this Software for any non-commercial purpose, subject to the
restrictions in this license. Some purposes which can be non-commercial are
teaching, academic research, and personal experimentation.

2. Description of Other Rights and Limitations

You may not use or distribute this Software or any derivative works in any
form for commercial purposes. Examples of commercial purposes would be
using results or images generated by the software, licensing, leasing, or
selling the Software, or distributing the Software for use with commercial
products.

You may modify this Software and distribute the modified Software for
non-commercial purposes, however, you may not grant rights to the Software
or derivative works that are broader than those provided by this License.
For example, you may not distribute modifications of the Software under
terms that would permit commercial use, or under terms that purport to
require the Software or derivative works to be sublicensed to others.  You
must distribute the source code of any modifications you make along with
binary executables.  Furthermore, you must not remove any copyright or
other notices from the Software.

3. DISCLAIMER OF WARRANTY.

THE SOFTWARE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT
LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS, MERCHANTABLE,
FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE
QUALITY AND PERFORMANCE OF THE SOFTWARE IS WITH USER. SHOULD SOFTWARE PROVE
DEFECTIVE IN ANY RESPECT, YOU (NOT THE AUTHORS OR THE PUBLISHER) ASSUME THE
COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF
WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY
COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

4. LIMITATION OF LIABILITY.

UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER TORT (INCLUDING
NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL THE AUTHORS, THE PUBLISHER, ANY
OTHER CONTRIBUTOR, OR ANY DISTRIBUTOR OF COVERED CODE, OR ANY SUPPLIER OF
ANY OF SUCH PARTIES, BE LIABLE TO USER OR ANY OTHER PERSON FOR ANY
INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER
INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE,
COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR
LOSSES, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF
SUCH DAMAGES. THIS LIMITATION OF LIABILITY SHALL NOT APPLY TO LIABILITY FOR
DEATH OR PERSONAL INJURY RESULTING FROM SUCH PARTY'S NEGLIGENCE TO THE
EXTENT APPLICABLE LAW PROHIBITS SUCH LIMITATION. SOME JURISDICTIONS DO NOT
ALLOW THE EXCLUSION OR LIMITATION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES,
SO THAT EXCLUSION AND LIMITATION MAY NOT APPLY TO USER.

5. Termination of License

All of your rights under this Agreement shall terminate if you fail to
comply with any of the material terms or conditions of this Agreement and
does not cure such failure in a reasonable period of time after becoming
aware of such noncompliance. If all of your rights under this Agreement
terminate, you agree to cease use and distribution of the Program as soon
as reasonably practicable. However, your obligations under this Agreement
and any licenses granted by you relating to the Program shall continue and
survive.

6. General

The authors reserve all rights not expressly granted in this license.


Original Copyright notice:


* pbrt source code Copyright(c) 1998-2004 Matt Pharr and Greg Humphreys
*
* All Rights Reserved.
* For educational use only; commercial use expressly forbidden.
* NO WARRANTY, express or implied, for this software.
* (See file License.txt for complete license)
 

*****************************************************************/

#ifndef _BBOX2D_H_
#define _BBOX2D_H_


#include <vector>
#include <math.h>
#include <algorithm>
#include "Types.h"
#include <iostream>
#include <limits>
using std::min;
using std::max;

// #ifndef INFINITY
// #define INFINITY 1e100
// #endif

namespace PBRMath
{

///////////////////////////////////
//
// Class Point from pbrt source code by 
// Matt Pharr and Greg Humphreys
//
///////////////////////////////////
class Point
{
  
public:
  Point() {};
  Point(const Point &p):x(p.x), y(p.y) {}
  Point(Float _x, Float _y): x(_x), y(_y) { };
  Float x, y; 
  Point &operator+=(const Point &p)
  {
    x += p.x; y += p.y; 
    return *this;
  }
  Point operator+(const Point &p) const
  {
    return Point(x + p.x, y + p.y );
  }
  Point operator* (Float f) const
  {
    return Point( f*x, f*y );
  }

  Point &operator*=(Float f)
  {
    x *= f; y *= f;
    return *this;
  }
  Point operator/ (Float f) const
  {
    Float inv = Float(1)/f;
    return Point( inv * x, inv * y);
  }
  Point &operator/=(Float f)
  {
    Float inv = 1.f/f;
    x *= inv; y *= inv;
    return *this;
  }

  Point operator-(const Point &v) const
  {
    return Point( x - v.x, y - v.y );
  }
	
  Point &operator-=(const Point &v)
  {
    x -= v.x; y -= v.y;
    return *this;
  }
};


////////////////////////////
//
//  Modified from BBox from Matt Pharr and Greg Humphreys's code
// 
////////////////////////////
class  BBox2D
{
public:
  // BBox Public Methods
  BBox2D()
  {
    pMin = Point( std::numeric_limits<Float>::max(), std::numeric_limits<Float>::max() );
    pMax = Point( - std::numeric_limits<Float>::max(), - std::numeric_limits<Float>::max() );
  }
  
  BBox2D(const Point &p) : pMin(p), pMax(p) { }
  BBox2D(const Point &p1, const Point &p2)
  {
    pMin = Point( min(p1.x, p2.x), min(p1.y, p2.y) );
    pMax = Point( max(p1.x, p2.x), max(p1.y, p2.y) );
  }

  friend BBox2D Union(const BBox2D &b, const Point &p);
  friend BBox2D Union(const BBox2D &b, const BBox2D &b2);
  bool Overlaps(const BBox2D &b) const
  {
    bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
    bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
    return (x && y);
  }
  
  bool Inside(const Point &pt) const
  {
    return (pt.x >= pMin.x && pt.x <= pMax.x &&
            pt.y >= pMin.y && pt.y <= pMax.y );
  }

  bool Contains( const BBox2D & b ) const
  {
    return ( Inside( b.pMin ) && Inside( b.pMax ) );
  }
  
  void Zero()
  {
    pMin.x = pMin.y = 0;
    pMax.x = pMax.y = 0;
  }

	
  // BBox2D Public Data
  Point pMin, pMax;

  //
  //
  //  GetBoundingBoxLimits
  //
  //  Return:  4 corners of the bounding box in counterclockwise order
  //
  inline void GetBoundingBoxLimits( Point & p0, Point & p1,
                                    Point & p2, Point & p3) const
  {
    p3.x = pMin.x;
    p3.y = pMin.y;

    p2.x = pMin.x;
    p2.y = pMax.y;
    
    p1.x = pMax.x;
    p1.y = pMax.y;

    p0.x = pMax.x;
    p0.y = pMin.y;
  }

  inline Point GetCenter( ) const
  {
    return (pMax + pMin) / 2.0f;
  }
  
};

//------------------------------------------------
//  operator<<
//------------------------------------------------
std::ostream & operator<< ( std::ostream & os, const BBox2D &m ); 

inline Float DistanceSquared( Point p1, Point p2 )
{
  return ( (p1.x - p2.x) * (p1.x - p2.x) +
           (p1.y - p2.y) * (p1.y - p2.y) );
}

inline Point operator*(Float f, const Point &p)
{
  return p*f;
}

}
#endif
