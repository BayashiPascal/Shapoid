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
  _ShapoidGetCoverageDelta((Shapoid* const)ShapoidA, \
  (Shapoid* const)ShapoidB, 0.1)
#define ShapoidGetCoverageDelta(ShapoidA, ShapoidB, Prec) \
  _ShapoidGetCoverageDelta((Shapoid* const)ShapoidA, \
  (Shapoid* const)ShapoidB, Prec)

#define ShapoidIterCreateStatic(Shap, Delta) \
  _ShapoidIterCreateStatic((Shapoid* const)(Shap), (VecFloat*)(Delta))

#define ShapoidIterSetShapoid(Iter, Shap) \
  _ShapoidIterSetShapoid(Iter, (Shapoid* const)(Shap))

#define ShapoidIterSetDelta(Iter, Delta) \
  _ShapoidIterSetDelta(Iter, (VecFloat* const)(Delta))

extern const char* ShapoidTypeString[3];

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
Shapoid* ShapoidCreate(const int dim, const ShapoidType type);

// Clone a Shapoid
Shapoid* _ShapoidClone(const Shapoid* const that);
inline Facoid* FacoidClone(const Facoid* const that) {
  return (Facoid*)_ShapoidClone((const Shapoid*)that);
}
inline Pyramidoid* PyramidoidClone(const Pyramidoid* const that) {
  return (Pyramidoid*)_ShapoidClone((const Shapoid*)that);
}
inline Spheroid* SpheroidClone(const Spheroid* const that) {
  return (Spheroid*)_ShapoidClone((const Shapoid*)that);
}

// Free memory used by a Shapoid
void _ShapoidFree(Shapoid** that);

// Function which return the JSON encoding of 'that' 
JSONNode* _ShapoidEncodeAsJSON(const Shapoid* const that);

// Function which decode from JSON encoding 'json' to 'that'
bool _ShapoidDecodeAsJSON(Shapoid** that, const JSONNode* const json);

// Load the Shapoid of type 'type' from the stream
// If the Shapoid is already allocated, it is freed before loading
// Return true upon success else false
bool _ShapoidLoad(Shapoid** that, FILE* const stream);
#if BUILDMODE != 0
inline
#endif 
bool FacoidLoad(Facoid** that, FILE* const stream);
#if BUILDMODE != 0
inline
#endif 
bool PyramidoidLoad(Pyramidoid** that, FILE* const stream);
#if BUILDMODE != 0
inline
#endif 
bool SpheroidLoad(Spheroid** that, FILE* const stream);

// Save the Shapoid to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success else false
bool _ShapoidSave(const Shapoid* const that, FILE* const stream, 
  const bool compact);

// Print the Shapoid on 'stream'
void _ShapoidPrintln(const Shapoid* const that, FILE* const stream);

// Get the dimension of the Shapoid
#if BUILDMODE != 0
inline
#endif 
int _ShapoidGetDim(const Shapoid* const that);

// Get the type of the Shapoid
#if BUILDMODE != 0
inline
#endif 
ShapoidType _ShapoidGetType(const Shapoid* const that);

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
#if BUILDMODE != 0
inline
#endif 
const char* _ShapoidGetTypeAsString(const Shapoid* const that);

// Return a VecFloat equals to the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetPos(const Shapoid* const that);

// Return a VecFloat equals to the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetAxis(const Shapoid* const that, const int dim);

// Return the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
const VecFloat* _ShapoidPos(const Shapoid* const that);

// Return the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
const VecFloat* _ShapoidAxis(const Shapoid* const that, const int dim);

// Set the position of the Shapoid to 'pos'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetPos(Shapoid* const that, const VecFloat* const pos);

// Set the 'iElem'-th value of the position of the Shapoid to 'val'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidPosSet(Shapoid* const that, const int iElem, 
  const float val);

// Set the 'iElem'-th value of the position of the Shapoid to 'val' 
// added to its current value
#if BUILDMODE != 0
inline
#endif 
void _ShapoidPosSetAdd(Shapoid* const that, const int iElem, 
  const float val);

// Set the position of the Shapoid such as its center is at 'pos'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetCenterPos(Shapoid* const that, 
  const VecFloat* const pos);

// Set the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidSetAxis(Shapoid* const that, const int dim, 
  const VecFloat* const v);

// Set the 'iElem'-th element of the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidAxisSet(Shapoid* const that, const int dim, 
  const int iElem, const float v);

// Set the 'iElem'-th element of the 'dim'-th axis of the Shapoid to 
// 'v' added to its current value
#if BUILDMODE != 0
inline
#endif 
void _ShapoidAxisSetAdd(Shapoid* const that, const int dim, 
  const int iElem, const float v);

// Translate the Shapoid by 'v'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidTranslate(Shapoid* const that, const VecFloat* const v);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
#if BUILDMODE != 0
inline
#endif 
void _ShapoidScaleVector(Shapoid* const that, const VecFloat* const v);

// Scale the Shapoid by 'c'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidScaleScalar(Shapoid* const that, const float c);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void _ShapoidGrowVector(Shapoid* const that, const VecFloat* const v);

// Scale the Shapoid by 'c'
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void _ShapoidGrowScalar(Shapoid* const that, const float c);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its center
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotCenter(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its position
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotStart(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotOrigin(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotAxisCenter(Shapoid* const that, 
  const VecFloat3D* const axis, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotAxisStart(Shapoid* const that, 
  const VecFloat3D* const axis, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotAxisOrigin(Shapoid* const that, 
  const VecFloat3D* const axis, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around X
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotXCenter(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around X
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotXStart(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around X
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotXOrigin(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around Y
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotYCenter(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around Y
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotYStart(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around Y
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotYOrigin(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around Z
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotZCenter(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around Z
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotZStart(Shapoid* const that, const float theta);

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around Z
#if BUILDMODE != 0
inline
#endif 
void _ShapoidRotZOrigin(Shapoid* const that, const float theta);

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidImportCoord(const Shapoid* const that, 
  const VecFloat* const pos);

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidExportCoord(const Shapoid* const that, 
  const VecFloat* const pos);

// Return true if 'pos' (in standard coordinates system) is inside the 
// Shapoid
// Else return false
#if BUILDMODE != 0
inline
#endif 
bool _ShapoidIsPosInside(const Shapoid* const that, 
  const VecFloat* const pos);
#if BUILDMODE != 0
inline
#endif 
bool FacoidIsPosInside(const Facoid* const that, 
  const VecFloat* const pos);
#if BUILDMODE != 0
inline
#endif 
bool PyramidoidIsPosInside(const Pyramidoid* const that, 
  const VecFloat* const pos);
#if BUILDMODE != 0
inline
#endif 
bool SpheroidIsPosInside(const Spheroid* const that, 
  const VecFloat* const pos);

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* _ShapoidGetBoundingBox(const Shapoid* const that); 
Facoid* FacoidGetBoundingBox(const Facoid* const that); 
Facoid* PyramidoidGetBoundingBox(const Pyramidoid* const that); 
Facoid* SpheroidGetBoundingBox(const Spheroid* const that); 

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* ShapoidGetBoundingBoxSet(const GSetShapoid* const set);

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
float _ShapoidGetPosDepth(const Shapoid* const that, 
  const VecFloat* const pos);
#if BUILDMODE != 0
inline
#endif 
float FacoidGetPosDepth(const Facoid* const that, 
  const VecFloat* const pos);
#if BUILDMODE != 0
inline
#endif 
float PyramidoidGetPosDepth(const Pyramidoid* const that, 
  const VecFloat* const pos);
#if BUILDMODE != 0
inline
#endif 
float SpheroidGetPosDepth(const Spheroid* const that, 
  const VecFloat* const pos);

// Get the center of the shapoid in standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* _ShapoidGetCenter(const Shapoid* const that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* FacoidGetCenter(const Facoid* const that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* PyramidoidGetCenter(const Pyramidoid* const that);
#if BUILDMODE != 0
inline
#endif 
VecFloat* SpheroidGetCenter(const Spheroid* const that);

// Get the percentage of 'tho' included into 'that' (in [0.0, 1.0])
// 0.0 -> 'tho' is completely outside of 'that'
// 1.0 -> 'tho' is completely inside of 'that'
// 'that' and 'tho' must me of same dimensions
// delta is the step of the algorithm (in ]0.0, 1.0])
// small -> slow but precise
// big -> fast but rough
float _ShapoidGetCoverageDelta(const Shapoid* const that, 
  const Shapoid* const tho, const float delta);

// Update the system of linear equation used to import coordinates
#if BUILDMODE != 0
inline
#endif 
void ShapoidUpdateSysLinEqImport(Shapoid* const that);

// Check if shapoid 'that' and 'tho' are equals
#if BUILDMODE != 0
inline
#endif 
bool _ShapoidIsEqual(const Shapoid* const that, 
  const Shapoid* const tho);

// Add a copy of the Facoid 'that' to the GSet 'set' (containing 
// other Facoid), taking care to avoid overlaping Facoid
// The copy of 'that' may be resized or divided
// The Facoid in the set and 'that' must be aligned with the 
// coordinates system axis and have 
// same dimensions
void FacoidAlignedAddClippedToSet(const Facoid* const that, 
  GSetShapoid* const set);

// Check if the Facoid 'that' is completely included into the Facoid 
// 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
// Return true if it is included, false else
bool FacoidAlignedIsInsideFacoidAligned(const Facoid* const that, 
  const Facoid* const facoid);

// Check if the Facoid 'that' is completely excluded from the Facoid 
// 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
// Return true if it is excluded, false else
bool FacoidAlignedIsOutsideFacoidAligned(const Facoid* const that, 
  const Facoid* const facoid);

// Get a GSet of Facoid aligned with coordinates system covering the 
// Facoid 'that' except for area in the Facoid 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
GSetShapoid* FacoidAlignedSplitExcludingFacoidAligned(
  const Facoid* const that, const Facoid* const facoid);

// Return true if 'that' intersects 'tho'
// Return false else
// 'that' and 'tho' must have same dimension
bool _SpheroidIsInterSpheroid(const Spheroid* const that, 
  const Spheroid* const tho);

// Update the major and minor axis of the Spheroid 'that'
void SpheroidUpdateMajMinAxis(Spheroid* const that);

// Get the maximum distance from the center of the Shapoid 'that' and 
// its surface
// Currenty only defined for spheroid, return 0.0 else
float _ShapoidGetBoundingRadius(const Shapoid* const that);

// -------------- ShapoidIter

// ================= Data structure ===================

typedef struct ShapoidIter {
  // Attached shapoid
  const Shapoid* _shap;
  // Delta step
  VecFloat* _delta;
  // Current position (in internal coordinates of the shapoid)
  VecFloat* _pos;
} ShapoidIter;

// ================ Functions declaration ====================

// Create a new iterator on the Shapoid 'shap' with a step of 'delta'
// (step on the internal coordinates of the Shapoid)
// The iterator is initialized and ready to be stepped
ShapoidIter _ShapoidIterCreateStatic(const Shapoid* const shap, 
  const VecFloat* const delta);

// Free the memory used by the ShapoidIter 'that'
void ShapoidIterFreeStatic(ShapoidIter* const that);

// Reinitialise the ShapoidIter 'that' to its starting position
void ShapoidIterInit(ShapoidIter* const that);

// Step the ShapoidIter 'that'
// Return false if the iterator is at its end and couldn't be stepped
bool ShapoidIterStep(ShapoidIter* const that);

// Return the current position in Shapoid coordinates of the 
// ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidIterGetInternalPos(const ShapoidIter* const that);

// Return the current position in standard coordinates of the 
// ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidIterGetExternalPos(const ShapoidIter* const that);

// Set the attached Shapoid of the ShapoidIter 'that' to 'shap'
// The iterator is reset to its initial position
#if BUILDMODE != 0
inline
#endif 
void _ShapoidIterSetShapoid(ShapoidIter* const that, 
  const Shapoid* const shap);

// Get the Shapoid of the ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
const Shapoid* ShapoidIterShapoid(const ShapoidIter* const that);

// Set the delta of the ShapoidIter 'that' to a copy of 'delta'
#if BUILDMODE != 0
inline
#endif 
void _ShapoidIterSetDelta(ShapoidIter* const that, 
  const VecFloat* const delta);

// Get the delta of the ShapoidIter 'that'
#if BUILDMODE != 0
inline
#endif 
const VecFloat* ShapoidIterDelta(const ShapoidIter* const that);

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

#define ShapoidEncodeAsJSON(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidEncodeAsJSON, \
  Facoid*: _ShapoidEncodeAsJSON, \
  Pyramidoid*: _ShapoidEncodeAsJSON, \
  Spheroid*: _ShapoidEncodeAsJSON, \
  const Shapoid*: _ShapoidEncodeAsJSON, \
  const Facoid*: _ShapoidEncodeAsJSON, \
  const Pyramidoid*: _ShapoidEncodeAsJSON, \
  const Spheroid*: _ShapoidEncodeAsJSON, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)Shap)

#define ShapoidDecodeAsJSON(ShapRef, Json) _Generic(ShapRef, \
  Shapoid**: _ShapoidDecodeAsJSON, \
  Facoid**: _ShapoidDecodeAsJSON, \
  Pyramidoid**: _ShapoidDecodeAsJSON, \
  Spheroid**: _ShapoidDecodeAsJSON, \
  default: PBErrInvalidPolymorphism)((Shapoid**)ShapRef, Json)

#define ShapoidLoad(ShapRef, Stream) _Generic(ShapRef, \
  Shapoid**: _ShapoidLoad, \
  Facoid**: FacoidLoad, \
  Pyramidoid**: PyramidoidLoad, \
  Spheroid**: SpheroidLoad, \
  default: PBErrInvalidPolymorphism)(ShapRef, Stream)

#define ShapoidSave(Shap, Stream, Compact) _Generic(Shap, \
  Shapoid*: _ShapoidSave, \
  Facoid*: _ShapoidSave, \
  Pyramidoid*: _ShapoidSave, \
  Spheroid*: _ShapoidSave, \
  const Shapoid*: _ShapoidSave, \
  const Facoid*: _ShapoidSave, \
  const Pyramidoid*: _ShapoidSave, \
  const Spheroid*: _ShapoidSave, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap), \
    Stream, Compact)

#define ShapoidPrintln(Shap, Stream) _Generic(Shap, \
  Shapoid*: _ShapoidPrintln, \
  Facoid*: _ShapoidPrintln, \
  Pyramidoid*: _ShapoidPrintln, \
  Spheroid*: _ShapoidPrintln, \
  const Shapoid*: _ShapoidPrintln, \
  const Facoid*: _ShapoidPrintln, \
  const Pyramidoid*: _ShapoidPrintln, \
  const Spheroid*: _ShapoidPrintln, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap), Stream)

#define ShapoidGetType(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetType, \
  Facoid*: _ShapoidGetType, \
  Pyramidoid*: _ShapoidGetType, \
  Spheroid*: _ShapoidGetType, \
  const Shapoid*: _ShapoidGetType, \
  const Facoid*: _ShapoidGetType, \
  const Pyramidoid*: _ShapoidGetType, \
  const Spheroid*: _ShapoidGetType, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap))

#define ShapoidGetTypeAsString(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetTypeAsString, \
  Facoid*: _ShapoidGetTypeAsString, \
  Pyramidoid*: _ShapoidGetTypeAsString, \
  Spheroid*: _ShapoidGetTypeAsString, \
  const Shapoid*: _ShapoidGetTypeAsString, \
  const Facoid*: _ShapoidGetTypeAsString, \
  const Pyramidoid*: _ShapoidGetTypeAsString, \
  const Spheroid*: _ShapoidGetTypeAsString, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap))

#define ShapoidGetDim(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetDim, \
  Facoid*: _ShapoidGetDim, \
  Pyramidoid*: _ShapoidGetDim, \
  Spheroid*: _ShapoidGetDim, \
  const Shapoid*: _ShapoidGetDim, \
  const Facoid*: _ShapoidGetDim, \
  const Pyramidoid*: _ShapoidGetDim, \
  const Spheroid*: _ShapoidGetDim, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap))

#define ShapoidGetPos(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetPos, \
  Facoid*: _ShapoidGetPos, \
  Pyramidoid*: _ShapoidGetPos, \
  Spheroid*: _ShapoidGetPos, \
  const Shapoid*: _ShapoidGetPos, \
  const Facoid*: _ShapoidGetPos, \
  const Pyramidoid*: _ShapoidGetPos, \
  const Spheroid*: _ShapoidGetPos, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap))

#define ShapoidPos(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidPos, \
  Facoid*: _ShapoidPos, \
  Pyramidoid*: _ShapoidPos, \
  Spheroid*: _ShapoidPos, \
  const Shapoid*: _ShapoidPos, \
  const Facoid*: _ShapoidPos, \
  const Pyramidoid*: _ShapoidPos, \
  const Spheroid*: _ShapoidPos, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap))

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

#define ShapoidPosSet(Shap, Index, Val) _Generic(Shap, \
  Shapoid*: _ShapoidPosSet, \
  Facoid*: _ShapoidPosSet, \
  Pyramidoid*: _ShapoidPosSet, \
  Spheroid*: _ShapoidPosSet, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Index, Val)

#define ShapoidPosSetAdd(Shap, Index, Val) _Generic(Shap, \
  Shapoid*: _ShapoidPosSetAdd, \
  Facoid*: _ShapoidPosSetAdd, \
  Pyramidoid*: _ShapoidPosSetAdd, \
  Spheroid*: _ShapoidPosSetAdd, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Index, Val)

#define ShapoidSetCenterPos(Shap, Vec) _Generic(Shap, \
  Shapoid*: _ShapoidSetCenterPos, \
  Facoid*: _ShapoidSetCenterPos, \
  Pyramidoid*: _ShapoidSetCenterPos, \
  Spheroid*: _ShapoidSetCenterPos, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), (VecFloat*)Vec)

#define ShapoidGetAxis(Shap, Index) _Generic(Shap, \
  Shapoid*: _ShapoidGetAxis, \
  Facoid*: _ShapoidGetAxis, \
  Pyramidoid*: _ShapoidGetAxis, \
  Spheroid*: _ShapoidGetAxis, \
  const Shapoid*: _ShapoidGetAxis, \
  const Facoid*: _ShapoidGetAxis, \
  const Pyramidoid*: _ShapoidGetAxis, \
  const Spheroid*: _ShapoidGetAxis, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap), Index)

#define ShapoidAxis(Shap, Index) _Generic(Shap, \
  Shapoid*: _ShapoidAxis, \
  Facoid*: _ShapoidAxis, \
  Pyramidoid*: _ShapoidAxis, \
  Spheroid*: _ShapoidAxis, \
  const Shapoid*: _ShapoidAxis, \
  const Facoid*: _ShapoidAxis, \
  const Pyramidoid*: _ShapoidAxis, \
  const Spheroid*: _ShapoidAxis, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap), Index)

#define ShapoidAxisSet(Shap, Dim, Index, Val) _Generic(Shap, \
  Shapoid*: _ShapoidAxisSet, \
  Facoid*: _ShapoidAxisSet, \
  Pyramidoid*: _ShapoidAxisSet, \
  Spheroid*: _ShapoidAxisSet, \
  const Shapoid*: _ShapoidAxisSet, \
  const Facoid*: _ShapoidAxisSet, \
  const Pyramidoid*: _ShapoidAxisSet, \
  const Spheroid*: _ShapoidAxisSet, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Dim, Index, Val)

#define ShapoidAxisSetAdd(Shap, Dim, Index, Val) _Generic(Shap, \
  Shapoid*: _ShapoidAxisSetAdd, \
  Facoid*: _ShapoidAxisSetAdd, \
  Pyramidoid*: _ShapoidAxisSetAdd, \
  Spheroid*: _ShapoidAxisSetAdd, \
  const Shapoid*: _ShapoidAxisSetAdd, \
  const Facoid*: _ShapoidAxisSetAdd, \
  const Pyramidoid*: _ShapoidAxisSetAdd, \
  const Spheroid*: _ShapoidAxisSetAdd, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), Dim, Index, Val)

#define ShapoidTranslate(Shap, Vec) _Generic(Shap, \
  Shapoid*: _ShapoidTranslate, \
  Facoid*: _ShapoidTranslate, \
  Pyramidoid*: _ShapoidTranslate, \
  Spheroid*: _ShapoidTranslate, \
  default: PBErrInvalidPolymorphism)((Shapoid*)(Shap), (VecFloat*)Vec)

#define ShapoidIsEqual(ShapA, ShapB) _Generic(ShapA, \
  Shapoid*: _Generic(ShapB, \
    Shapoid*: _ShapoidIsEqual, \
    Facoid*: _ShapoidIsEqual, \
    Pyramidoid*: _ShapoidIsEqual, \
    Spheroid*: _ShapoidIsEqual, \
    const Shapoid*: _ShapoidIsEqual, \
    const Facoid*: _ShapoidIsEqual, \
    const Pyramidoid*: _ShapoidIsEqual, \
    const Spheroid*: _ShapoidIsEqual, \
    default: PBErrInvalidPolymorphism), \
  Facoid*: _Generic(ShapB, \
    Shapoid*: _ShapoidIsEqual, \
    Facoid*: _ShapoidIsEqual, \
    Pyramidoid*: _ShapoidIsEqual, \
    Spheroid*: _ShapoidIsEqual, \
    const Shapoid*: _ShapoidIsEqual, \
    const Facoid*: _ShapoidIsEqual, \
    const Pyramidoid*: _ShapoidIsEqual, \
    const Spheroid*: _ShapoidIsEqual, \
    default: PBErrInvalidPolymorphism), \
  Pyramidoid*: _Generic(ShapB, \
    Shapoid*: _ShapoidIsEqual, \
    Facoid*: _ShapoidIsEqual, \
    Pyramidoid*: _ShapoidIsEqual, \
    Spheroid*: _ShapoidIsEqual, \
    const Shapoid*: _ShapoidIsEqual, \
    const Facoid*: _ShapoidIsEqual, \
    const Pyramidoid*: _ShapoidIsEqual, \
    const Spheroid*: _ShapoidIsEqual, \
    default: PBErrInvalidPolymorphism), \
  Spheroid*: _Generic(ShapB, \
    Shapoid*: _ShapoidIsEqual, \
    Facoid*: _ShapoidIsEqual, \
    Pyramidoid*: _ShapoidIsEqual, \
    Spheroid*: _ShapoidIsEqual, \
    const Shapoid*: _ShapoidIsEqual, \
    const Facoid*: _ShapoidIsEqual, \
    const Pyramidoid*: _ShapoidIsEqual, \
    const Spheroid*: _ShapoidIsEqual, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(ShapA), \
    (const Shapoid*)(ShapB))

#define ShapoidGetBoundingBox(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetBoundingBox, \
  Facoid*: FacoidGetBoundingBox, \
  Pyramidoid*: PyramidoidGetBoundingBox, \
  Spheroid*: SpheroidGetBoundingBox, \
  const Shapoid*: _ShapoidGetBoundingBox, \
  const Facoid*: FacoidGetBoundingBox, \
  const Pyramidoid*: PyramidoidGetBoundingBox, \
  const Spheroid*: SpheroidGetBoundingBox, \
  GSetShapoid*: ShapoidGetBoundingBoxSet, \
  const GSetShapoid*: ShapoidGetBoundingBoxSet, \
  default: PBErrInvalidPolymorphism)(Shap)

#define ShapoidScale(Shap, Scale) _Generic(Scale, \
  VecFloat*: _ShapoidScaleVector, \
  VecFloat2D*: _ShapoidScaleVector, \
  VecFloat3D*: _ShapoidScaleVector, \
  float: _ShapoidScaleScalar, \
  const VecFloat*: _ShapoidScaleVector, \
  const VecFloat2D*: _ShapoidScaleVector, \
  const VecFloat3D*: _ShapoidScaleVector, \
  const float: _ShapoidScaleScalar, \
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
  const Shapoid*: _ShapoidGetCenter, \
  const Facoid*: FacoidGetCenter, \
  const Pyramidoid*: PyramidoidGetCenter, \
  const Spheroid*: SpheroidGetCenter, \
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
  const Shapoid*: _ShapoidImportCoord, \
  const Facoid*: _ShapoidImportCoord, \
  const Pyramidoid*: _ShapoidImportCoord, \
  const Spheroid*: _ShapoidImportCoord, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap), \
    (const VecFloat*)VecPos)

#define ShapoidExportCoord(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidExportCoord, \
  Facoid*: _ShapoidExportCoord, \
  Pyramidoid*: _ShapoidExportCoord, \
  Spheroid*: _ShapoidExportCoord, \
  const Shapoid*: _ShapoidExportCoord, \
  const Facoid*: _ShapoidExportCoord, \
  const Pyramidoid*: _ShapoidExportCoord, \
  const Spheroid*: _ShapoidExportCoord, \
  default: PBErrInvalidPolymorphism)((const Shapoid*)(Shap), \
    (const VecFloat*)VecPos)

#define ShapoidIsPosInside(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidIsPosInside, \
  Facoid*: FacoidIsPosInside, \
  Pyramidoid*: PyramidoidIsPosInside, \
  Spheroid*: SpheroidIsPosInside, \
  const Shapoid*: _ShapoidIsPosInside, \
  const Facoid*: FacoidIsPosInside, \
  const Pyramidoid*: PyramidoidIsPosInside, \
  const Spheroid*: SpheroidIsPosInside, \
  default: PBErrInvalidPolymorphism)(Shap, VecPos)

#define ShapoidGetPosDepth(Shap, VecPos) _Generic(Shap, \
  Shapoid*: _ShapoidGetPosDepth, \
  Facoid*: FacoidGetPosDepth, \
  Pyramidoid*: PyramidoidGetPosDepth, \
  Spheroid*: SpheroidGetPosDepth, \
  const Shapoid*: _ShapoidGetPosDepth, \
  const Facoid*: FacoidGetPosDepth, \
  const Pyramidoid*: PyramidoidGetPosDepth, \
  const Spheroid*: SpheroidGetPosDepth, \
  default: PBErrInvalidPolymorphism)(Shap, (VecFloat*)VecPos)

#define ShapoidIsInter(ShapA, ShapB) _Generic(ShapA, \
  Spheroid*: _Generic(ShapB, \
    Spheroid*: _SpheroidIsInterSpheroid, \
    const Spheroid*: _SpheroidIsInterSpheroid, \
    default: PBErrInvalidPolymorphism), \
  const Spheroid*: _Generic(ShapB, \
    Spheroid*: _SpheroidIsInterSpheroid, \
    const Spheroid*: _SpheroidIsInterSpheroid, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) (ShapA, ShapB)

#define ShapoidGetBoundingRadius(Shap) _Generic(Shap, \
  Shapoid*: _ShapoidGetBoundingRadius, \
  Pyramidoid*: _ShapoidGetBoundingRadius, \
  Facoid*: _ShapoidGetBoundingRadius, \
  Spheroid*: _ShapoidGetBoundingRadius, \
  const Shapoid*: _ShapoidGetBoundingRadius, \
  const Pyramidoid*: _ShapoidGetBoundingRadius, \
  const Facoid*: _ShapoidGetBoundingRadius, \
  const Spheroid*: _ShapoidGetBoundingRadius, \
  default: PBErrInvalidPolymorphism) ((const Shapoid*)(Shap))
  
// ================ Inliner ====================

#if BUILDMODE != 0
#include "shapoid-inline.c"
#endif


#endif


