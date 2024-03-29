/* Copyright (c) 2002-2012 Croteam Ltd. All rights reserved. */

#ifndef SE_INCL_BSP_H
#define SE_INCL_BSP_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Templates/StaticArray.h>

/*
 * Template class for BSP-tree
 */
template<class Type, int iDimensions>
class BSPTree {
public:
  CStaticArray< BSPNode<Type, iDimensions> > bt_abnNodes;  // all nodes are stored here together here

  /* Create bsp-subtree from array of polygons oriented inwards. */
  BSPNode<Type, iDimensions> *CreateSubTree(CDynamicArray<BSPPolygon<Type, iDimensions> > &arbpoPolygons);
  /* Move one subtree to array. */
  void MoveSubTreeToArray(BSPNode<Type, iDimensions> *pbnSubtree);
  /* Count nodes in subtree. */
  INDEX CountNodes(BSPNode<Type, iDimensions> *pbnSubtree);
  
  /* Move all nodes to array. */
  void MoveNodesToArray(void);

public:
  BSPNode<Type, iDimensions> *bt_pbnRoot;                  // root node of BSP-tree

  /* Default constructor. */
  BSPTree(void);
  /* Destructor. */
  ~BSPTree(void);
  /* Constructor with array of polygons oriented inwards. */
  BSPTree(CDynamicArray<BSPPolygon<Type, iDimensions> > &arbpoPolygons);

  /* Create bsp-tree from array of polygons oriented inwards. */
  void Create(CDynamicArray<BSPPolygon<Type, iDimensions> > &arbpoPolygons);
  /* Destroy bsp-tree. */
  void Destroy(void);
  // find minimum/maximum parameters of points on a line that are inside
  void FindLineMinMax(
    const Vector<Type, iDimensions> &v0,
    const Vector<Type, iDimensions> &v1,
    Type &tMin,
    Type &tMax) const;
  /* Test if a sphere is inside, outside, or intersecting. (Just a trivial rejection test) */
  FLOAT TestSphere(const Vector<Type, iDimensions> &vSphereCenter, Type tSphereRadius) const;
  /* Test if a box is inside, outside, or intersecting. (Just a trivial rejection test) */
  FLOAT TestBox(const OBBox<Type> &box) const;
  /* Read/write entire bsp tree to disk. */
  void Read_t(CTStream &strm); // throw char *
  void Write_t(CTStream &strm); // throw char *
};


#endif  /* include-once check. */

