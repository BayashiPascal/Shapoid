// ============ SHAPOID.H ================

#ifndef SHAPOID_H
#define SHAPOID_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pbmath.h"
#include "gset.h"
struct Shapoid;
typedef struct Shapoid Shapoid;
struct SCurve;
typedef struct SCurve SCurve;
#include "bcurve.h"

// ================= Define ==================

// ================= Generic functions ==================

void ShapoidGetBoundingBoxUnsupported(void*t, ...); 
#define ShapoidGetBoundingBox(T) _Generic((T), \
  Shapoid*: ShapoidGetBoundingBoxThat, \
  GSet*: ShapoidGetBoundingBoxSet, \
  default: ShapoidGetBoundingBoxUnsupported)(T)


// -------------- Shapoid

// ================= Define ==================

#define SpheroidCreate(D) ShapoidCreate(D, ShapoidTypeSpheroid)
#define FacoidCreate(D) ShapoidCreate(D, ShapoidTypeFacoid)
#define PyramidoidCreate(D) ShapoidCreate(D, ShapoidTypePyramidoid)

// ================= Data structure ===================

typedef enum ShapoidType {
  ShapoidTypeInvalid, ShapoidTypeFacoid, ShapoidTypeSpheroid,
  ShapoidTypePyramidoid
} ShapoidType;
// Don't forget to update ShapoidTypeString in pbmath.c when adding 
// new type

typedef struct Shapoid {
  // Position of origin
  VecFloat *_pos;
  // Dimension
  int _dim;
  // Vectors defining faces
  VecFloat **_axis;
  // Type of Shapoid
  ShapoidType _type;
  // Linear sytem used to import coordinates
  EqLinSys *_eqLinSysImport;
} Shapoid;

// ================ Functions declaration ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
// Return NULL if arguments are invalid or malloc failed
Shapoid* ShapoidCreate(int dim, ShapoidType type);

// Clone a Shapoid
// Return NULL if couldn't clone
Shapoid* ShapoidClone(Shapoid *that);

// Free memory used by a Shapoid
// Do nothing if arguments are invalid
void ShapoidFree(Shapoid **that);

// Load the Shapoid from the stream
// If the VecFloat is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
int ShapoidLoad(Shapoid **that, FILE *stream);

// Save the Shapoid to the stream
// Return 0 upon success, or
// 1: invalid arguments
// 2: fprintf error
int ShapoidSave(Shapoid *that, FILE *stream);

// Print the Shapoid on 'stream'
// Do nothing if arguments are invalid
void ShapoidPrint(Shapoid *that, FILE *stream);

// Get the dimension of the Shapoid
// Return 0 if arguments are invalid
int ShapoidGetDim(Shapoid *that);

// Get the type of the Shapoid
// Return ShapoidTypeInvalid if arguments are invalid
ShapoidType ShapoidGetType(Shapoid *that);

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
// Return the string for ShapoidTypeInvalid if arguments are invalid
const char* ShapoidGetTypeAsString(Shapoid *that);

// Return a VecFloat equal to the position of the Shapoid
// Return NULL if arguments are invalid
VecFloat* ShapoidGetPos(Shapoid *that);

// Return a VecFloat equal to the 'dim'-th axis of the Shapoid
// Return NULL if arguments are invalid
VecFloat* ShapoidGetAxis(Shapoid *that, int dim);

// Set the position of the Shapoid to 'pos'
// Do nothing if arguments are invalid
void ShapoidSetPos(Shapoid *that, VecFloat *pos);

// Set the 'dim'-th axis of the Shapoid to 'v'
// Do nothing if arguments are invalid
void ShapoidSetAxis(Shapoid *that, int dim, VecFloat *v);

// Translate the Shapoid by 'v'
// Do nothing if arguments are invalid
void ShapoidTranslate(Shapoid *that, VecFloat *v);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// Do nothing if arguments are invalid
void ShapoidScale(Shapoid *that, VecFloat *v);

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
// Do nothing if arguments are invalid
void ShapoidGrow(Shapoid *that, VecFloat *v);

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// Do nothing if arguments are invalid
void ShapoidRotate2D(Shapoid *that, float theta);

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
// Return null if the arguments are invalid
VecFloat* ShapoidImportCoord(Shapoid *that, VecFloat *pos);

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
// Return null if the arguments are invalid
VecFloat* ShapoidExportCoord(Shapoid *that, VecFloat *pos);

// Return true if 'pos' is inside the Shapoid
// Else return false
bool ShapoidIsPosInside(Shapoid *that, VecFloat *pos);

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
// Return null if the argument are invalid.
Shapoid* ShapoidGetBoundingBoxThat(Shapoid *that); 

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
// Return null if the arguments are invalid or the shapoid in the set
// don't have all the same dimension.
Shapoid* ShapoidGetBoundingBoxSet(GSet *set);

// Get a SCurve approximating the Shapoid 'that'
// 'that' must be of dimension 2
// Return null if arguments are invalid
SCurve* Shapoid2SCurve(Shapoid *that);

// Get the depth value in the Shapoid of 'pos'
// The depth is defined as follow: the point with depth equals 1.0 is 
// the farthest point from the surface of the Shapoid (inside it),
// points with depth equals to 0.0 are point on the surface of the
// Shapoid. Depth is continuous and derivable over the volume of the
// Shapoid
// Return 0.0 if arguments are invalid, or pos is outside the Shapoid
float ShapoidGetPosDepth(Shapoid *that, VecFloat *pos);

// Get the center of the shapoid in standard coordinate system
// Return null if arguments are invalid
VecFloat* ShapoidGetCenter(Shapoid *that);

// Get the percentage of 'tho' included 'that' (in [0.0, 1.0])
// 0.0 -> 'tho' is completely outside of 'that'
// 1.0 -> 'tho' is completely inside of 'that'
// 'that' and 'tho' must me of same dimensions
// Return 0.0 if the arguments are invalid or something went wrong
float ShapoidGetCoverage(Shapoid *that, Shapoid *tho);

#endif
