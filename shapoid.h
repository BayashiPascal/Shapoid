// ============ SHAPOID.H ================

#ifndef SHAPOID_H
#define SHAPOID_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pberr.h"
#include "pbmath.h"
#include "gset.h"

// ================= Define ==================

#define SpheroidCreate(D) ShapoidCreate(D, ShapoidTypeSpheroid)
#define FacoidCreate(D) ShapoidCreate(D, ShapoidTypeFacoid)
#define PyramidoidCreate(D) ShapoidCreate(D, ShapoidTypePyramidoid)
#define ShapoidGetCoverage(A, B) ShapoidGetCoverageDelta(A, B, 0.1)

extern const char *ShapoidTypeString[3];

// ================= Polymorphism ==================

#define ShapoidGetBoundingBox(T) _Generic((T), \
  Shapoid*: ShapoidGetBoundingBoxThat, \
  GSet*: ShapoidGetBoundingBoxSet, \
  default: PBErrInvalidPolymorphism)(T)

#define ShapoidScale(T, C) _Generic((C), \
  VecFloat*: ShapoidScaleVector, \
  float: ShapoidScaleScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

#define ShapoidGrow(T, C) _Generic((C), \
  VecFloat*: ShapoidGrowVector, \
  float: ShapoidGrowScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

// ================= Data structure ===================

typedef enum ShapoidType {
  ShapoidTypeFacoid, ShapoidTypeSpheroid,
  ShapoidTypePyramidoid
} ShapoidType;
// Don't forget to update ShapoidTypeString in shapoid.c when adding 
// new type

typedef struct Shapoid {
  // Position of origin
  VecFloat *_pos;
  // Dimension
  int _dim;
  // Vectors defining axes
  VecFloat **_axis;
  // Type of Shapoid
  ShapoidType _type;
  // Linear sytem used to import coordinates
  SysLinEq *_sysLinEqImport;
} Shapoid;

// ================ Functions declaration ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
Shapoid* ShapoidCreate(int dim, ShapoidType type);

// Clone a Shapoid
Shapoid* ShapoidClone(Shapoid *that);

// Free memory used by a Shapoid
void ShapoidFree(Shapoid **that);

// Load the Shapoid from the stream
// If the Shapoid is already allocated, it is freed before loading
// Return true upon success else false
bool ShapoidLoad(Shapoid **that, FILE *stream);

// Save the Shapoid to the stream
// Return true upon success else false
bool ShapoidSave(Shapoid *that, FILE *stream);

// Print the Shapoid on 'stream'
void ShapoidPrintln(Shapoid *that, FILE *stream);

// Get the dimension of the Shapoid
#if BUILDMODE != 0
inline
#endif 
int ShapoidGetDim(Shapoid *that);

// Get the type of the Shapoid
#if BUILDMODE != 0
inline
#endif 
ShapoidType ShapoidGetType(Shapoid *that);

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
#if BUILDMODE != 0
inline
#endif 
const char* ShapoidGetTypeAsString(Shapoid *that);

// Return a VecFloat equal to the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidGetPos(Shapoid *that);

// Return a VecFloat equal to the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidGetAxis(Shapoid *that, int dim);

// Set the position of the Shapoid to 'pos'
#if BUILDMODE != 0
inline
#endif 
void ShapoidSetPos(Shapoid *that, VecFloat *pos);

// Set the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
inline
#endif 
void ShapoidSetAxis(Shapoid *that, int dim, VecFloat *v);

// Translate the Shapoid by 'v'
#if BUILDMODE != 0
inline
#endif 
void ShapoidTranslate(Shapoid *that, VecFloat *v);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
#if BUILDMODE != 0
inline
#endif 
void ShapoidScaleVector(Shapoid *that, VecFloat *v);

// Scale the Shapoid by 'c'
#if BUILDMODE != 0
inline
#endif 
void ShapoidScaleScalar(Shapoid *that, float c);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void ShapoidGrowVector(Shapoid *that, VecFloat *v);

// Scale the Shapoid by 'c'
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void ShapoidGrowScalar(Shapoid *that, float c);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its center
#if BUILDMODE != 0
inline
#endif 
void ShapoidRotate2D(Shapoid *that, float theta);

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidImportCoord(Shapoid *that, VecFloat *pos);

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidExportCoord(Shapoid *that, VecFloat *pos);

// Return true if 'pos' (in stand coordinate system) is inside the 
// Shapoid
// Else return false
bool ShapoidIsPosInside(Shapoid *that, VecFloat *pos);

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Shapoid* ShapoidGetBoundingBoxThat(Shapoid *that); 

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Shapoid* ShapoidGetBoundingBoxSet(GSet *set);

// Get the depth value in the Shapoid of 'pos' in standard coordinate
// system
// The depth is defined as follow: the point with depth equals 1.0 is 
// the farthest point from the surface of the Shapoid (inside it),
// points with depth equals to 0.0 are point on the surface of the
// Shapoid. Depth is continuous and derivable over the volume of the
// Shapoid
// Return 0.0 if pos is outside the Shapoid
float ShapoidGetPosDepth(Shapoid *that, VecFloat *pos);

// Get the center of the shapoid in standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidGetCenter(Shapoid *that);

// Get the percentage of 'tho' included into 'that' (in [0.0, 1.0])
// 0.0 -> 'tho' is completely outside of 'that'
// 1.0 -> 'tho' is completely inside of 'that'
// 'that' and 'tho' must me of same dimensions
// delta is the step of the algorithm (in ]0.0, 1.0])
// small -> slow but precise
// big -> fast but rough
float ShapoidGetCoverageDelta(Shapoid *that, Shapoid *tho, float delta);

// Update the system of linear equation used to import coordinates
#if BUILDMODE != 0
inline
#endif 
void ShapoidUpdateSysLinEqImport(Shapoid *that);

// Check if shapoid 'that' and 'tho' are equals
#if BUILDMODE != 0
inline
#endif 
bool ShapoidIsEqual(Shapoid *that, Shapoid *tho);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "shapoid-inline.c"
#endif


#endif


