// ============ SHAPOID.C ================

// ================= Include =================

#include "shapoid.h"
#if BUILDMODE == 0
#include "shapoid-inline.c"
#endif

// ================= Define ==================

const char* ShapoidTypeString[3] = {
  (const char*)"Facoid", (const char*)"Spheroid", 
  (const char*)"Pyramidoid"};

// ================ Functions implementation ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
Shapoid* ShapoidCreate(int dim, ShapoidType type) {
#if BUILDMODE == 0
  if (dim <= 0) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Invalid dimension (%d>0)", dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a vector used for initialisation 
  VecShort2D d = VecShortCreateStatic2D();
  // Declare a identity matrix used for initialisation 
  VecSet(&d, 0, dim);
  VecSet(&d, 1, dim);
  MatFloat* mat = MatFloatCreate(&d);
  MatSetIdentity(mat);
  // Allocate memory
  Shapoid* that = PBErrMalloc(ShapoidErr, sizeof(Shapoid));
  // Init pointers
  that->_pos = NULL;
  that->_axis = NULL;
  that->_sysLinEqImport = NULL;
  // Set the dimension and type
  that->_type = type;
  that->_dim = dim;
  // Allocate memory for position
  that->_pos = VecFloatCreate(dim);
  // Allocate memory for array of axis
  that->_axis = PBErrMalloc(ShapoidErr, sizeof(VecFloat*) * dim);
  for (int iAxis = dim; iAxis--;)
    that->_axis[iAxis] = NULL;
  // Allocate memory for each axis
  for (int iAxis = 0; iAxis < dim; ++iAxis) {
    // Allocate memory for position
    that->_axis[iAxis] = VecFloatCreate(dim);
    // Set value of the axis
    VecSet(that->_axis[iAxis], iAxis, 1.0);
  }
  // Create the linear system for coordinate importation
  that->_sysLinEqImport = SysLinEqCreate(mat, (VecFloat*)NULL);
  // Free memory
  MatFree(&mat);
  // Return the new Shapoid
  return that;
}

// Clone a Shapoid
Shapoid* _ShapoidClone(Shapoid* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Create a clone
  Shapoid* clone = ShapoidCreate(that->_dim, that->_type);
  // Set the position and axis of the clone
  ShapoidSetPos(clone, that->_pos);
  for (int iAxis = clone->_dim; iAxis--;)
    ShapoidSetAxis(clone, iAxis, that->_axis[iAxis]);
  // Clone the SysLinEq
  SysLinEqFree(&(clone->_sysLinEqImport));
  clone->_sysLinEqImport = SysLinEqClone(that->_sysLinEqImport);
  // Return the clone
  return clone;
}

// Free memory used by a Shapoid
void _ShapoidFree(Shapoid** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  for (int iAxis = (*that)->_dim; iAxis--;)
    VecFree((*that)->_axis + iAxis);
  free((*that)->_axis);
  VecFree(&((*that)->_pos));
  SysLinEqFree(&((*that)->_sysLinEqImport));
  free(*that);
  *that = NULL;
}

// Load the Shapoid from the stream
// If the Shapoid is already allocated, it is freed before loading
// Return true upon success else false
bool _ShapoidLoad(Shapoid** that, FILE* stream, ShapoidType type) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (stream == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'stream' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    ShapoidFree(that);
  // Read the dimension and type
  int dim;
  int ret = fscanf(stream, "%d", &dim);
  // If we couldn't fscanf
  if (ret == EOF)
    return false;
  if (dim <= 0)
    return false;
  ShapoidType typeLoad;
  ret = fscanf(stream, "%u", &typeLoad);
  // If we coudln't fscanf
  if (ret == EOF)
    return false;
  // Check the type
  if (type != typeLoad)
    return false;
  // Allocate memory
  *that = ShapoidCreate(dim, type);
  // Read the values
  bool ok = VecLoad(&((*that)->_pos), stream);
  if (ok == false)
    return false;
  for (int iAxis = 0; iAxis < dim; ++iAxis) {
    ok = VecLoad((*that)->_axis + iAxis, stream);
    if (ok == false)
      return false;
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(*that);
  // Return success code
  return true;
}

// Save the Shapoid to the stream
// Return true upon success else false
bool _ShapoidSave(Shapoid* that, FILE* stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (stream == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'stream' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Save the dimension and type
  int ret = fprintf(stream, "%d %u\n", that->_dim, that->_type);
  // If we coudln't fprintf
  if (ret < 0)
    return false;
  // Save the position and axis
  bool ok = VecSave(that->_pos, stream);
  if (ok == false)
    return false;
  for (int iAxis = 0; iAxis < that->_dim; ++iAxis) {
    ok = VecSave(that->_axis[iAxis], stream);
    if (ok == false)
      return false;
  }
  // Return success code
  return true;
}

// Print the Shapoid on 'stream'
void _ShapoidPrintln(Shapoid* that, FILE* stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (stream == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'stream' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Print the Shapoid
  fprintf(stream, "Type: %s\n", ShapoidTypeString[that->_type]);
  fprintf(stream, "Dim: %d\n", that->_dim);
  fprintf(stream, "Pos: ");
  VecPrint(that->_pos, stream);
  fprintf(stream, "\n");
  for (int iAxis = 0; iAxis < that->_dim; ++iAxis) {
    fprintf(stream, "Axis(%d): ", iAxis);
    VecPrint(that->_axis[iAxis], stream);
    fprintf(stream, "\n");
  }
}

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* _ShapoidGetBoundingBox(Shapoid* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_type != ShapoidTypeFacoid &&
    that->_type != ShapoidTypeSpheroid &&
    that->_type != ShapoidTypePyramidoid) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "No implementation for 'that' 's type");
    PBErrCatch(ShapoidErr);
  }
#endif
  // If the Shapoid is a Facoid
  if (that->_type == ShapoidTypeFacoid) {
    return FacoidGetBoundingBox((Facoid*)that);
  // Else, if the Shapoid is a Pyramidoid
  } else  if (that->_type == ShapoidTypePyramidoid) {
    return PyramidoidGetBoundingBox((Pyramidoid*)that);
  // Else, if the Shapoid is a Spheroid
  } else  if (that->_type == ShapoidTypeSpheroid) {
    return SpheroidGetBoundingBox((Spheroid*)that);
  } else {
    return NULL;
  }
}

Facoid* FacoidGetBoundingBox(Facoid* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a variable to memorize the result
  Facoid* res = FacoidCreate(ShapoidGetDim(that));
  // For each axis
  for (int dim = ShapoidGetDim(that); dim--;) {
    // Declare a variable to memorize the bound of the interval on 
    // this axis
    float bound[2];
    bound[0] = bound[1] = VecGet(((Shapoid*)that)->_pos, dim);
    // For each parameter
    for (int param = ShapoidGetDim(that); param--;) {
      // Get the value of the axis influencing the current dimension
      float v = VecGet(((Shapoid*)that)->_axis[param], dim);
      // If the value is negative, update the minimum bound
      if (v < 0.0)
        bound[0] += v;
      // Else, if the value is negative, update the minimum bound
      else
        bound[1] += v;
    }
    // Memorize the result
    VecSet(((Shapoid*)res)->_pos, dim, bound[0]);
    VecSet(((Shapoid*)res)->_axis[dim], dim, bound[1] - bound[0]);
  }
  // Return the result
  return res;
}

Facoid* PyramidoidGetBoundingBox(Pyramidoid* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a variable to memorize the result
  Facoid* res = FacoidCreate(ShapoidGetDim(that));
  // For each axis
  for (int dim = ShapoidGetDim(that); dim--;) {
    // Declare a variable to memorize the bound of the interval on 
    // this axis
    float bound[2];
    bound[0] = bound[1] = VecGet(((Shapoid*)that)->_pos, dim);
    // For each parameter
    for (int param = ShapoidGetDim(that); param--;) {
      // Get the value of the axis influencing the current dimension
      float v = VecGet(((Shapoid*)that)->_axis[param], dim);
      // Search the min and max values
      if (v < bound[0])
        bound[0] = v;
      if (v > bound[1])
        bound[1] = v;
    }
    // Memorize the result
    VecSet(((Shapoid*)res)->_pos, dim, bound[0]);
    VecSet(((Shapoid*)res)->_axis[dim], dim, bound[1] - bound[0]);
  }
  // Return the result
  return res;
}

Facoid* SpheroidGetBoundingBox(Spheroid* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a variable to memorize the result
  Facoid* res = FacoidCreate(ShapoidGetDim(that));
  // In case of a Spheroid, things get complicate
  // We'll approximate the bounding box of the Spheroid 
  // with the one of the same Spheroid viewed as a Facoid
  // and simply take care that the _pos is at the center of the 
  // Spheroid
  // For each axis
  for (int dim = ShapoidGetDim(that); dim--;) {
    // Declare a variable to memorize the bound of the interval on 
    // this axis
    float bound[2];
    bound[0] = VecGet(((Shapoid*)that)->_pos, dim);
    // Correct position
    // For each parameter
    for (int param = ShapoidGetDim(that); param--;) {
      // Get the value of the axis influencing the current dimension
      float v = VecGet(((Shapoid*)that)->_axis[param], dim);
      // Correct the pos
      bound[0] -= 0.5 * v;
    }
    bound[1] = bound[0];
    // For each parameter
    for (int param = ShapoidGetDim(that); param--;) {
      // Get the value of the axis influencing the current dimension
      float v = VecGet(((Shapoid*)that)->_axis[param], dim);
      // If the value is negative, update the minimum bound
      if (v < 0.0)
        bound[0] += v;
      // Else, if the value is negative, update the minimum bound
      else
        bound[1] += v;
    }
    // Memorize the result
    VecSet(((Shapoid*)res)->_pos, dim, bound[0]);
    VecSet(((Shapoid*)res)->_axis[dim], dim, bound[1] - bound[0]);
  }
  // Return the result
  return res;
}

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
Facoid* ShapoidGetBoundingBoxSet(GSet* set) {
#if BUILDMODE == 0
  if (set == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'set' is null");
    PBErrCatch(ShapoidErr);
  }
  GSetElem* elemCheck = set->_head;
  int dim = ((Shapoid*)(elemCheck->_data))->_dim;
  while (elemCheck != NULL) {
    if (((Shapoid*)(elemCheck->_data))->_dim != dim) {
      ShapoidErr->_type = PBErrTypeInvalidArg;
      sprintf(ShapoidErr->_msg, 
        "'set' contains Shapoids of various dimensions");
      PBErrCatch(ShapoidErr);
    }
    elemCheck = elemCheck->_next;
  }
#endif
  // Declare a variable for the result
  Facoid* res = NULL;
  // Declare a pointer to the elements of the set
  GSetElem* elem = set->_head;
  // Loop on element of the set
  while (elem != NULL) {
    // Declare a pointer to the Facoid
    Shapoid* shapoid = (Shapoid*)(elem->_data);
    // If it's the first Facoid in the set
    if (res == NULL) {
      // Get the bounding box of this shapoid
      res = ShapoidGetBoundingBox(shapoid);
    // Else, this is not the first Shapoid in the set
    } else {
      // Get the bounding box of this shapoid
      Facoid* bound = ShapoidGetBoundingBox(shapoid);
      // For each dimension
      for (int iDim = ShapoidGetDim(res); iDim--;) {
        // Update the bounding box
        if (VecGet(((Shapoid*)bound)->_pos, iDim) < 
          VecGet(((Shapoid*)res)->_pos, iDim)) {
          VecSet(((Shapoid*)res)->_axis[iDim], iDim, 
            VecGet(((Shapoid*)res)->_axis[iDim], iDim) +
            VecGet(((Shapoid*)res)->_pos, iDim) -
            VecGet(((Shapoid*)bound)->_pos, iDim));
          VecSet(((Shapoid*)res)->_pos, iDim, 
          VecGet(((Shapoid*)bound)->_pos, iDim));
        }
        if (VecGet(((Shapoid*)bound)->_pos, iDim) + 
          VecGet(((Shapoid*)bound)->_axis[iDim], iDim) >
          VecGet(((Shapoid*)res)->_pos, iDim) + 
          VecGet(((Shapoid*)res)->_axis[iDim], iDim))
          VecSet(((Shapoid*)res)->_axis[iDim], iDim, 
          VecGet(((Shapoid*)bound)->_pos, iDim) + 
          VecGet(((Shapoid*)bound)->_axis[iDim], iDim) -
          VecGet(((Shapoid*)res)->_pos, iDim));
      }
      // Free memory used by the bounding box
      ShapoidFree(&bound);
    }
    // Move to the next element
    elem = elem->_next;
  }
  // Return the result
  return res;
}

// Get the percentage of 'tho' included 'that' (in [0.0, 1.0])
// 0.0 -> 'tho' is completely outside of 'that'
// 1.0 -> 'tho' is completely inside of 'that'
// 'that' and 'tho' must me of same dimensions
// delta is the step of the algorithm (in ]0.0, 1.0])
// small -> slow but precise
// big -> fast but rough
float _ShapoidGetCoverageDelta(Shapoid* that, Shapoid* tho, 
  float delta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (tho == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'tho' is null");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != ShapoidGetDim(tho)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "Shapoids dimensions are different (%d==%d)", 
      ShapoidGetDim(that), ShapoidGetDim(tho));
    PBErrCatch(ShapoidErr);
  }
  if (that->_type != ShapoidTypeFacoid &&
    that->_type != ShapoidTypeSpheroid &&
    that->_type != ShapoidTypePyramidoid) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "No implementation for 'that' 's type");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare variables to compute the result
  float ratio = 0.0;
  float sum = 0.0;
  // Declare variables for the relative and absolute position in 'tho'
  VecFloat* pRel = VecFloatCreate(ShapoidGetDim(that));
  VecFloat* pStd = NULL;
  // Declare a variable to memorize the last index in dimension
  int lastI = VecGetDim(pRel) - 1;
  // Declare a variable to memorize the max value of coordinates
  float max = 1.0;
  // If 'tho' is a spheroid, correct the start coordinates and range
  if (tho->_type == ShapoidTypeSpheroid) {
    max = 0.5;
    for (int iDim = ShapoidGetDim(that); iDim--;)
      VecSet(pRel, iDim, -0.5);
  }
  // Loop on relative coordinates
  while (VecGet(pRel, lastI) <= max + PBMATH_EPSILON) {
    // Get the coordinates in standard system
    pStd = ShapoidExportCoord(tho, pRel);
    // If this position is inside 'tho'
    if (ShapoidIsPosInside(tho, pStd) == true) {
      // If this position is inside 'that'
      if (ShapoidIsPosInside(that, pStd) == true)
        // Increment the ratio
        ratio += 1.0;
      sum += 1.0;
    }
    // Free memory
    VecFree(&pStd);
    // Step the relative coordinates
    int iDim = 0;
    while (iDim >= 0) {
      VecSet(pRel, iDim, VecGet(pRel, iDim) + delta);
      if (iDim != lastI && 
        VecGet(pRel, iDim) > max + PBMATH_EPSILON) {
        VecSet(pRel, iDim, max - 1.0);
        ++iDim;
      } else {
        iDim = -1;
      }
    }
  }
  // Finish the computation of the ratio
  ratio /= sum;
  // Free memory
  VecFree(&pRel);
  // Return the result
  return ratio;
}


