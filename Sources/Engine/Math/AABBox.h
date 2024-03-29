/* Copyright (c) 2002-2012 Croteam Ltd. All rights reserved. */

#ifndef SE_INCL_AABBOX_H
#define SE_INCL_AABBOX_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Math/Vector.h>
#include <Engine/Math/Functions.h>

/*
 * Template for axis-aligned bounding box of arbitrary dimensions
 */
template<class Type, int iDimensions>
class AABBox {
// implementation:
public:
  Vector<Type, iDimensions> minvect;   // vector of min coordinates
  Vector<Type, iDimensions> maxvect;   // vector of max coordinates

  /* Clear to normalized empty bounding box. */
  inline void SetToNormalizedEmpty(void);
// interface:
public:
  /* Default constructor. */
  inline AABBox(void);
  /* Constructor for one-point bounding box. */
  inline AABBox(const Vector<Type, iDimensions> &vPoint);
  /* Constructor for one-point and radius bounding box. */
  inline AABBox(const Vector<Type, iDimensions> &vPoint, const Type radius);
  /* Constructor for two diagonal points. */
  inline AABBox(const Vector<Type, iDimensions> &vPoint1, const Vector<Type, iDimensions> &vPoint2);

  /* Bounding box for union. */
  inline AABBox<Type, iDimensions> &operator|=(const AABBox<Type, iDimensions> &b);
  /* Bounding box for intersection. */
  inline AABBox<Type, iDimensions> &operator&=(const AABBox<Type, iDimensions> &b);
  /* Bounding box for intersection. */
  inline AABBox<Type, iDimensions> operator&(const AABBox<Type, iDimensions> &b) const;
  /* Function for moving bounding box. */
  inline AABBox<Type, iDimensions> &operator+=(const Vector<Type, iDimensions> &vct);
  inline AABBox<Type, iDimensions> &operator-=(const Vector<Type, iDimensions> &vct);
  /* Function for testing equality of bounding boxes. */
  inline BOOL operator==(const AABBox<Type, iDimensions> &box2) const;
  /* Function for testing difference between bounding boxes. */
  inline BOOL operator!=(const AABBox<Type, iDimensions> &box2) const;
  /* Test if the bounding box contains another bounding box. */
  inline BOOL operator>=(const AABBox<Type, iDimensions> &b) const;
  /* Test if the bounding box is contained in another bounding box. */
  inline BOOL operator<=(const AABBox<Type, iDimensions> &b) const;

  /* Get diagonal vector (size of box). */
  inline const Vector<Type, iDimensions> Size(void) const;
  /* Get center vector (middle of box). */
  inline const Vector<Type, iDimensions> Center(void) const;
  /* Get minimal vector (lower left of box). */
  inline const Vector<Type, iDimensions> &Min(void) const;
  /* Get maximal vector (upper right of box). */
  inline const Vector<Type, iDimensions> &Max(void) const;
  /* Check if empty. */
  inline BOOL IsEmpty(void) const;

  /* Check if intersects or touches another bounding box. */
  inline BOOL HasContactWith(const AABBox<Type, iDimensions> &b) const;
  inline BOOL HasContactWith(const AABBox<Type, iDimensions> &b, Type tEpsilon) const;
  /* Check if intersects or touches a sphere. */
  inline BOOL TouchesSphere(
    const Vector<Type, iDimensions> &vSphereCenter, Type fSphereRadius) const;

  // expand the bounding box by given size
  inline void Expand(Type tEpsilon);
  // expand the bounding box by given factor of its size along each axis
  inline void ExpandByFactor(Type tFactor);
  // stretch the bounding box by a given sizing factor
  inline void StretchByFactor(Type tSizing);
  // stretch the bounding box by a given sizing vector
  inline void StretchByVector(Vector<Type, iDimensions> vSizing);
};

/*
 * Clear to normalized empty bounding box.
 */
template<class Type, int iDimensions>
inline void AABBox<Type, iDimensions>::SetToNormalizedEmpty(void) {
  for ( int i=1; i<=iDimensions; i++ ) {
    minvect(i) = UpperLimit(Type(0));
    maxvect(i) = LowerLimit(Type(0));
  }
}

/*
 * Constructor for empty bounding box.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions>::AABBox() {
  SetToNormalizedEmpty();
}

/*
 * Constructor for one-point bounding box.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions>::AABBox(const Vector<Type, iDimensions> &vPoint) {
  for ( int i=1; i<=iDimensions; i++ ) {
    minvect(i) = maxvect(i) = vPoint(i);
  }
}

/*
 * Constructor for one-point and radius bounding box.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions>::AABBox(const Vector<Type, iDimensions> &vPoint, const Type radius) {
  for ( int i=1; i<=iDimensions; i++ ) {
    minvect(i) = vPoint(i)-radius;
    maxvect(i) = vPoint(i)+radius;
  }
}

/*
 * Constructor for two diagonal points.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions>::AABBox(const Vector<Type, iDimensions> &vPoint1, const Vector<Type, iDimensions> &vPoint2) {
  for ( int i=1; i<=iDimensions; i++ ) {
    minvect(i) = ::Min(vPoint1(i), vPoint2(i));
    maxvect(i) = ::Max(vPoint1(i), vPoint2(i));
  }
}

/*
 * Function for testing equality of bounding boxes.
 */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::operator==(const AABBox<Type, iDimensions> &box2) const {
  return ( (minvect==box2.minvect) && (maxvect==box2.maxvect) );
}

/*
 * Function for testing diferences between bounding boxes.
 */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::operator!=(const AABBox<Type, iDimensions> &box2) const {
  return !(*this == box2);
}

/*
 * Test if the bounding box contains another bounding box.
 */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::operator>=(const AABBox<Type, iDimensions> &b) const
{
  return b<=*this;
}

/*
 * Test if the bounding box is contained in another bounding box.
 */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::operator<=(const AABBox<Type, iDimensions> &b) const
{
  // for each dimension
  for (INDEX i=1; i<=iDimensions; i++ ) {
    // if that dimension's span is not contained
    if (minvect(i) < b.minvect(i) || maxvect(i) > b.maxvect(i)) {
      // the box is not contained
      return FALSE;
    }
  }
  // otherwise, it is contained
  return TRUE;
}

/*
 * Check if empty.
 */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::IsEmpty(void) const {
  // if any dimension is empty, it is empty
  for ( int i=1; i<=iDimensions; i++ ) {
    if (minvect(i) > maxvect(i)) {
      return TRUE;
    }
  }
  // otherwise, it is not empty
  return FALSE;
}

/*
 * Get center vector (middle of box).
 */
template<class Type, int iDimensions>
inline const Vector<Type, iDimensions> AABBox<Type, iDimensions>::Center(void) const {
  // center is in the middle between min and max
  return (maxvect + minvect)/(Type)2;
}

/*
 * Get diagonal vector (size of box).
 */
template<class Type, int iDimensions>
inline const Vector<Type, iDimensions> AABBox<Type, iDimensions>::Size(void) const {
  // size is difference between min and max vectors
  return maxvect - minvect;
}

/*
 * Get minimal vector (lower left of box).
 */
template<class Type, int iDimensions>
inline const Vector<Type, iDimensions> &AABBox<Type, iDimensions>::Min(void) const {
  return minvect;
}

/*
 * Get maximal vector (upper right of box).
 */
template<class Type, int iDimensions>
inline const Vector<Type, iDimensions> &AABBox<Type, iDimensions>::Max(void) const {
  return maxvect;
}

/* 
 * Bounding box for union.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions> &AABBox<Type, iDimensions>::operator|=(const AABBox<Type, iDimensions> &b) {
  for ( int i=1; i<=iDimensions; i++ ) {
    minvect(i) = ::Min(minvect(i), b.minvect(i));
    maxvect(i) = ::Max(maxvect(i), b.maxvect(i));
  }
  return *this;
}

/*
 * Bounding box for intersection.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions> &AABBox<Type, iDimensions>::operator&=(const AABBox<Type, iDimensions> &b) {
  for ( int i=1; i<=iDimensions; i++ ) {
    minvect(i) = ::Max(minvect(i), b.minvect(i));
    maxvect(i) = ::Min(maxvect(i), b.maxvect(i));
  }
  // if the result is empty bounding box, normalize it
  if ( IsEmpty() ) SetToNormalizedEmpty();
  return *this;
}

/*
 * Function for moving bounding box.
 */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions> &AABBox<Type, iDimensions>::operator+=(const Vector<Type, iDimensions> &vct) {
  minvect += vct;
  maxvect += vct;
  return *this;
}

template<class Type, int iDimensions>
inline AABBox<Type, iDimensions> &AABBox<Type, iDimensions>::operator-=(const Vector<Type, iDimensions> &vct) {
  minvect -= vct;
  maxvect -= vct;
  return *this;
}

/* Bounding box for intersection. */
template<class Type, int iDimensions>
inline AABBox<Type, iDimensions> AABBox<Type, iDimensions>::operator&(const AABBox<Type, iDimensions> &b) const {
  return AABBox<Type, iDimensions>(*this)&=b;
}

/* Check if intersects or touches another bounding box. */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::HasContactWith(const AABBox<Type, iDimensions> &b) const
{
  // for all dimensions
  for ( int i=1; i<=iDimensions; i++ ) {
    // if spans in that dimension don't have contact
    if (maxvect(i)<b.minvect(i) || minvect(i)>b.maxvect(i) ) {
      // whole bounding boxes don't have contact
      return FALSE;
    }
  }
  return TRUE;
}

/* Check if intersects or touches another bounding box. */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::HasContactWith(const AABBox<Type, iDimensions> &b, Type tEpsilon) const
{
  // for all dimensions
  for ( int i=1; i<=iDimensions; i++ ) {
    // if spans in that dimension don't have contact
    if( (maxvect(i)+tEpsilon<b.minvect(i))
      ||(minvect(i)-tEpsilon>b.maxvect(i)) ) {
      // whole bounding boxes don't have contact
      return FALSE;
    }
  }
  return TRUE;
}
/* Check if intersects or touches a sphere. */
template<class Type, int iDimensions>
inline BOOL AABBox<Type, iDimensions>::TouchesSphere(
  const Vector<Type, iDimensions> &vSphereCenter, Type fSphereRadius) const
{
  // for all dimensions
  for ( int i=1; i<=iDimensions; i++ ) {
    // if spans in that dimension don't have contact
    if( (vSphereCenter(i)+fSphereRadius<minvect(i))
      ||(vSphereCenter(i)-fSphereRadius>maxvect(i)) ) {
      // no contact
      return FALSE;
    }
  }
  return TRUE;
}

// expand the bounding box by given size
template<class Type, int iDimensions>
inline void AABBox<Type, iDimensions>::Expand(Type tEpsilon)
{
  // for all dimensions
  for ( int i=1; i<=iDimensions; i++ ) {
    // expand in that dimension
    maxvect(i)+=tEpsilon;
    minvect(i)-=tEpsilon;
  }
}

// expand the bounding box by given factor of its size along each axis
template<class Type, int iDimensions>
inline void AABBox<Type, iDimensions>::ExpandByFactor(Type tFactor)
{
  // for all dimensions
  for ( int i=1; i<=iDimensions; i++ ) {
    // expand in that dimension
    Type tEpsilon = (maxvect(i)-minvect(i))*tFactor;
    maxvect(i)+=tEpsilon;
    minvect(i)-=tEpsilon;
  }
}

// stretch the bounding box by a given sizing factor
template<class Type, int iDimensions>
inline void AABBox<Type, iDimensions>::StretchByFactor(Type tSizing)
{
  tSizing = Abs(tSizing);
  // for each dimension
  for ( int i=1; i<=iDimensions; i++ ) {
    // stretch in that dimension
    maxvect(i)*=tSizing;
    minvect(i)*=tSizing;
  }
}

// stretch the bounding box by a given sizing vector
template<class Type, int iDimensions>
inline void AABBox<Type, iDimensions>::StretchByVector(Vector<Type, iDimensions> vSizing)
{
  // for each dimension
  for ( int i=1; i<=iDimensions; i++ ) {
    // stretch in that dimension
    maxvect(i)*=Abs(vSizing(i));
    minvect(i)*=Abs(vSizing(i));
  }
}

// helper functions for converting between FLOAT and DOUBLE aabboxes
inline DOUBLEaabbox3D FLOATtoDOUBLE(const FLOATaabbox3D &plf) {
  return DOUBLEaabbox3D( FLOATtoDOUBLE(plf.Min()), FLOATtoDOUBLE(plf.Max()));
}
inline FLOATaabbox3D DOUBLEtoFLOAT(const DOUBLEaabbox3D &pld) {
  return FLOATaabbox3D( DOUBLEtoFLOAT(pld.Min()), DOUBLEtoFLOAT(pld.Max()));
}


#endif  /* include-once check. */

