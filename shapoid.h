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

#define CloneShapoidType typedef struct {Shapoid _s;}

#define SpheroidCreate(D) \
  (Spheroid*)ShapoidCreate(D, ShapoidTypeSpheroid)
#define FacoidCreate(D) \
  (Facoid*)ShapoidCreate(D, ShapoidTypeFacoid)
#define PyramidoidCreate(D) \
  (Pyramidoid*)ShapoidCreate(D, ShapoidTypePyramidoid)

#define ShapoidGetCoverage(A, B) \
  _ShapoidGetCoverageDelta((Shapoid*)A, (Shapoid*)B, 0.1)
#define ShapoidGetCoverageDelta(A, B, D) \
  _ShapoidGetCoverageDelta((Shapoid*)A, (Shapoid*)B, D)

extern const char *ShapoidTypeString[3];

// ================= Polymorphism ==================

#define ShapoidClone(T) _Generic((T), \
  Shapoid*: _ShapoidClone, \
  Facoid*: FacoidClone, \
  Pyramidoid*: PyramidoidClone, \
  Spheroid*: SpheroidClone, \
  default: PBErrInvalidPolymorphism)(T)

#define ShapoidFree(T) _Generic((T), \
  Shapoid**: _ShapoidFree, \
  Facoid**: _ShapoidFree, \
  Pyramidoid**: _ShapoidFree, \
  Spheroid**: _ShapoidFree, \
  default: PBErrInvalidPolymorphism)((Shapoid**)(T))

#define ShapoidLoad(T, S) _Generic((T), \
  Shapoid**: _ShapoidLoad, \
  Facoid**: FacoidLoad, \
  Pyramidoid**: PyramidoidLoad, \
  Spheroid**: SpheroidLoad, \
  default: PBErrInvalidPolymorphism)(T, S)

#define ShapoidSave(T, S) _Generic((T), \
  Shapoid*: _ShapoidSave, \
  Facoid*: _ShapoidSave, \
  Pyramidoid*: _ShapoidSave, \
  Spheroid*: _ShapoidSave, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), S)

#define ShapoidPrintln(T, S) _Generic((T), \
  Shapoid*: _ShapoidPrintln, \
  Facoid*: _ShapoidPrintln, \
  Pyramidoid*: _ShapoidPrintln, \
  Spheroid*: _ShapoidPrintln, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), S)

#define ShapoidGetType(T) _Generic((T), \
  Shapoid*: _ShapoidGetType, \
  Facoid*: _ShapoidGetType, \
  Pyramidoid*: _ShapoidGetType, \
  Spheroid*: _ShapoidGetType, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T))

#define ShapoidGetTypeAsString(T) _Generic((T), \
  Shapoid*: _ShapoidGetTypeAsString, \
  Facoid*: _ShapoidGetTypeAsString, \
  Pyramidoid*: _ShapoidGetTypeAsString, \
  Spheroid*: _ShapoidGetTypeAsString, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T))

#define ShapoidGetDim(T) _Generic((T), \
  Shapoid*: _ShapoidGetDim, \
  Facoid*: _ShapoidGetDim, \
  Pyramidoid*: _ShapoidGetDim, \
  Spheroid*: _ShapoidGetDim, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T))

#define ShapoidGetPos(T) _Generic((T), \
  Shapoid*: _ShapoidGetPos, \
  Facoid*: _ShapoidGetPos, \
  Pyramidoid*: _ShapoidGetPos, \
  Spheroid*: _ShapoidGetPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T))

#define ShapoidPos(T) _Generic((T), \
  Shapoid*: _ShapoidPos, \
  Facoid*: _ShapoidPos, \
  Pyramidoid*: _ShapoidPos, \
  Spheroid*: _ShapoidPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T))

#define ShapoidSetAxis(T, I, V) _Generic((T), \
  Shapoid*: _ShapoidSetAxis, \
  Facoid*: _ShapoidSetAxis, \
  Pyramidoid*: _ShapoidSetAxis, \
  Spheroid*: _ShapoidSetAxis, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), I, V)

#define ShapoidSetPos(T, V) _Generic((T), \
  Shapoid*: _ShapoidSetPos, \
  Facoid*: _ShapoidSetPos, \
  Pyramidoid*: _ShapoidSetPos, \
  Spheroid*: _ShapoidSetPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), V)

#define ShapoidGetAxis(T, I) _Generic((T), \
  Shapoid*: _ShapoidGetAxis, \
  Facoid*: _ShapoidGetAxis, \
  Pyramidoid*: _ShapoidGetAxis, \
  Spheroid*: _ShapoidGetAxis, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), I)

#define ShapoidAxis(T, I) _Generic((T), \
  Shapoid*: _ShapoidAxis, \
  Facoid*: _ShapoidAxis, \
  Pyramidoid*: _ShapoidAxis, \
  Spheroid*: _ShapoidAxis, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), I)

#define ShapoidTranslate(T, V) _Generic((T), \
  Shapoid*: _ShapoidTranslate, \
  Facoid*: _ShapoidTranslate, \
  Pyramidoid*: _ShapoidTranslate, \
  Spheroid*: _ShapoidTranslate, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), V)

#define ShapoidIsEqual(T, O) _Generic((T), \
  Shapoid*: _ShapoidIsEqual, \
  Facoid*: _ShapoidIsEqual, \
  Pyramidoid*: _ShapoidIsEqual, \
  Spheroid*: _ShapoidIsEqual, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), (Shapoid*)(O))

#define ShapoidGetBoundingBox(T) _Generic((T), \
  Shapoid*: _ShapoidGetBoundingBox, \
  Facoid*: FacoidGetBoundingBox, \
  Pyramidoid*: PyramidoidGetBoundingBox, \
  Spheroid*: SpheroidGetBoundingBox, \
  GSet*: ShapoidGetBoundingBoxSet, \
  default: PBErrInvalidPolymorphism)(T)

#define ShapoidScale(T, C) _Generic((C), \
  VecFloat*: ShapoidScaleVector, \
  float: ShapoidScaleScalar, \
  default: PBErrInvalidPolymorphism)((Shapoid*)T, C)

#define ShapoidGrow(T, C) _Generic((T), \
  Shapoid*: _Generic((C), \
    VecFloat*: ShapoidGrowVector, \
    float: ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  Facoid*: _Generic((C), \
    VecFloat*: ShapoidGrowVector, \
    float: ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  Pyramidoid*: _Generic((C), \
    VecFloat*: ShapoidGrowVector, \
    float: ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  Spheroid*: _Generic((C), \
    VecFloat*: ShapoidGrowVector, \
    float: ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism)((Shapoid*)T, C)

#define ShapoidGetCenter(T) _Generic((T), \
  Shapoid*: _ShapoidGetCenter, \
  Facoid*: FacoidGetCenter, \
  Pyramidoid*: PyramidoidGetCenter, \
  Spheroid*: SpheroidGetCenter, \
  default: PBErrInvalidPolymorphism)(T)

#define ShapoidRotate2D(T, A) _Generic((T), \
  Shapoid*: _ShapoidRotate2D, \
  Facoid*: _ShapoidRotate2D, \
  Pyramidoid*: _ShapoidRotate2D, \
  Spheroid*: _ShapoidRotate2D, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), A)

#define ShapoidImportCoord(T, P) _Generic((T), \
  Shapoid*: _ShapoidImportCoord, \
  Facoid*: _ShapoidImportCoord, \
  Pyramidoid*: _ShapoidImportCoord, \
  Spheroid*: _ShapoidImportCoord, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), P)

#define ShapoidExportCoord(T, P) _Generic((T), \
  Shapoid*: _ShapoidExportCoord, \
  Facoid*: _ShapoidExportCoord, \
  Pyramidoid*: _ShapoidExportCoord, \
  Spheroid*: _ShapoidExportCoord, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(T), P)

#define ShapoidIsPosInside(T, P) _Generic((T), \
  Shapoid*: _ShapoidIsPosInside, \
  Facoid*: FacoidIsPosInside, \
  Pyramidoid*: PyramidoidIsPosInside, \
  Spheroid*: SpheroidIsPosInside, \
  default: PBErrInvalidPolymorphism)(T, P)

#define ShapoidGetPosDepth(T, P) _Generic((T), \
  Shapoid*: _ShapoidGetPosDepth, \
  Facoid*: FacoidGetPosDepth, \
  Pyramidoid*: PyramidoidGetPosDepth, \
  Spheroid*: SpheroidGetPosDepth, \
  default: PBErrInvalidPolymorphism)(T, P)

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

CloneShapoidType Facoid;
CloneShapoidType Pyramidoid;
CloneShapoidType Spheroid;

// ================ Functions declaration ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
Shapoid* ShapoidCreate(int dim, ShapoidType type);

// Clone a Shapoid
Shapoid* _ShapoidClone(Shapoid *that);
inline Facoid* FacoidClone(Facoid *that) {
  return (Facoid*)_ShapoidClone((Shapoid*)that);
}
inline Pyramidoid* PyramidoidClone(Pyramidoid *that) {
  return (Pyramidoid*)_ShapoidClone((Shapoid*)that);
}
inline Spheroid* SpheroidClone(Spheroid *that) {
  return (Spheroid*)_ShapoidClone((Shapoid*)that);
}

// Free memory used by a Shapoid
void _ShapoidFree(Shapoid **that);

// Load the Shapoid of type 'type' from the stream
// If the Shapoid is already allocated, it is freed before loading
// Return true upon success else false
bool _ShapoidLoad(Shapoid **that, FILE *stream, ShapoidType type);
inline bool FacoidLoad(Facoid **that, FILE *stream) {
  return _ShapoidLoad((Shapoid**)that, stream, ShapoidTypeFacoid);
}
inline bool PyramidoidLoad(Pyramidoid **that, FILE *stream) {
  return _ShapoidLoad((Shapoid**)that, stream, 
    ShapoidTypePyramidoid);
}
inline bool SpheroidLoad(Spheroid **that, FILE *stream) {
  return _ShapoidLoad((Shapoid**)that, stream, 
    ShapoidTypeSpheroid);
}

// Save the Shapoid to the stream
// Return true upon success else false
bool _ShapoidSave(Shapoid *that, FILE *stream);

// Print the Shapoid on 'stream'
void _ShapoidPrintln(Shapoid *that, FILE *stream);

// Get the dimension of the Shapoid
#if BUILDMODE != 0
inline
#endif 
int _ShapoidGetDim(Shapoid *that);

// Get the type of the Shapoid
#if BUILDMODE != 0
inline
#endif 
ShapoidType _ShapoidGetType(Shapoid *that);

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
#if BUILDMODE != 0
inline
#endif 
const char* _ShapoidGetTypeAsString(Shapoid *that);

// Return a VecFloat equals to the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetPos(Shapoid *that);

// Return a VecFloat equals to the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetAxis(Shapoid *that, int dim);

// Return the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidPos(Shapoid *that);

// Return the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidAxis(Shapoid *that, int dim);

// Set the position of the Shapoid to 'pos'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetPos(Shapoid *that, VecFloat *pos);

// Set the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetAxis(Shapoid *that, int dim, VecFloat *v);

// Translate the Shapoid by 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidTranslate(Shapoid *that, VecFloat *v);

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
void _ShapoidRotate2D(Shapoid *that, float theta);

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidImportCoord(Shapoid *that, VecFloat *pos);

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidExportCoord(Shapoid *that, VecFloat *pos);

// Return true if 'pos' (in stand coordinate system) is inside the 
// Shapoid
// Else return false
#if BUILDMODE != 0
inline
#endif 
bool _ShapoidIsPosInside(Shapoid *that, VecFloat *pos);
#if BUILDMODE != 0
inline
#endif 
bool FacoidIsPosInside(Facoid *that, VecFloat *pos);
#if BUILDMODE != 0
inline
#endif 
bool PyramidoidIsPosInside(Pyramidoid *that, VecFloat *pos);
#if BUILDMODE != 0
inline
#endif 
bool SpheroidIsPosInside(Spheroid *that, VecFloat *pos);

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* _ShapoidGetBoundingBox(Shapoid *that); 
Facoid* FacoidGetBoundingBox(Facoid *that); 
Facoid* PyramidoidGetBoundingBox(Pyramidoid *that); 
Facoid* SpheroidGetBoundingBox(Spheroid *that); 

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* ShapoidGetBoundingBoxSet(GSet *set);

// Get the depth value in the Shapoid of 'pos' in standard coordinate
// system
// The depth is defined as follow: the point with depth equals 1.0 is 
// the farthest point from the surface of the Shapoid (inside it),
// points with depth equals to 0.0 are point on the surface of the
// Shapoid. Depth is continuous and derivable over the volume of the
// Shapoid
// Return 0.0 if pos is outside the Shapoid
#if BUILDMODE != 0
inline
#endif 
float _ShapoidGetPosDepth(Shapoid *that, VecFloat *pos);
#if BUILDMODE != 0
inline
#endif 
float FacoidGetPosDepth(Facoid *that, VecFloat *pos);
#if BUILDMODE != 0
inline
#endif 
float PyramidoidGetPosDepth(Pyramidoid *that, VecFloat *pos);
#if BUILDMODE != 0
inline
#endif 
float SpheroidGetPosDepth(Spheroid *that, VecFloat *pos);

// Get the center of the shapoid in standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetCenter(Shapoid *that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* FacoidGetCenter(Facoid *that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* PyramidoidGetCenter(Pyramidoid *that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* SpheroidGetCenter(Spheroid *that);

// Get the percentage of 'tho' included into 'that' (in [0.0, 1.0])
// 0.0 -> 'tho' is completely outside of 'that'
// 1.0 -> 'tho' is completely inside of 'that'
// 'that' and 'tho' must me of same dimensions
// delta is the step of the algorithm (in ]0.0, 1.0])
// small -> slow but precise
// big -> fast but rough
float _ShapoidGetCoverageDelta(Shapoid *that, Shapoid *tho, float delta);

// Update the system of linear equation used to import coordinates
#if BUILDMODE != 0
inline
#endif 
void ShapoidUpdateSysLinEqImport(Shapoid *that);

// Check if shapoid 'that' and 'tho' are equals
#if BUILDMODE != 0
inline
#endif 
bool _ShapoidIsEqual(Shapoid *that, Shapoid *tho);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "shapoid-inline.c"
#endif


#endif


