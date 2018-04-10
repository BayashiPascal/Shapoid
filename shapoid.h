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

#define CloneShapoidType(T) typedef struct T {Shapoid _s;} T

#define SpheroidCreate(Dim) \
  (Spheroid*)ShapoidCreate(Dim, ShapoidTypeSpheroid)
#define FacoidCreate(Dim) \
  (Facoid*)ShapoidCreate(Dim, ShapoidTypeFacoid)
#define PyramidoidCreate(Dim) \
  (Pyramidoid*)ShapoidCreate(Dim, ShapoidTypePyramidoid)

#define ShapoidGetCoverage(ShapoidA, ShapoidB) \
  _ShapoidGetCoverageDelta((Shapoid*)ShapoidA, (Shapoid*)ShapoidB, 0.1)
#define ShapoidGetCoverageDelta(ShapoidA, ShapoidB, Prec) \
  _ShapoidGetCoverageDelta((Shapoid*)ShapoidA, (Shapoid*)ShapoidB, Prec)

#define ShapoidIterCreateStatic(Shap, Delta) \
  _ShapoidIterCreateStatic((Shapoid*)(Shap), (VecFloat*)(Delta))

#define ShapoidIterSetShapoid(Iter, Shap) \
  _ShapoidIterSetShapoid(Iter, (Shapoid*)(Shap))

#define ShapoidIterSetDelta(Iter, Delta) \
  _ShapoidIterSetDelta(Iter, (VecFloat*)(Delta))

extern const char* ShapoidTypeString[3];

// ================= Polymorphism ==================

#define ShapoidClone(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidClone, \
  Facoid*: FacoidClone, \
  Pyramidoid*: PyramidoidClone, \
  Spheroid*: SpheroidClone, \
  default: PBErrInvalidPolymorphism)(Shap)

#define ShapoidFree(ShapRef) _Generic(ShapRef, \
  Shapoid**: _ShapoidFree, \
  Facoid**: _ShapoidFree, \
  Pyramidoid**: _ShapoidFree, \
  Spheroid**: _ShapoidFree, \
  default: PBErrInvalidPolymorphism)((Shapoid**)(ShapRef))

#define ShapoidLoad(ShapRef, Stream) _Generic(ShapRef, \
  Shapoid**: _ShapoidLoad, \
  Facoid**: FacoidLoad, \
  Pyramidoid**: PyramidoidLoad, \
  Spheroid**: SpheroidLoad, \
  default: PBErrInvalidPolymorphism)(ShapRef, Stream)

#define ShapoidSave(Shap, Stream) _Generic(Shap, \
  Shapoid*: _ShapoidSave, \
  Facoid*: _ShapoidSave, \
  Pyramidoid*: _ShapoidSave, \
  Spheroid*: _ShapoidSave, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Stream)

#define ShapoidPrintln(Shap, Stream) _Generic(Shap, \
  Shapoid*: _ShapoidPrintln, \
  Facoid*: _ShapoidPrintln, \
  Pyramidoid*: _ShapoidPrintln, \
  Spheroid*: _ShapoidPrintln, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Stream)

#define ShapoidGetType(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetType, \
  Facoid*: _ShapoidGetType, \
  Pyramidoid*: _ShapoidGetType, \
  Spheroid*: _ShapoidGetType, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap))

#define ShapoidGetTypeAsString(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetTypeAsString, \
  Facoid*: _ShapoidGetTypeAsString, \
  Pyramidoid*: _ShapoidGetTypeAsString, \
  Spheroid*: _ShapoidGetTypeAsString, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap))

#define ShapoidGetDim(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetDim, \
  Facoid*: _ShapoidGetDim, \
  Pyramidoid*: _ShapoidGetDim, \
  Spheroid*: _ShapoidGetDim, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap))

#define ShapoidGetPos(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetPos, \
  Facoid*: _ShapoidGetPos, \
  Pyramidoid*: _ShapoidGetPos, \
  Spheroid*: _ShapoidGetPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap))

#define ShapoidPos(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidPos, \
  Facoid*: _ShapoidPos, \
  Pyramidoid*: _ShapoidPos, \
  Spheroid*: _ShapoidPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap))

#define ShapoidSetAxis(Shap, Index, Vec) _Generic(Shap, \
  Shapoid*: _ShapoidSetAxis, \
  Facoid*: _ShapoidSetAxis, \
  Pyramidoid*: _ShapoidSetAxis, \
  Spheroid*: _ShapoidSetAxis, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Index, \
    (VecFloat*)Vec)

#define ShapoidSetPos(Shap, Vec) _Generic(Shap, \
  Shapoid*: _ShapoidSetPos, \
  Facoid*: _ShapoidSetPos, \
  Pyramidoid*: _ShapoidSetPos, \
  Spheroid*: _ShapoidSetPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), (VecFloat*)Vec)

#define ShapoidGetAxis(Shap, Index) _Generic(Shap, \
  Shapoid*: _ShapoidGetAxis, \
  Facoid*: _ShapoidGetAxis, \
  Pyramidoid*: _ShapoidGetAxis, \
  Spheroid*: _ShapoidGetAxis, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Index)

#define ShapoidAxis(Shap, Index) _Generic(Shap, \
  Shapoid*: _ShapoidAxis, \
  Facoid*: _ShapoidAxis, \
  Pyramidoid*: _ShapoidAxis, \
  Spheroid*: _ShapoidAxis, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Index)

#define ShapoidTranslate(Shap, Vec) _Generic(Shap, \
  Shapoid*: _ShapoidTranslate, \
  Facoid*: _ShapoidTranslate, \
  Pyramidoid*: _ShapoidTranslate, \
  Spheroid*: _ShapoidTranslate, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), (VecFloat*)Vec)

#define ShapoidIsEqual(ShapA, ShapB) _Generic(ShapA, \
  Shapoid*: _ShapoidIsEqual, \
  Facoid*: _ShapoidIsEqual, \
  Pyramidoid*: _ShapoidIsEqual, \
  Spheroid*: _ShapoidIsEqual, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(ShapA), \
    (Shapoid*)(ShapB))

#define ShapoidGetBoundingBox(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetBoundingBox, \
  Facoid*: FacoidGetBoundingBox, \
  Pyramidoid*: PyramidoidGetBoundingBox, \
  Spheroid*: SpheroidGetBoundingBox, \
  GSetShapoid*: ShapoidGetBoundingBoxSet, \
  default: PBErrInvalidPolymorphism)(Shap)

#define ShapoidScale(Shap, Scale) _Generic(Scale, \
  VecFloat*: _ShapoidScaleVector, \
  VecFloat2D*: _ShapoidScaleVector, \
  VecFloat3D*: _ShapoidScaleVector, \
  float: _ShapoidScaleScalar, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Scale)

#define ShapoidGrow(Shap, Scale) _Generic(Shap, \
  Shapoid*: _Generic(Scale, \
    VecFloat*: _ShapoidGrowVector, \
    float: _ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  Facoid*: _Generic(Scale, \
    VecFloat*: _ShapoidGrowVector, \
    float: _ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  Pyramidoid*: _Generic(Scale, \
    VecFloat*: _ShapoidGrowVector, \
    float: _ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  Spheroid*: _Generic(Scale, \
    VecFloat*: _ShapoidGrowVector, \
    float: _ShapoidGrowScalar, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Scale)

#define ShapoidGetCenter(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetCenter, \
  Facoid*: FacoidGetCenter, \
  Pyramidoid*: PyramidoidGetCenter, \
  Spheroid*: SpheroidGetCenter, \
  default: PBErrInvalidPolymorphism)(Shap)

#define ShapoidRotCenter(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotCenter, \
  Facoid*: _ShapoidRotCenter, \
  Pyramidoid*: _ShapoidRotCenter, \
  Spheroid*: _ShapoidRotCenter, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotOrigin(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotOrigin, \
  Facoid*: _ShapoidRotOrigin, \
  Pyramidoid*: _ShapoidRotOrigin, \
  Spheroid*: _ShapoidRotOrigin, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotStart(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotStart, \
  Facoid*: _ShapoidRotStart, \
  Pyramidoid*: _ShapoidRotStart, \
  Spheroid*: _ShapoidRotStart, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotAxisCenter(Shap, Axis, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotAxisCenter, \
  Facoid*: _ShapoidRotAxisCenter, \
  Pyramidoid*: _ShapoidRotAxisCenter, \
  Spheroid*: _ShapoidRotAxisCenter, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Axis, Theta)

#define ShapoidRotAxisOrigin(Shap, Axis, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotAxisOrigin, \
  Facoid*: _ShapoidRotAxisOrigin, \
  Pyramidoid*: _ShapoidRotAxisOrigin, \
  Spheroid*: _ShapoidRotAxisOrigin, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Axis, Theta)

#define ShapoidRotAxisStart(Shap, Axis, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotAxisStart, \
  Facoid*: _ShapoidRotAxisStart, \
  Pyramidoid*: _ShapoidRotAxisStart, \
  Spheroid*: _ShapoidRotAxisStart, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Axis, Theta)

#define ShapoidRotXCenter(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotXCenter, \
  Facoid*: _ShapoidRotXCenter, \
  Pyramidoid*: _ShapoidRotXCenter, \
  Spheroid*: _ShapoidRotXCenter, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotXOrigin(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotXOrigin, \
  Facoid*: _ShapoidRotXOrigin, \
  Pyramidoid*: _ShapoidRotXOrigin, \
  Spheroid*: _ShapoidRotXOrigin, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotXStart(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotXStart, \
  Facoid*: _ShapoidRotXStart, \
  Pyramidoid*: _ShapoidRotXStart, \
  Spheroid*: _ShapoidRotXStart, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotYCenter(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotYCenter, \
  Facoid*: _ShapoidRotYCenter, \
  Pyramidoid*: _ShapoidRotYCenter, \
  Spheroid*: _ShapoidRotYCenter, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotYOrigin(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotYOrigin, \
  Facoid*: _ShapoidRotYOrigin, \
  Pyramidoid*: _ShapoidRotYOrigin, \
  Spheroid*: _ShapoidRotYOrigin, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotYStart(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotYStart, \
  Facoid*: _ShapoidRotYStart, \
  Pyramidoid*: _ShapoidRotYStart, \
  Spheroid*: _ShapoidRotYStart, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotZCenter(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotZCenter, \
  Facoid*: _ShapoidRotZCenter, \
  Pyramidoid*: _ShapoidRotZCenter, \
  Spheroid*: _ShapoidRotZCenter, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotZOrigin(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotZOrigin, \
  Facoid*: _ShapoidRotZOrigin, \
  Pyramidoid*: _ShapoidRotZOrigin, \
  Spheroid*: _ShapoidRotZOrigin, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidRotZStart(Shap, Theta) _Generic(Shap, \
  Shapoid*: _ShapoidRotZStart, \
  Facoid*: _ShapoidRotZStart, \
  Pyramidoid*: _ShapoidRotZStart, \
  Spheroid*: _ShapoidRotZStart, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Theta)

#define ShapoidImportCoord(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidImportCoord, \
  Facoid*: _ShapoidImportCoord, \
  Pyramidoid*: _ShapoidImportCoord, \
  Spheroid*: _ShapoidImportCoord, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), \
    (VecFloat*)VecPos)

#define ShapoidExportCoord(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidExportCoord, \
  Facoid*: _ShapoidExportCoord, \
  Pyramidoid*: _ShapoidExportCoord, \
  Spheroid*: _ShapoidExportCoord, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), \
    (VecFloat*)VecPos)

#define ShapoidIsPosInside(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidIsPosInside, \
  Facoid*: FacoidIsPosInside, \
  Pyramidoid*: PyramidoidIsPosInside, \
  Spheroid*: SpheroidIsPosInside, \
  default: PBErrInvalidPolymorphism)(Shap, VecPos)

#define ShapoidGetPosDepth(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidGetPosDepth, \
  Facoid*: FacoidGetPosDepth, \
  Pyramidoid*: PyramidoidGetPosDepth, \
  Spheroid*: SpheroidGetPosDepth, \
  default: PBErrInvalidPolymorphism)(Shap, (VecFloat*)VecPos)

#define ShapoidIsInter(ShapA, ShapB) _Generic(ShapA, \
  Spheroid*: _Generic(ShapB, \
    Spheroid*: _SpheroidIsInterSpheroid, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) (ShapA, ShapB)

// -------------- ShapoidIter

// ================= Data structure ===================

typedef enum ShapoidType {
  ShapoidTypeFacoid, ShapoidTypeSpheroid,
  ShapoidTypePyramidoid
} ShapoidType;
// Don't forget to update ShapoidTypeString in shapoid.c when adding 
// new type

typedef struct Shapoid {
  // Position of origin
  VecFloat* _pos;
  // Dimension
  int _dim;
  // Vectors defining axes
  VecFloat** _axis;
  // Type of Shapoid
  ShapoidType _type;
  // Linear sytem used to import coordinates
  SysLinEq* _sysLinEqImport;
} Shapoid;

CloneShapoidType(Facoid);
CloneShapoidType(Pyramidoid);
typedef struct Spheroid {
  Shapoid _s;
  // Major and minor axis for optimization
  int _majAxis;
  int _minAxis;
} Spheroid;
//CloneShapoidType(Spheroid);

// ================ Functions declaration ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
Shapoid* ShapoidCreate(int dim, ShapoidType type);

// Clone a Shapoid
Shapoid* _ShapoidClone(Shapoid* that);
inline Facoid* FacoidClone(Facoid* that) {
  return (Facoid*)_ShapoidClone((Shapoid*)that);
}
inline Pyramidoid* PyramidoidClone(Pyramidoid* that) {
  return (Pyramidoid*)_ShapoidClone((Shapoid*)that);
}
inline Spheroid* SpheroidClone(Spheroid* that) {
  return (Spheroid*)_ShapoidClone((Shapoid*)that);
}

// Free memory used by a Shapoid
void _ShapoidFree(Shapoid** that);

// Load the Shapoid of type 'type' from the stream
// If the Shapoid is already allocated, it is freed before loading
// Return true upon success else false
bool _ShapoidLoad(Shapoid** that, FILE* stream, ShapoidType type);
inline bool FacoidLoad(Facoid** that, FILE* stream) {
  return _ShapoidLoad((Shapoid**)that, stream, ShapoidTypeFacoid);
}
inline bool PyramidoidLoad(Pyramidoid** that, FILE* stream) {
  return _ShapoidLoad((Shapoid**)that, stream, 
    ShapoidTypePyramidoid);
}
inline bool SpheroidLoad(Spheroid** that, FILE* stream) {
  return _ShapoidLoad((Shapoid**)that, stream, 
    ShapoidTypeSpheroid);
}

// Save the Shapoid to the stream
// Return true upon success else false
bool _ShapoidSave(Shapoid* that, FILE* stream);

// Print the Shapoid on 'stream'
void _ShapoidPrintln(Shapoid* that, FILE* stream);

// Get the dimension of the Shapoid
#if BUILDMODE != 0
inline
#endif 
int _ShapoidGetDim(Shapoid* that);

// Get the type of the Shapoid
#if BUILDMODE != 0
inline
#endif 
ShapoidType _ShapoidGetType(Shapoid* that);

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
#if BUILDMODE != 0
inline
#endif 
const char* _ShapoidGetTypeAsString(Shapoid* that);

// Return a VecFloat equals to the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetPos(Shapoid* that);

// Return a VecFloat equals to the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetAxis(Shapoid* that, int dim);

// Return the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidPos(Shapoid* that);

// Return the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidAxis(Shapoid* that, int dim);

// Set the position of the Shapoid to 'pos'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetPos(Shapoid* that, VecFloat* pos);

// Set the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetAxis(Shapoid* that, int dim, VecFloat* v);

// Translate the Shapoid by 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidTranslate(Shapoid* that, VecFloat* v);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
#if BUILDMODE != 0
inline
#endif 
void _ShapoidScaleVector(Shapoid* that, VecFloat* v);

// Scale the Shapoid by 'c'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidScaleScalar(Shapoid* that, float c);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void _ShapoidGrowVector(Shapoid* that, VecFloat* v);

// Scale the Shapoid by 'c'
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void _ShapoidGrowScalar(Shapoid* that, float c);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its center
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotCenter(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its position
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotStart(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotOrigin(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotAxisCenter(Shapoid* that, VecFloat3D* axis, 
  float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotAxisStart(Shapoid* that, VecFloat3D* axis, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotAxisOrigin(Shapoid* that, VecFloat3D* axis, 
  float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around X
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotXCenter(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around X
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotXStart(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around X
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotXOrigin(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around Y
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotYCenter(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around Y
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotYStart(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around Y
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotYOrigin(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around Z
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotZCenter(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around Z
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotZStart(Shapoid* that, float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around Z
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotZOrigin(Shapoid* that, float theta);

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidImportCoord(Shapoid* that, VecFloat* pos);

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidExportCoord(Shapoid* that, VecFloat* pos);

// Return true if 'pos' (in standard coordinates system) is inside the 
// Shapoid
// Else return false
#if BUILDMODE != 0
inline
#endif 
bool _ShapoidIsPosInside(Shapoid* that, VecFloat* pos);
#if BUILDMODE != 0
inline
#endif 
bool FacoidIsPosInside(Facoid* that, VecFloat* pos);
#if BUILDMODE != 0
inline
#endif 
bool PyramidoidIsPosInside(Pyramidoid* that, VecFloat* pos);
#if BUILDMODE != 0
inline
#endif 
bool SpheroidIsPosInside(Spheroid* that, VecFloat* pos);

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* _ShapoidGetBoundingBox(Shapoid* that); 
Facoid* FacoidGetBoundingBox(Facoid* that); 
Facoid* PyramidoidGetBoundingBox(Pyramidoid* that); 
Facoid* SpheroidGetBoundingBox(Spheroid* that); 

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* ShapoidGetBoundingBoxSet(GSetShapoid* set);

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
float _ShapoidGetPosDepth(Shapoid* that, VecFloat* pos);
#if BUILDMODE != 0
inline
#endif 
float FacoidGetPosDepth(Facoid* that, VecFloat* pos);
#if BUILDMODE != 0
inline
#endif 
float PyramidoidGetPosDepth(Pyramidoid* that, VecFloat* pos);
#if BUILDMODE != 0
inline
#endif 
float SpheroidGetPosDepth(Spheroid* that, VecFloat* pos);

// Get the center of the shapoid in standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetCenter(Shapoid* that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* FacoidGetCenter(Facoid* that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* PyramidoidGetCenter(Pyramidoid* that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* SpheroidGetCenter(Spheroid* that);

// Get the percentage of 'tho' included into 'that' (in [0.0, 1.0])
// 0.0 -> 'tho' is completely outside of 'that'
// 1.0 -> 'tho' is completely inside of 'that'
// 'that' and 'tho' must me of same dimensions
// delta is the step of the algorithm (in ]0.0, 1.0])
// small -> slow but precise
// big -> fast but rough
float _ShapoidGetCoverageDelta(Shapoid* that, Shapoid* tho, float delta);

// Update the system of linear equation used to import coordinates
#if BUILDMODE != 0
inline
#endif 
void ShapoidUpdateSysLinEqImport(Shapoid* that);

// Check if shapoid 'that' and 'tho' are equals
#if BUILDMODE != 0
inline
#endif 
bool _ShapoidIsEqual(Shapoid* that, Shapoid* tho);

// Add a copy of the Facoid 'that' to the GSet 'set' (containing 
// other Facoid), taking care to avoid overlaping Facoid
// The copy of 'that' made be resized or divided
// The Facoid in the set and 'that' must be aligned with the 
// coordinates system axis and have 
// same dimensions
void FacoidAlignedAddClippedToSet(Facoid* that, GSetShapoid* set);

// Check if the Facoid 'that' is completely included into the Facoid 
// 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
// Return true if it is included, false else
bool FacoidAlignedIsInsideFacoidAligned(Facoid* that, Facoid* facoid);

// Check if the Facoid 'that' is completely excluded from the Facoid 
// 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
// Return true if it is excluded, false else
bool FacoidAlignedIsOutsideFacoidAligned(Facoid* that, Facoid* facoid);

// Get a GSet of Facoid aligned with coordinates system covering the 
// Facoid 'that' except for area in the Facoid 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
GSetShapoid* FacoidAlignedSplitExcludingFacoidAligned(Facoid* that, 
  Facoid* facoid);

// Return true if 'that' intersects 'tho'
// Return false else
// 'that' and 'tho' must have same dimension
bool _SpheroidIsInterSpheroid(Spheroid* that, Spheroid* tho);

// Update the major and minor axis of the Spheroid 'that'
void SpheroidUpdateMajMinAxis(Spheroid* that);

// -------------- ShapoidIter

// ================= Data structure ===================

typedef struct ShapoidIter {
  // Attached shapoid
  Shapoid* _shap;
  // Delta step
  VecFloat* _delta;
  // Current position (in internal coordinates of the shapoid)
  VecFloat* _pos;
} ShapoidIter;

// ================ Functions declaration ====================

// Create a new iterator on the Shapoid 'shap' with a step of 'delta'
// (step on the internal coordinates of the Shapoid)
// The iterator is initialized and ready to be stepped
ShapoidIter _ShapoidIterCreateStatic(Shapoid* shap, VecFloat* delta);

// Free the memory used by the ShapoidIter 'that'
void ShapoidIterFreeStatic(ShapoidIter* that);

// Reinitialise the ShapoidIter 'that' to its starting position
void ShapoidIterInit(ShapoidIter* that);

// Step the ShapoidIter 'that'
// Return false if the iterator is at its end and couldn't be stepped
bool ShapoidIterStep(ShapoidIter* that);

// Return the current position in Shapoid coordinates of the 
// ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidIterGetInternal(ShapoidIter* that);

// Return the current position in standard coordinates of the 
// ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidIterGetExternal(ShapoidIter* that);

// Set the attached Shapoid of the ShapoidIter 'that' to 'shap'
// The iterator is reset to its initial position
#if BUILDMODE != 0
inline
#endif 
void _ShapoidIterSetShapoid(ShapoidIter* that, Shapoid* shap);

// Get the Shapoid of the ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
Shapoid* ShapoidIterShapoid(ShapoidIter* that);

// Set the delta of the ShapoidIter 'that' to a copy of 'delta'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidIterSetDelta(ShapoidIter* that, VecFloat* delta);

// Get the delta of the ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidIterDelta(ShapoidIter* that);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "shapoid-inline.c"
#endif


#endif


