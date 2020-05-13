// ============ SHAPOID.C ================

// ================= Include =================

#include "shapoid.h"
#if BUILDMODE == 0
#include "shapoid-inline.c"
#endif

// -------------- Shapoid

// ================= Define ==================

const char* ShapoidTypeString[3] = {
  (const char*)"Facoid", (const char*)"Spheroid", 
  (const char*)"Pyramidoid"};

// ================ Functions implementation ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
Shapoid* ShapoidCreate(const int dim, const ShapoidType type) {
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
  Shapoid* that = NULL;
  if (type == ShapoidTypeSpheroid)
    that = PBErrMalloc(ShapoidErr, sizeof(Spheroid));
  else
    that = PBErrMalloc(ShapoidErr, sizeof(Shapoid));
  // Init pointers
  that->_pos = NULL;
  that->_axis = NULL;
  that->_sysLinEqImport = NULL;
  // Set the dimension and type
  *(ShapoidType*)&(that->_type) = type;
  *(int*)&(that->_dim) = dim;
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
  // Specific properties of Spheroid
  if (type == ShapoidTypeSpheroid) {
    ((Spheroid*)that)->_majAxis = 0;
    ((Spheroid*)that)->_minAxis = 0;
  }
  // Return the new Shapoid
  return that;
}

// Clone a Shapoid
Shapoid* _ShapoidClone(const Shapoid* const that) {
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
    VecCopy(clone->_axis[iAxis], that->_axis[iAxis]);
  ShapoidUpdateSysLinEqImport(clone);
  // Clone the SysLinEq
  SysLinEqFree(&(clone->_sysLinEqImport));
  clone->_sysLinEqImport = SysLinEqClone(that->_sysLinEqImport);
  // If it's a spheroid, copy the spheroid properties too
  if (that->_type == ShapoidTypeSpheroid) {
    ((Spheroid*)clone)->_majAxis = ((Spheroid*)that)->_majAxis;
    ((Spheroid*)clone)->_minAxis = ((Spheroid*)that)->_minAxis;
  }
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

// Function which return the JSON encoding of 'that' 
JSONNode* _ShapoidEncodeAsJSON(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // Create the JSON structure
  JSONNode* json = JSONCreate();
  // Declare a buffer to convert value into string
  char val[100];
  // Encode the dimension
  sprintf(val, "%d", that->_dim);
  JSONAddProp(json, "_dim", val);
  // Encode the type
  sprintf(val, "%u", that->_type);
  JSONAddProp(json, "_type", val);
  // Encode the position
  JSONAddProp(json, "_pos", VecEncodeAsJSON(ShapoidPos(that)));
  // Encode the axis
  JSONArrayStruct setAxis = JSONArrayStructCreateStatic();
  // For each axis
  for (int iAxis = 0; iAxis < that->_dim; ++iAxis)
    JSONArrayStructAdd(&setAxis, 
      VecEncodeAsJSON(ShapoidAxis(that, iAxis)));
  JSONAddProp(json, "_axis", &setAxis);
  // Free memory
  JSONArrayStructFlush(&setAxis);
  // Return the created JSON 
  return json;
}

// Function which decode from JSON encoding 'json' to 'that'
bool _ShapoidDecodeAsJSON(Shapoid** that, const JSONNode* const json) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
  if (json == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'json' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    ShapoidFree(that);
  // Get the dim from the JSON
  JSONNode* prop = JSONProperty(json, "_dim");
  if (prop == NULL) {
    return false;
  }
  int dim = atoi(JSONLblVal(prop));
  // Get the type from the JSON
  prop = JSONProperty(json, "_type");
  if (prop == NULL) {
    return false;
  }
  ShapoidType type = atoi(JSONLblVal(prop));
  // If data are invalid
  if (dim <= 0)
    return false;
  // Allocate memory
  *that = ShapoidCreate(dim, type);
  // Get the position from the JSON
  prop = JSONProperty(json, "_pos");
  if (prop == NULL) {
    return false;
  }
  if (!VecDecodeAsJSON(&((*that)->_pos), prop)) {
    return false;
  }
  // Decode the axis
  prop = JSONProperty(json, "_axis");
  if (prop == NULL) {
    return false;
  }
  if (JSONGetNbValue(prop) != dim) {
    return false;
  }
  for (int iAxis = 0; iAxis < dim; ++iAxis) {
    JSONNode* axis = JSONValue(prop, iAxis);
    if (!VecDecodeAsJSON((*that)->_axis + iAxis, axis))
      return false;
    // If the axis is not of the correct dimension
    if (VecGetDim((*that)->_axis[iAxis]) != (*that)->_dim)
      return false;
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(*that);
  // If it's a Spheroid
  if ((*that)->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)*that);
  // Return the success code
  return true;
}

// Load the Shapoid from the stream
// If the Shapoid is already allocated, it is freed before loading
// Return true upon success else false
bool _ShapoidLoad(Shapoid** that, FILE* const stream) {
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
  // Declare a json to load the encoded data
  JSONNode* json = JSONCreate();
  // Load the whole encoded data
  if (!JSONLoad(json, stream)) {
    return false;
  }
  // Decode the data from the JSON
  if (!ShapoidDecodeAsJSON(that, json)) {
    return false;
  }
  // Free the memory used by the JSON
  JSONFree(&json);
  // Return success code
  return true;
}

// Save the Shapoid to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success else false
bool _ShapoidSave(const Shapoid* const that, FILE* const stream, 
  const bool compact) {
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
  // Get the JSON encoding
  JSONNode* json = ShapoidEncodeAsJSON(that);
  // Save the JSON
  if (!JSONSave(json, stream, compact)) {
    return false;
  }
  // Free memory
  JSONFree(&json);
  // Return success code
  return true;
}

// Print the Shapoid on 'stream'
void _ShapoidPrintln(const Shapoid* const that, FILE* const stream) {
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
Facoid* _ShapoidGetBoundingBox(const Shapoid* const that) {
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

Facoid* FacoidGetBoundingBox(const Facoid* const that) {
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

Facoid* PyramidoidGetBoundingBox(const Pyramidoid* const that) {
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
    bound[0] = bound[1] = 0.0;
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
    if (bound[0] > 0.0)
      bound[0] = 0.0;
    if (bound[1] < 0.0)
      bound[1] = 0.0;
    // Memorize the result
    VecSet(((Shapoid*)res)->_pos, dim, 
      ShapoidPosGet(that, dim) + bound[0]);
    VecSet(((Shapoid*)res)->_axis[dim], dim, bound[1] - bound[0]);
  }
  // Return the result
  return res;
}

Facoid* SpheroidGetBoundingBox(const Spheroid* const that) {
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
Facoid* ShapoidGetBoundingBoxSet(const GSetShapoid* const set) {
#if BUILDMODE == 0
  if (set == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'set' is null");
    PBErrCatch(ShapoidErr);
  }
  if (GSetNbElem(set) > 0) {
    GSetIterForward iter = GSetIterForwardCreateStatic(set);
    int dim = ((Shapoid*)GSetIterGet(&iter))->_dim;
    do {
      if (((Shapoid*)GSetIterGet(&iter))->_dim != dim) {
        ShapoidErr->_type = PBErrTypeInvalidArg;
        sprintf(ShapoidErr->_msg, 
          "'set' contains Shapoids of various dimensions");
        PBErrCatch(ShapoidErr);
      }
    } while (GSetIterStep(&iter));
  }
#endif
  // Declare a variable for the result
  Facoid* res = NULL;
  if (GSetNbElem(set) > 0) {
    // Declare an interator on the elements of the set
    GSetIterForward iter = GSetIterForwardCreateStatic(set);
    // Loop on element of the set
    do {
      // Declare a pointer to the Facoid
      Shapoid* shapoid = GSetIterGet(&iter);
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
            VecSetAdd(((Shapoid*)res)->_axis[iDim], iDim, 
              VecGet(((Shapoid*)res)->_pos, iDim) -
              VecGet(((Shapoid*)bound)->_pos, iDim));
            VecSet(((Shapoid*)res)->_pos, iDim, 
            VecGet(((Shapoid*)bound)->_pos, iDim));
          }
          if (VecGet(((Shapoid*)bound)->_pos, iDim) + 
            VecGet(((Shapoid*)bound)->_axis[iDim], iDim) >
            VecGet(((Shapoid*)res)->_pos, iDim) + 
            VecGet(((Shapoid*)res)->_axis[iDim], iDim))
            VecSetAdd(((Shapoid*)res)->_axis[iDim], iDim, 
              VecGet(((Shapoid*)bound)->_axis[iDim], iDim) -
              VecGet(((Shapoid*)res)->_pos, iDim));
        }
        // Free memory used by the bounding box
        ShapoidFree(&bound);
      }
    } while (GSetIterStep(&iter));
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
float _ShapoidGetCoverageDelta(const Shapoid* const that, 
  const Shapoid* const tho, const float delta) {
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
  long lastI = VecGetDim(pRel) - 1;
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
    long iDim = 0;
    while (iDim >= 0) {
      VecSetAdd(pRel, iDim, delta);
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

// Add a copy of the Facoid 'that' to the GSet 'set' (containing 
// other Facoid), taking care to avoid overlaping Facoid
// The copy of 'that' made be resized or divided
// The Facoid in the set and 'that' must be aligned with the 
// coordinates system axis and have 
// same dimensions
void FacoidAlignedAddClippedToSet(const Facoid* const that, 
  GSetShapoid* const set) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (set == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'set' is null");
    PBErrCatch(ShapoidErr);
  }
#endif  
  // If the set is empty 
  if (GSetNbElem(set) == 0) {
    // Add a clone of the facoid
    GSetAppend(set, FacoidClone(that));
  // Else, the set is not empty
  } else {
    // Create a set of sub facoid to be added and initialize it with a
    // clone of 'that'
    GSetShapoid setToAdd = GSetShapoidCreateStatic();
    GSetAppend(&setToAdd, FacoidClone(that));
    // For each sub facoid to add
    GSetIterForward iterToAdd = GSetIterForwardCreateStatic(&setToAdd);
    do {
      // Get the current facoid to add
      Facoid* facoidToAdd = GSetIterGet(&iterToAdd);
      // Declare a flag to skip the loop when possible
      bool flagSkip = false;
      // For each facoid in the set
      GSetIterForward iter = GSetIterForwardCreateStatic(set);
      do {
        // Get the current facoid
        Facoid* facoid = GSetIterGet(&iter);
        // If the facoid to be added is completely included into this 
        // facoid
        if (FacoidAlignedIsInsideFacoidAligned(facoidToAdd, facoid)) {
          // This facoid doesn't need to be added, delete it
          ShapoidFree(&facoidToAdd);
          GSetIterSetData(&iterToAdd, NULL);
          // And skip the other facoids in the set
          flagSkip = true;
        // Else, if this facoid is completely include in the facoid to 
        // be added
        } else if (FacoidAlignedIsInsideFacoidAligned(facoid, 
          facoidToAdd)) {
          // Remove the facoid in the set
          ShapoidFree(&facoid);
          GSetIterSetData(&iter, NULL);
        // Else, if both facoid are in intersection
        } else if (!FacoidAlignedIsOutsideFacoidAligned(facoidToAdd, 
          facoid)) {
          // Split the facoid to be added into new facoids
          // which cover the non intersecting area
          GSetShapoid* split = 
            FacoidAlignedSplitExcludingFacoidAligned(facoidToAdd, 
              facoid);
          GSetAppendSet(&setToAdd, split);
          GSetFree(&split);
          // Delete the splitted facoid
          ShapoidFree(&facoidToAdd);
          GSetIterSetData(&iterToAdd, NULL);
          // And skip the other facoids in the set
          flagSkip = true;          
        }
        // Else the facoid to add is completely outside, leave it as 
        // it is
      } while (!flagSkip && GSetIterStep(&iter));
    } while (GSetIterStep(&iterToAdd));
    // When we arrive here the set 'setToAdd' contains the facoids
    // to be added to 'set'
    GSetAppendSet(set, &setToAdd);
    // There may have been deleted facoid, ensure the resulting set
    // is clean by removing null pointer
    GSetRemoveAll(set, (Shapoid*)NULL);
    // Free memory used by the set of sub facoid to add
    GSetFlush(&setToAdd);
  }
}

// Check if the Facoid 'that' is completely included into the Facoid 
// 'facoid'
// Both Facoid must be aligned with the coordinates system
// 'that' and 'facoid' must have same dimensions and have 
// same dimensions
// Return true if it is included, false else
bool FacoidAlignedIsInsideFacoidAligned(const Facoid* const that, 
  const Facoid* const facoid) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (facoid == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'facoid' is null");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != ShapoidGetDim(facoid)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'facoid' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(facoid));
    PBErrCatch(ShapoidErr);
  }
#endif  
  // Check inclusion for each axis
  for (int iAxis = ShapoidGetDim(that); iAxis--;)
    // If 'that' is outside 'facoid' for this axis
    if (ShapoidPosGet(that, iAxis) < 
      ShapoidPosGet(facoid, iAxis) ||
      ShapoidPosGet(that, iAxis) + 
      ShapoidAxisGet(that, iAxis, iAxis) > 
      ShapoidPosGet(facoid, iAxis) +
      ShapoidAxisGet(facoid, iAxis, iAxis))
      // Return false
      return false;
  // If we reach here it means 'that' is inside 'facoid', return true
  return true;
}

// Check if the Facoid 'that' is completely excluded from the Facoid 
// 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
// Return true if it is excluded, false else
bool FacoidAlignedIsOutsideFacoidAligned(const Facoid* const that, 
  const Facoid* const facoid) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (facoid == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'facoid' is null");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != ShapoidGetDim(facoid)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'facoid' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(facoid));
    PBErrCatch(ShapoidErr);
  }
#endif  
  // Check exclusion for each axis
  for (int iAxis = ShapoidGetDim(that); iAxis--;)
    // If 'that' is outside 'facoid' for this axis
    if (ShapoidPosGet(that, iAxis) > 
      ShapoidPosGet(facoid, iAxis) +
      ShapoidAxisGet(facoid, iAxis, iAxis) - PBMATH_EPSILON ||
      ShapoidPosGet(that, iAxis) + 
      ShapoidAxisGet(that, iAxis, iAxis) < 
      ShapoidPosGet(facoid, iAxis) + PBMATH_EPSILON)
      // Return true
      return true;
  // If we reach here it means 'that' intersects 'facoid', return false
  return false;
}

// Get a GSet of Facoid aligned with coordinates system covering the 
// Facoid 'that' except for area in the Facoid 'facoid'
// Both Facoid must be aligned with the coordinates system and have 
// same dimensions
GSetShapoid* FacoidAlignedSplitExcludingFacoidAligned(
  const Facoid* const that, const Facoid* const facoid) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (facoid == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'facoid' is null");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != ShapoidGetDim(facoid)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'facoid' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(facoid));
    PBErrCatch(ShapoidErr);
  }
#endif  
  // Ladies and Gentleman, here comes the infamous "Gruyere Algorithm"
  // Declare the result GSet
  GSetShapoid* set = GSetShapoidCreate();
  // Declare a clone of the original facoid
  Facoid* src = FacoidClone(that);
  // For each axis
  for (int iAxis = ShapoidGetDim(that); iAxis--;) {
    // If 'src' has area on the left of 'facoid' along this axis
    if (ShapoidPosGet(src, iAxis) < ShapoidPosGet(facoid, iAxis)) {
      // Create the facoid made of this area
      Facoid* sub = FacoidClone(src);
      ShapoidAxisSet(sub, iAxis, iAxis, 
        ShapoidPosGet(facoid, iAxis) - ShapoidPosGet(src, iAxis));
      // Add it to the result set
      GSetAppend(set, sub);
      // Chop the added area from 'src'
      ShapoidAxisSetAdd(src, iAxis, iAxis,
        -1.0 * ShapoidAxisGet(sub, iAxis, iAxis));
      ShapoidPosSet(src, iAxis, ShapoidPosGet(facoid, iAxis));
    }
    // If 'src' has area on the right of 'facoid' along this axis
    if (ShapoidPosGet(src, iAxis) + ShapoidAxisGet(src, iAxis, iAxis) > 
      ShapoidPosGet(facoid, iAxis) + 
      ShapoidAxisGet(facoid, iAxis, iAxis)) {
      // Create the facoid made of this area
      Facoid* sub = FacoidClone(src);
      ShapoidAxisSet(sub, iAxis, iAxis, 
        (ShapoidPosGet(src, iAxis) + 
        ShapoidAxisGet(src, iAxis, iAxis)) - 
        (ShapoidPosGet(facoid, iAxis) +
        ShapoidAxisGet(facoid, iAxis, iAxis)));
      ShapoidPosSet(sub, iAxis, ShapoidPosGet(facoid, iAxis) +
        ShapoidAxisGet(facoid, iAxis, iAxis));
      // Add it to the result set
      GSetAppend(set, sub);
      // Chop the added area from 'src'
      ShapoidAxisSetAdd(src, iAxis, iAxis,
        -1.0 * ShapoidAxisGet(sub, iAxis, iAxis));
    }
    // If 'src' is empty
    if (ISEQUALF(ShapoidAxisGet(src, iAxis, iAxis), 0.0))
      // End the loop
      iAxis = 0;
  }
  // Free memory
  ShapoidFree(&src);
  // Return the result set
  return set;
}

// Return true if 'that' intersects 'tho'
// Return false else
// 'that' and 'tho' must have same dimension
// https://hal.inria.fr/hal-00646511/PDF/CCD.3.0.pdf
bool _SpheroidIsInterSpheroid(const Spheroid* const that, 
  const Spheroid* const tho) {
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
      "'that' and 'tho' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(tho));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Create the projection of 'tho' in 'that' 's coordinates space
  Spheroid* proj = SpheroidCreate(ShapoidGetDim(tho));
  VecFloat* v = ShapoidImportCoord(that, ShapoidPos(tho));
  ShapoidSetPos(proj, v);
  VecFree(&v);
  for (int iAxis = ShapoidGetDim(tho); iAxis--;) {
    v = ShapoidImportCoord(that, ShapoidAxis(tho, iAxis));
    ShapoidSetAxis(proj, iAxis, v);
    VecFree(&v);
  }
  // Declare a variable to memorize the distance to the origin of
  // 'that' 's coordinate system
  float dist = VecNorm(ShapoidPos(proj));
  // Check for trivial cases
  float majRadius = 0.5 * VecNorm(ShapoidAxis(proj, proj->_majAxis));
  if (dist > majRadius + 0.5) {
    ShapoidFree(&proj);
    VecFree(&v);
    return false;
  } else if (proj->_majAxis == proj->_minAxis) {
    ShapoidFree(&proj);
    VecFree(&v);
    return true;
  }
  float minRadius = 0.5 * VecNorm(ShapoidAxis(proj, proj->_minAxis));
  if (dist <= minRadius + 0.5) {
    ShapoidFree(&proj);
    VecFree(&v);
    return true;
  }
  // Non trivial case
  // Search a position in the projection of 'tho' less than 1.0 units 
  // from the origin in 'that' 's coordinates space
  // Declare a variable to move in the projection's coordinates space
  VecFloat* pos = VecFloatCreate(ShapoidGetDim(tho));
  // Declare a variable to memorize the derivative
  VecFloat* dPos = VecFloatCreate(ShapoidGetDim(tho));
  // Declare a variable to memorize the step for derivate calculation
  float delta = 0.01;
  // Declare a flag to stop the loop in case of deadlock
  bool flag = false;
  // Loop until we find a solution or deadlock
  while (dist > 0.5 && !flag) {
    // Calculate the derivative along each axis
    v = VecFloatCreate(VecGetDim(pos));
    for (int iAxis = ShapoidGetDim(tho); iAxis--;) {
      // Copy the current position
      VecCopy(v, pos);
      // Move a delta along the current axis
      VecSetAdd(v, iAxis, delta);
      // Get the cooridnate in 'that' 's coordinates system
      VecFloat* w = ShapoidExportCoord(proj, v);
      // Calculate the distance ot origin of 'that' 's coordinates 
      // system
      float dp = VecNorm(w);
      // Free memory
      VecFree(&w);
      // Do the same thing with minus delta
      VecSetAdd(v, iAxis, -2.0 * delta);
      w = ShapoidExportCoord(proj, v);
      float dm = VecNorm(w);
      VecFree(&w);
      // Calculate the derivative along the current axis
      VecSet(dPos, iAxis, (dp - dm) / (2.0 * delta));
    }
    // Free memory
    VecFree(&v);
    // Move toward better solution
    // Declare a variable to memorize the next position
    VecFloat* nPos = VecGetOp(pos, 1.0, dPos, -1.0);
    // Ensure the position stay inside the Spheroid
    if (VecNorm(nPos) > 0.5) {
      VecNormalise(nPos);
      VecScale(nPos, 0.5);
    }
    // If we are stuck to the same position
    if (VecDist(pos, nPos) < PBMATH_EPSILON)
      // Stop the loop
      flag = true;
    // Else we keep moving
    else {
      VecCopy(pos, nPos);
      // Update the current distance
      v = ShapoidExportCoord(proj, pos);
      dist = VecNorm(v);
      VecFree(&v);
    }
    // Free memory
    VecFree(&nPos);
  }
  // Free memory
  ShapoidFree(&proj);
  VecFree(&pos);
  VecFree(&dPos);
  // If we have found a position less than one unit from the origin
  // of 'that' 's coordinates system
  if (dist <= 0.5)
    // The spheroids intersect
    return true;
  else
    // The spheroids do not intersect
    return false;
}

// SAT algorithm on 2D Facoid-Facoid
bool SATFF(const Facoid* const that, 
  const Facoid* const tho) {
  // Declare a variable to loop on Frames and commonalize code
  const Shapoid* frameEdge = (const Shapoid*)that;
  // Loop to commonalize code when checking SAT based on that's edges
  // and then tho's edges
  for (
    int iFrame = 2;
    iFrame--;) {
    // Declare a variable to memorize the number of edges, by default 2
    int nbEdges = 2;
    // Loop on the frame's edges
    for (
      int iEdge = nbEdges;
      iEdge--;) {
      // Get the current edge
      const float* edge = frameEdge->_axis[iEdge]->_val;
      // Declare variables to memorize the boundaries of projection
      // of the two frames on the current edge
      float bdgBoxA[2];
      float bdgBoxB[2];
      // Declare two variables to loop on Frames and commonalize code
      const Shapoid* frame = (const Shapoid*)that;
      float* bdgBox = bdgBoxA;
      // Loop on Frames
      for (
        int jFrame = 2;
        jFrame--;) {
        // Shortcuts
        const float* frameOrig = frame->_pos->_val;
        const float* frameCompA = frame->_axis[0]->_val;
        const float* frameCompB = frame->_axis[1]->_val;
        // Get the number of vertices of frame
        int nbVertices = 4;
        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;
        // Loop on vertices of the frame
        for (
          int iVertex = nbVertices;
          iVertex--;) {
          // Get the vertex
          float vertex[2];
          vertex[0] = frameOrig[0];
          vertex[1] = frameOrig[1];
          switch (iVertex) {
            case 3:
              vertex[0] += frameCompA[0] + frameCompB[0];
              vertex[1] += frameCompA[1] + frameCompB[1];
              break;
            case 2:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              break;
            case 1:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              break;
            default:
              break;
          }
          // Get the projection of the vertex on the normal of the edge
          // Orientation of the normal doesn't matter, so we
          // use arbitrarily the normal (edge[1], -edge[0])
          float proj = vertex[0] * edge[1] - vertex[1] * edge[0];
          // If it's the first vertex
          if (firstVertex == true) {
            // Initialize the boundaries of the projection of the
            // Frame on the edge
            bdgBox[0] = proj;
            bdgBox[1] = proj;
            // Update the flag to memorize we did the first vertex
            firstVertex = false;
          // Else, it's not the first vertex
          } else {
            // Update the boundaries of the projection of the Frame on
            // the edge
            if (bdgBox[0] > proj) {
              bdgBox[0] = proj;
            }
            if (bdgBox[1] < proj) {
              bdgBox[1] = proj;
            }
          }
        }
        // Switch the frame to check the vertices of the second Frame
        frame = (const Shapoid*)tho;
        bdgBox = bdgBoxB;
      }
      // If the projections of the two frames on the edge are
      // not intersecting
      if (
        bdgBoxB[1] < bdgBoxA[0] ||
        bdgBoxA[1] < bdgBoxB[0]) {
        // There exists an axis which separates the Frames,
        // thus they are not in intersection
        return false;
      }
    }
    // Switch the frames to test against the second Frame's edges
    frameEdge = (const Shapoid*)tho;
  }
  // If we reaches here, it means the two Frames are intersecting
  return true;
}

// SAT algorithm on 2D Facoid-Pyramidoid
bool SATFP(const Facoid* const that, 
  const Pyramidoid* const tho) {
  // Declare a variable to loop on Frames and commonalize code
  const Shapoid* frameEdge = (const Shapoid*)that;
  // Loop to commonalize code when checking SAT based on that's edges
  // and then tho's edges
  for (
    int iFrame = 2;
    iFrame--;) {
    // Shortcuts
    const float* frameEdgeCompA = frameEdge->_axis[0]->_val;
    const float* frameEdgeCompB = frameEdge->_axis[1]->_val;
    // Declare a variable to memorize the number of edges, by default 2
    int nbEdges = 2;
    // Declare a variable to memorize the third edge in case of
    // tetrahedron
    float thirdEdge[2];
    // If the frame is a tetrahedron
    if (iFrame == 0) {
      // Initialise the third edge
      thirdEdge[0] = frameEdgeCompB[0] - frameEdgeCompA[0];
      thirdEdge[1] = frameEdgeCompB[1] - frameEdgeCompA[1];
      // Correct the number of edges
      nbEdges = 3;
    }
    // Loop on the frame's edges
    for (
      int iEdge = nbEdges;
      iEdge--;) {
      // Get the current edge
      const float* edge =
        (iEdge == 2 ? thirdEdge : frameEdge->_axis[iEdge]->_val);
      // Declare variables to memorize the boundaries of projection
      // of the two frames on the current edge
      float bdgBoxA[2];
      float bdgBoxB[2];
      // Declare two variables to loop on Frames and commonalize code
      const Shapoid* frame = (const Shapoid*)that;
      float* bdgBox = bdgBoxA;
      // Loop on Frames
      for (
        int jFrame = 2;
        jFrame--;) {
        // Shortcuts
        const float* frameOrig = frame->_pos->_val;
        const float* frameCompA = frame->_axis[0]->_val;
        const float* frameCompB = frame->_axis[1]->_val;
        // Get the number of vertices of frame
        int nbVertices = (jFrame == 0 ? 3 : 4);
        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;
        // Loop on vertices of the frame
        for (
          int iVertex = nbVertices;
          iVertex--;) {
          // Get the vertex
          float vertex[2];
          vertex[0] = frameOrig[0];
          vertex[1] = frameOrig[1];
          switch (iVertex) {
            case 3:
              vertex[0] += frameCompA[0] + frameCompB[0];
              vertex[1] += frameCompA[1] + frameCompB[1];
              break;
            case 2:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              break;
            case 1:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              break;
            default:
              break;
          }
          // Get the projection of the vertex on the normal of the edge
          // Orientation of the normal doesn't matter, so we
          // use arbitrarily the normal (edge[1], -edge[0])
          float proj = vertex[0] * edge[1] - vertex[1] * edge[0];
          // If it's the first vertex
          if (firstVertex == true) {
            // Initialize the boundaries of the projection of the
            // Frame on the edge
            bdgBox[0] = proj;
            bdgBox[1] = proj;
            // Update the flag to memorize we did the first vertex
            firstVertex = false;
          // Else, it's not the first vertex
          } else {
            // Update the boundaries of the projection of the Frame on
            // the edge
            if (bdgBox[0] > proj) {
              bdgBox[0] = proj;
            }
            if (bdgBox[1] < proj) {
              bdgBox[1] = proj;
            }
          }
        }
        // Switch the frame to check the vertices of the second Frame
        frame = (const Shapoid*)tho;
        bdgBox = bdgBoxB;
      }
      // If the projections of the two frames on the edge are
      // not intersecting
      if (
        bdgBoxB[1] < bdgBoxA[0] ||
        bdgBoxA[1] < bdgBoxB[0]) {
        // There exists an axis which separates the Frames,
        // thus they are not in intersection
        return false;
      }
    }
    // Switch the frames to test against the second Frame's edges
    frameEdge = (const Shapoid*)tho;
  }
  // If we reaches here, it means the two Frames are intersecting
  return true;
}

// SAT algorithm on 2D Pyramidoid-Facoid
bool SATPF(const Pyramidoid* const that, 
  const Facoid* const tho) {
  // Declare a variable to loop on Frames and commonalize code
  const Shapoid* frameEdge = (const Shapoid*)that;
  // Loop to commonalize code when checking SAT based on that's edges
  // and then tho's edges
  for (
    int iFrame = 2;
    iFrame--;) {
    // Shortcuts
    const float* frameEdgeCompA = frameEdge->_axis[0]->_val;
    const float* frameEdgeCompB = frameEdge->_axis[1]->_val;
    // Declare a variable to memorize the number of edges, by default 2
    int nbEdges = 2;
    // Declare a variable to memorize the third edge in case of
    // tetrahedron
    float thirdEdge[2];
    // If the frame is a tetrahedron
    // Initialise the third edge
    thirdEdge[0] = frameEdgeCompB[0] - frameEdgeCompA[0];
    thirdEdge[1] = frameEdgeCompB[1] - frameEdgeCompA[1];
    // If the frame is a tetrahedron
    if (iFrame == 1) {
      // Initialise the third edge
      thirdEdge[0] = frameEdgeCompB[0] - frameEdgeCompA[0];
      thirdEdge[1] = frameEdgeCompB[1] - frameEdgeCompA[1];
      // Correct the number of edges
      nbEdges = 3;
    }
    // Loop on the frame's edges
    for (
      int iEdge = nbEdges;
      iEdge--;) {
      // Get the current edge
      const float* edge =
        (iEdge == 2 ? thirdEdge : frameEdge->_axis[iEdge]->_val);
      // Declare variables to memorize the boundaries of projection
      // of the two frames on the current edge
      float bdgBoxA[2];
      float bdgBoxB[2];
      // Declare two variables to loop on Frames and commonalize code
      const Shapoid* frame = (const Shapoid*)that;
      float* bdgBox = bdgBoxA;
      // Loop on Frames
      for (
        int jFrame = 2;
        jFrame--;) {
        // Shortcuts
        const float* frameOrig = frame->_pos->_val;
        const float* frameCompA = frame->_axis[0]->_val;
        const float* frameCompB = frame->_axis[1]->_val;
        // Get the number of vertices of frame
        int nbVertices = (jFrame == 1 ? 3 : 4);
        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;
        // Loop on vertices of the frame
        for (
          int iVertex = nbVertices;
          iVertex--;) {
          // Get the vertex
          float vertex[2];
          vertex[0] = frameOrig[0];
          vertex[1] = frameOrig[1];
          switch (iVertex) {
            case 3:
              vertex[0] += frameCompA[0] + frameCompB[0];
              vertex[1] += frameCompA[1] + frameCompB[1];
              break;
            case 2:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              break;
            case 1:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              break;
            default:
              break;
          }
          // Get the projection of the vertex on the normal of the edge
          // Orientation of the normal doesn't matter, so we
          // use arbitrarily the normal (edge[1], -edge[0])
          float proj = vertex[0] * edge[1] - vertex[1] * edge[0];
          // If it's the first vertex
          if (firstVertex == true) {
            // Initialize the boundaries of the projection of the
            // Frame on the edge
            bdgBox[0] = proj;
            bdgBox[1] = proj;
            // Update the flag to memorize we did the first vertex
            firstVertex = false;
          // Else, it's not the first vertex
          } else {
            // Update the boundaries of the projection of the Frame on
            // the edge
            if (bdgBox[0] > proj) {
              bdgBox[0] = proj;
            }
            if (bdgBox[1] < proj) {
              bdgBox[1] = proj;
            }
          }
        }
        // Switch the frame to check the vertices of the second Frame
        frame = (const Shapoid*)tho;
        bdgBox = bdgBoxB;
      }
      // If the projections of the two frames on the edge are
      // not intersecting
      if (
        bdgBoxB[1] < bdgBoxA[0] ||
        bdgBoxA[1] < bdgBoxB[0]) {
        // There exists an axis which separates the Frames,
        // thus they are not in intersection
        return false;
      }
    }
    // Switch the frames to test against the second Frame's edges
    frameEdge = (const Shapoid*)tho;
  }
  // If we reaches here, it means the two Frames are intersecting
  return true;
}

// SAT algorithm on 2D Pyramidoid-Pyramidoid
bool SATPP(const Pyramidoid* const that, 
  const Pyramidoid* const tho) {
  // Declare a variable to loop on Frames and commonalize code
  const Shapoid* frameEdge = (const Shapoid*)that;
  // Loop to commonalize code when checking SAT based on that's edges
  // and then tho's edges
  for (
    int iFrame = 2;
    iFrame--;) {
    // Shortcuts
    const float* frameEdgeCompA = frameEdge->_axis[0]->_val;
    const float* frameEdgeCompB = frameEdge->_axis[1]->_val;
    // Declare a variable to memorize the number of edges, by default 2
    int nbEdges = 3;
    // Declare a variable to memorize the third edge in case of
    // tetrahedron
    float thirdEdge[2];
    // If the frame is a tetrahedron
    // Initialise the third edge
    thirdEdge[0] = frameEdgeCompB[0] - frameEdgeCompA[0];
    thirdEdge[1] = frameEdgeCompB[1] - frameEdgeCompA[1];
    // Loop on the frame's edges
    for (
      int iEdge = nbEdges;
      iEdge--;) {
      // Get the current edge
      const float* edge =
        (iEdge == 2 ? thirdEdge : frameEdge->_axis[iEdge]->_val);
      // Declare variables to memorize the boundaries of projection
      // of the two frames on the current edge
      float bdgBoxA[2];
      float bdgBoxB[2];
      // Declare two variables to loop on Frames and commonalize code
      const Shapoid* frame = (const Shapoid*)that;
      float* bdgBox = bdgBoxA;
      // Loop on Frames
      for (
        int iFrame = 2;
        iFrame--;) {
        // Shortcuts
        const float* frameOrig = frame->_pos->_val;
        const float* frameCompA = frame->_axis[0]->_val;
        const float* frameCompB = frame->_axis[1]->_val;
        // Get the number of vertices of frame
        int nbVertices = 3;
        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;
        // Loop on vertices of the frame
        for (
          int iVertex = nbVertices;
          iVertex--;) {
          // Get the vertex
          float vertex[2];
          vertex[0] = frameOrig[0];
          vertex[1] = frameOrig[1];
          switch (iVertex) {
            case 2:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              break;
            case 1:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              break;
            default:
              break;
          }
          // Get the projection of the vertex on the normal of the edge
          // Orientation of the normal doesn't matter, so we
          // use arbitrarily the normal (edge[1], -edge[0])
          float proj = vertex[0] * edge[1] - vertex[1] * edge[0];
          // If it's the first vertex
          if (firstVertex == true) {
            // Initialize the boundaries of the projection of the
            // Frame on the edge
            bdgBox[0] = proj;
            bdgBox[1] = proj;
            // Update the flag to memorize we did the first vertex
            firstVertex = false;
          // Else, it's not the first vertex
          } else {
            // Update the boundaries of the projection of the Frame on
            // the edge
            if (bdgBox[0] > proj) {
              bdgBox[0] = proj;
            }
            if (bdgBox[1] < proj) {
              bdgBox[1] = proj;
            }
          }
        }
        // Switch the frame to check the vertices of the second Frame
        frame = (const Shapoid*)tho;
        bdgBox = bdgBoxB;
      }
      // If the projections of the two frames on the edge are
      // not intersecting
      if (
        bdgBoxB[1] < bdgBoxA[0] ||
        bdgBoxA[1] < bdgBoxB[0]) {
        // There exists an axis which separates the Frames,
        // thus they are not in intersection
        return false;
      }
    }
    // Switch the frames to test against the second Frame's edges
    frameEdge = (const Shapoid*)tho;
  }
  // If we reaches here, it means the two Frames are intersecting
  return true;
}

// Eliminate the first variable in the system M.X<=Y
// using the Fourier-Motzkin method and return
// the resulting system in Mp and Yp, and the number of rows of
// the resulting system in nbRemainRows
// (M arrangement is [iRow][iCol])
// Return true if the system becomes inconsistent during elimination,
// else return false
#define neg(x) (x < 0.0 ? x : 0.0)
#define sgn(v) (((0.0 < (v)) ? 1 : 0) - (((v) < 0.0) ? 1 : 0))
bool ElimVar3D(
  const float (*M)[3],
  const float* Y,
  const int nbRows,
  const int nbCols,
  float (*Mp)[3],
  float* Yp,
  int* const nbRemainRows) {
  // Initialize the number of rows in the result system
  int nbResRows = 0;
  // First we process the rows where the eliminated variable is not null
  // For each row except the last one
  for (
    int iRow = 0;
    iRow < nbRows - 1;
    ++iRow) {
    // Shortcuts
    const float fabsMIRowIVar = fabs(M[iRow][0]);
    // If the coefficient for the eliminated variable is not null
    // in this row
    if (fabsMIRowIVar > PBMATH_EPSILON) {
      // Shortcuts
      const float* MiRow = M[iRow];
      const int sgnMIRowIVar = sgn(MiRow[0]);
      const float YIRowDivideByFabsMIRowIVar = Y[iRow] / fabsMIRowIVar;
      // For each following rows
      for (
        int jRow = iRow + 1;
        jRow < nbRows;
        ++jRow) {
        // If coefficients of the eliminated variable in the two rows have
        // different signs and are not null
        if (
          sgnMIRowIVar != sgn(M[jRow][0]) &&
          fabs(M[jRow][0]) > PBMATH_EPSILON) {
          // Shortcuts
          const float* MjRow = M[jRow];
          const float fabsMjRow = fabs(MjRow[0]);
          // Declare a variable to memorize the sum of the negative
          // coefficients in the row
          float sumNegCoeff = 0.0;
          // Add the sum of the two normed (relative to the eliminated
          // variable) rows into the result system. This actually
          // eliminate the variable while keeping the constraints on
          // others variables
          for (
            int iCol = 1;
            iCol < nbCols;
            ++iCol ) {
            Mp[nbResRows][iCol - 1] =
              MiRow[iCol] / fabsMIRowIVar +
              MjRow[iCol] / fabsMjRow;
            // Update the sum of the negative coefficient
            sumNegCoeff += neg(Mp[nbResRows][iCol - 1]);
          }
          // Update the right side of the inequality
          Yp[nbResRows] =
            YIRowDivideByFabsMIRowIVar +
            Y[jRow] / fabsMjRow;
          // If the right side of the inequality is lower than the sum of
          // negative coefficients in the row
          // (Add epsilon for numerical imprecision)
          if (Yp[nbResRows] < sumNegCoeff - PBMATH_EPSILON) {
            // Given that X is in [0,1], the system is inconsistent
            return true;
          }
          // Increment the nb of rows into the result system
          ++nbResRows;
        }
      }
    }
  }
  // Then we copy and compress the rows where the eliminated
  // variable is null
  // Loop on rows of the input system
  for (
    int iRow = 0;
    iRow < nbRows;
    ++iRow) {
    // Shortcut
    const float* MiRow = M[iRow];
    // If the coefficient of the eliminated variable is null on
    // this row
    if (fabs(MiRow[0]) < PBMATH_EPSILON) {
      // Shortcut
      float* MpnbResRows = Mp[nbResRows];
      // Copy this row into the result system excluding the eliminated
      // variable
      for (
        int iCol = 1;
        iCol < nbCols;
        ++iCol) {
        MpnbResRows[iCol - 1] = MiRow[iCol];
      }
      Yp[nbResRows] = Y[iRow];
      // Increment the nb of rows into the result system
      ++nbResRows;
    }
  }
  // Memorize the number of rows in the result system
  *nbRemainRows = nbResRows;
  // If we reach here the system is not inconsistent
  return false;
}

typedef struct {
  float min[3];
  float max[3];
} AABB3D;

// Get the bounds of the iVar-th variable in the nbRows rows
// system M.X<=Y where the iVar-th variable is on the first column
// and store them in the iVar-th axis of the AABB bdgBox
// (M arrangement is [iRow][iCol])
void GetBoundVar3D(
  const int iVar,
  const float (*M)[3],
  const float* Y,
  const int nbRows,
  const int nbCols,
  AABB3D* const bdgBox) {
  // Shortcuts
  float* bdgBoxMin = bdgBox->min;
  float* bdgBoxMax = bdgBox->max;
  // Initialize the bounds
  bdgBoxMin[iVar] = 0.0;
  bdgBoxMax[iVar] = 1.0;
  // Loop on the rows
  for (
    int iRow = 0;
    iRow < nbRows;
    ++iRow) {
    // Shortcuts
    const float* MIRow = M[iRow];
    float fabsMIRowIVar = fabs(MIRow[0]);
    // If the coefficient of the first variable on this row is not null
    if (fabsMIRowIVar > PBMATH_EPSILON) {
      // Declare two variables to memorize the min and max of the
      // requested variable in this row
      float min = -1.0 * Y[iRow];
      float max = Y[iRow];
      // Loop on columns except the first one which is the one of the
      // requested variable
      for (
        int iCol = 1;
        iCol < nbCols;
        ++iCol) {
        if (MIRow[iCol] > PBMATH_EPSILON) {
          min += MIRow[iCol] * bdgBoxMin[iCol + iVar];
          max -= MIRow[iCol] * bdgBoxMin[iCol + iVar];
        } else if (MIRow[iCol] < PBMATH_EPSILON) {
          min += MIRow[iCol] * bdgBoxMax[iCol + iVar];
          max -= MIRow[iCol] * bdgBoxMax[iCol + iVar];
        }
      }
      min /= -1.0 * MIRow[0];
      max /= MIRow[0];
      if (bdgBoxMin[iVar] > min) {
        bdgBoxMin[iVar] = min;
      }
      if (bdgBoxMax[iVar] < max) {
        bdgBoxMax[iVar] = max;
      }
    }
  }
}

// Get the bounds of the iVar-th variable in the nbRows rows
// system M.X<=Y which has been reduced to only one variable
// and store them in the iVar-th axis of the
// AABB bdgBox
// (M arrangement is [iRow][iCol])
// May return inconsistent values (max < min), which would
// mean the system has no solution
void GetBoundLastVar3D(
  const int iVar,
  const float (*M)[3],
  const float* Y,
  const int nbRows,
  AABB3D* const bdgBox) {
  // Shortcuts
  float* min = bdgBox->min + iVar;
  float* max = bdgBox->max + iVar;
  // Initialize the bounds to their maximum maximum and minimum minimum
  *min = 0.0;
  *max = 1.0;
  // Loop on rows
  for (
    int jRow = 0;
    jRow < nbRows;
    ++jRow) {
    // Shortcut
    float MjRowiVar = M[jRow][0];
    // If this row has been reduced to the variable in argument
    // and it has a strictly positive coefficient
    if (MjRowiVar > PBMATH_EPSILON) {
      // Get the scaled value of Y for this row
      float y = Y[jRow] / MjRowiVar;
      // If the value is lower than the current maximum bound
      if (*max > y) {
        // Update the maximum bound
        *max = y;
      }
    // Else, if this row has been reduced to the variable in argument
    // and it has a strictly negative coefficient
    } else if (MjRowiVar < -PBMATH_EPSILON) {
      // Get the scaled value of Y for this row
      float y = Y[jRow] / MjRowiVar;
      // If the value is greater than the current minimum bound
      if (*min < y) {
        // Update the minimum bound
        *min = y;
      }
    }
  }
}

// FMB algorithm on 3D Facoid-Facoid
typedef struct {
  float orig[3];
  float comp[3][3];
} Frame3D;
bool FMBFF(const Facoid* const that, 
  const Facoid* const tho) {
  // Get the projection of the Frame tho in Frame that coordinates
  // system
  // TODO: should be externalised
  Frame3D thoProj;
  const float* qo  = tho->_s._pos->_val;
  float* qpo = thoProj.orig;
  const float* po  = that->_s._pos->_val;
  const float* pi = that->_s._sysLinEqImport->_Minv->_val;
  float (*qpc)[3] = thoProj.comp;
  float v[3];
  for (
    int i = 3;
    i--;) {
    v[i] = qo[i] - po[i];
  }
  for (
    int i = 3;
    i--;) {
    qpo[i] = 0.0;
    for (
      int j = 3;
      j--;) {
      qpo[i] += pi[j + 3 * i] * v[j];
      qpc[j][i] = 0.0;
      for (
        int k = 3;
        k--;) {
        qpc[j][i] += pi[k + 3 * i] * tho->_s._axis[j]->_val[k];
      }
    }
  }
  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  float M[12][3];
  float Y[12];
  // Create the inequality system
  // -sum_iC_j,iX_i<=O_j
  M[0][0] = -thoProj.comp[0][0];
  M[0][1] = -thoProj.comp[1][0];
  M[0][2] = -thoProj.comp[2][0];
  Y[0] = thoProj.orig[0];
  if (Y[0] < neg(M[0][0]) + neg(M[0][1]) + neg(M[0][2])) {
    return false;
  }
  M[1][0] = -thoProj.comp[0][1];
  M[1][1] = -thoProj.comp[1][1];
  M[1][2] = -thoProj.comp[2][1];
  Y[1] = thoProj.orig[1];
  if (Y[1] < neg(M[1][0]) + neg(M[1][1]) + neg(M[1][2])) {
    return false;
  }
  M[2][0] = -thoProj.comp[0][2];
  M[2][1] = -thoProj.comp[1][2];
  M[2][2] = -thoProj.comp[2][2];
  Y[2] = thoProj.orig[2];
  if (Y[2] < neg(M[2][0]) + neg(M[2][1]) + neg(M[2][2])) {
    return false;
  }
  // Variable to memorize the nb of rows in the system
  int nbRows = 12;
  // sum_iC_j,iX_i<=1.0-O_j
  M[3][0] = thoProj.comp[0][0];
  M[3][1] = thoProj.comp[1][0];
  M[3][2] = thoProj.comp[2][0];
  Y[3] = 1.0 - thoProj.orig[0];
  if (
    Y[3] < neg(M[3][0]) + neg(M[3][1]) + neg(M[3][2])) {
    return false;
  }
  M[4][0] = thoProj.comp[0][1];
  M[4][1] = thoProj.comp[1][1];
  M[4][2] = thoProj.comp[2][1];
  Y[4] = 1.0 - thoProj.orig[1];
  if (
    Y[4] < neg(M[4][0]) + neg(M[4][1]) + neg(M[4][2])) {
    return false;
  }
  M[5][0] = thoProj.comp[0][2];
  M[5][1] = thoProj.comp[1][2];
  M[5][2] = thoProj.comp[2][2];
  Y[5] = 1.0 - thoProj.orig[2];
  if (
    Y[5] < neg(M[5][0]) + neg(M[5][1]) + neg(M[5][2])) {
    return false;
  }
  // X_i <= 1.0
  M[6][0] = 1.0;
  M[6][1] = 0.0;
  M[6][2] = 0.0;
  Y[6] = 1.0;
  M[7][0] = 0.0;
  M[7][1] = 1.0;
  M[7][2] = 0.0;
  Y[7] = 1.0;
  M[8][0] = 0.0;
  M[8][1] = 0.0;
  M[8][2] = 1.0;
  Y[8] = 1.0;
  // -X_i <= 0.0
  M[9][0] = -1.0;
  M[9][1] = 0.0;
  M[9][2] = 0.0;
  Y[9] = 0.0;
  M[10][0] = 0.0;
  M[10][1] = -1.0;
  M[10][2] = 0.0;
  Y[10] = 0.0;
  M[11][0] = 0.0;
  M[11][1] = 0.0;
  M[11][2] = -1.0;
  Y[11] = 0.0;
  // Solve the system
  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of tho
  AABB3D bdgBoxLocal = {
    .min = {0.0, 0.0, 0.0},
    .max = {0.0, 0.0, 0.0}
  };
  // Declare variables to eliminate the first variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mp[20][3];
  float Yp[20];
  int nbRowsP;
  // Eliminate the first variable in the original system
  bool inconsistency =
    ElimVar3D(
      M,
      Y,
      nbRows,
      3,
      Mp,
      Yp,
      &nbRowsP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Declare variables to eliminate the second variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mpp[55][3];
  float Ypp[55];
  int nbRowsPP;
  // Eliminate the second variable (which is the first in the new system)
  inconsistency =
    ElimVar3D(
      Mp,
      Yp,
      nbRowsP,
      2,
      Mpp,
      Ypp,
      &nbRowsPP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Get the bounds for the remaining third variable
  GetBoundLastVar3D(
    2,
    Mpp,
    Ypp,
    nbRowsPP,
    &bdgBoxLocal);
  // If the bounds are inconsistent
  if (bdgBoxLocal.min[2] >= bdgBoxLocal.max[2]) {
    // The two Frames are not in intersection
    return false;
  }
  // If we've reached here the two Frames are intersecting
  return true;
}

// FMB algorithm on 3D Facoid-Pyramidoid
bool FMBFP(const Facoid* const that, 
  const Pyramidoid* const tho) {
  // Get the projection of the Frame tho in Frame that coordinates
  // system
  // TODO: should be externalised
  Frame3D thoProj;
  const float* qo  = tho->_s._pos->_val;
  float* qpo = thoProj.orig;
  const float* po  = that->_s._pos->_val;
  const float* pi = that->_s._sysLinEqImport->_Minv->_val;
  float (*qpc)[3] = thoProj.comp;
  float v[3];
  for (
    int i = 3;
    i--;) {
    v[i] = qo[i] - po[i];
  }
  for (
    int i = 3;
    i--;) {
    qpo[i] = 0.0;
    for (
      int j = 3;
      j--;) {
      qpo[i] += pi[j + 3 * i] * v[j];
      qpc[j][i] = 0.0;
      for (
        int k = 3;
        k--;) {
        qpc[j][i] += pi[k + 3 * i] * tho->_s._axis[j]->_val[k];
      }
    }
  }
  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  float M[10][3];
  float Y[10];
  // Create the inequality system
  // -sum_iC_j,iX_i<=O_j
  M[0][0] = -thoProj.comp[0][0];
  M[0][1] = -thoProj.comp[1][0];
  M[0][2] = -thoProj.comp[2][0];
  Y[0] = thoProj.orig[0];
  if (Y[0] < neg(M[0][0]) + neg(M[0][1]) + neg(M[0][2])) {
    return false;
  }
  M[1][0] = -thoProj.comp[0][1];
  M[1][1] = -thoProj.comp[1][1];
  M[1][2] = -thoProj.comp[2][1];
  Y[1] = thoProj.orig[1];
  if (Y[1] < neg(M[1][0]) + neg(M[1][1]) + neg(M[1][2])) {
    return false;
  }
  M[2][0] = -thoProj.comp[0][2];
  M[2][1] = -thoProj.comp[1][2];
  M[2][2] = -thoProj.comp[2][2];
  Y[2] = thoProj.orig[2];
  if (Y[2] < neg(M[2][0]) + neg(M[2][1]) + neg(M[2][2])) {
    return false;
  }
  // Variable to memorize the nb of rows in the system
  int nbRows = 10;
  // sum_iC_j,iX_i<=1.0-O_j
  M[3][0] = thoProj.comp[0][0];
  M[3][1] = thoProj.comp[1][0];
  M[3][2] = thoProj.comp[2][0];
  Y[3] = 1.0 - thoProj.orig[0];
  if (
    Y[3] < neg(M[3][0]) + neg(M[3][1]) + neg(M[3][2])) {
    return false;
  }
  M[4][0] = thoProj.comp[0][1];
  M[4][1] = thoProj.comp[1][1];
  M[4][2] = thoProj.comp[2][1];
  Y[4] = 1.0 - thoProj.orig[1];
  if (
    Y[4] < neg(M[4][0]) + neg(M[4][1]) +
    neg(M[4][2])) {
    return false;
  }
  M[5][0] = thoProj.comp[0][2];
  M[5][1] = thoProj.comp[1][2];
  M[5][2] = thoProj.comp[2][2];
  Y[5] = 1.0 - thoProj.orig[2];
  if (
    Y[5] < neg(M[5][0]) + neg(M[5][1]) +
    neg(M[5][2])) {
    return false;
  }
  // sum_iX_i<=1.0
  M[6][0] = 1.0;
  M[6][1] = 1.0;
  M[6][2] = 1.0;
  Y[6] = 1.0;
  // -X_i <= 0.0
  M[7][0] = -1.0;
  M[7][1] = 0.0;
  M[7][2] = 0.0;
  Y[7] = 0.0;
  M[8][0] = 0.0;
  M[8][1] = -1.0;
  M[8][2] = 0.0;
  Y[8] = 0.0;
  M[9][0] = 0.0;
  M[9][1] = 0.0;
  M[9][2] = -1.0;
  Y[9] = 0.0;
  // Solve the system
  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of tho
  AABB3D bdgBoxLocal = {
    .min = {0.0, 0.0, 0.0},
    .max = {0.0, 0.0, 0.0}
  };
  // Declare variables to eliminate the first variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mp[20][3];
  float Yp[20];
  int nbRowsP;
  // Eliminate the first variable in the original system
  bool inconsistency =
    ElimVar3D(
      M,
      Y,
      nbRows,
      3,
      Mp,
      Yp,
      &nbRowsP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Declare variables to eliminate the second variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mpp[55][3];
  float Ypp[55];
  int nbRowsPP;
  // Eliminate the second variable (which is the first in the new system)
  inconsistency =
    ElimVar3D(
      Mp,
      Yp,
      nbRowsP,
      2,
      Mpp,
      Ypp,
      &nbRowsPP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Get the bounds for the remaining third variable
  GetBoundLastVar3D(
    2,
    Mpp,
    Ypp,
    nbRowsPP,
    &bdgBoxLocal);
  // If the bounds are inconsistent
  if (bdgBoxLocal.min[2] >= bdgBoxLocal.max[2]) {
    // The two Frames are not in intersection
    return false;
  }
  // If we've reached here the two Frames are intersecting
  return true;
}

// FMB algorithm on 3D Pyramidoid-Facoid
bool FMBPF(const Pyramidoid* const that, 
  const Facoid* const tho) {
  // Get the projection of the Frame tho in Frame that coordinates
  // system
  // TODO: should be externalised
  Frame3D thoProj;
  const float* qo  = tho->_s._pos->_val;
  float* qpo = thoProj.orig;
  const float* po  = that->_s._pos->_val;
  const float* pi = that->_s._sysLinEqImport->_Minv->_val;
  float (*qpc)[3] = thoProj.comp;
  float v[3];
  for (
    int i = 3;
    i--;) {
    v[i] = qo[i] - po[i];
  }
  for (
    int i = 3;
    i--;) {
    qpo[i] = 0.0;
    for (
      int j = 3;
      j--;) {
      qpo[i] += pi[j + 3 * i] * v[j];
      qpc[j][i] = 0.0;
      for (
        int k = 3;
        k--;) {
        qpc[j][i] += pi[k + 3 * i] * tho->_s._axis[j]->_val[k];
      }
    }
  }
  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  float M[10][3];
  float Y[10];
  // Create the inequality system
  // -sum_iC_j,iX_i<=O_j
  M[0][0] = -thoProj.comp[0][0];
  M[0][1] = -thoProj.comp[1][0];
  M[0][2] = -thoProj.comp[2][0];
  Y[0] = thoProj.orig[0];
  if (Y[0] < neg(M[0][0]) + neg(M[0][1]) + neg(M[0][2])) {
    return false;
  }
  M[1][0] = -thoProj.comp[0][1];
  M[1][1] = -thoProj.comp[1][1];
  M[1][2] = -thoProj.comp[2][1];
  Y[1] = thoProj.orig[1];
  if (Y[1] < neg(M[1][0]) + neg(M[1][1]) + neg(M[1][2])) {
    return false;
  }
  M[2][0] = -thoProj.comp[0][2];
  M[2][1] = -thoProj.comp[1][2];
  M[2][2] = -thoProj.comp[2][2];
  Y[2] = thoProj.orig[2];
  if (Y[2] < neg(M[2][0]) + neg(M[2][1]) + neg(M[2][2])) {
    return false;
  }
  // Variable to memorize the nb of rows in the system
  int nbRows = 10;
  // sum_j(sum_iC_j,iX_i)<=1.0-sum_iO_i
  M[3][0] =
    thoProj.comp[0][0] + thoProj.comp[0][1] + thoProj.comp[0][2];
  M[3][1] =
    thoProj.comp[1][0] + thoProj.comp[1][1] + thoProj.comp[1][2];
  M[3][2] =
    thoProj.comp[2][0] + thoProj.comp[2][1] + thoProj.comp[2][2];
  Y[3] =
    1.0 - thoProj.orig[0] - thoProj.orig[1] - thoProj.orig[2];
  if (
    Y[3] < neg(M[3][0]) + neg(M[3][1]) + neg(M[3][2])) {
    return false;
  }
  // X_i <= 1.0
  M[4][0] = 1.0;
  M[4][1] = 0.0;
  M[4][2] = 0.0;
  Y[4] = 1.0;
  M[5][0] = 0.0;
  M[5][1] = 1.0;
  M[5][2] = 0.0;
  Y[5] = 1.0;
  M[6][0] = 0.0;
  M[6][1] = 0.0;
  M[6][2] = 1.0;
  Y[6] = 1.0;
  // -X_i <= 0.0
  M[7][0] = -1.0;
  M[7][1] = 0.0;
  M[7][2] = 0.0;
  Y[7] = 0.0;
  M[8][0] = 0.0;
  M[8][1] = -1.0;
  M[8][2] = 0.0;
  Y[8] = 0.0;
  M[9][0] = 0.0;
  M[9][1] = 0.0;
  M[9][2] = -1.0;
  Y[9] = 0.0;
  // Solve the system
  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of tho
  AABB3D bdgBoxLocal = {
    .min = {0.0, 0.0, 0.0},
    .max = {0.0, 0.0, 0.0}
  };
  // Declare variables to eliminate the first variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mp[20][3];
  float Yp[20];
  int nbRowsP;
  // Eliminate the first variable in the original system
  bool inconsistency =
    ElimVar3D(
      M,
      Y,
      nbRows,
      3,
      Mp,
      Yp,
      &nbRowsP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Declare variables to eliminate the second variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mpp[55][3];
  float Ypp[55];
  int nbRowsPP;
  // Eliminate the second variable (which is the first in the new system)
  inconsistency =
    ElimVar3D(
      Mp,
      Yp,
      nbRowsP,
      2,
      Mpp,
      Ypp,
      &nbRowsPP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Get the bounds for the remaining third variable
  GetBoundLastVar3D(
    2,
    Mpp,
    Ypp,
    nbRowsPP,
    &bdgBoxLocal);
  // If the bounds are inconsistent
  if (bdgBoxLocal.min[2] >= bdgBoxLocal.max[2]) {
    // The two Frames are not in intersection
    return false;
  }
  // If we've reached here the two Frames are intersecting
  return true;
}

// FMB algorithm on 3D Pyramidoid-Pyramidoid
bool FMBPP(const Pyramidoid* const that, 
  const Pyramidoid* const tho) {
  // Get the projection of the Frame tho in Frame that coordinates
  // system
  // TODO: should be externalised
  Frame3D thoProj;
  const float* qo  = tho->_s._pos->_val;
  float* qpo = thoProj.orig;
  const float* po  = that->_s._pos->_val;
  const float* pi = that->_s._sysLinEqImport->_Minv->_val;
  float (*qpc)[3] = thoProj.comp;
  float v[3];
  for (
    int i = 3;
    i--;) {
    v[i] = qo[i] - po[i];
  }
  for (
    int i = 3;
    i--;) {
    qpo[i] = 0.0;
    for (
      int j = 3;
      j--;) {
      qpo[i] += pi[j + 3 * i] * v[j];
      qpc[j][i] = 0.0;
      for (
        int k = 3;
        k--;) {
        qpc[j][i] += pi[k + 3 * i] * tho->_s._axis[j]->_val[k];
      }
    }
  }
  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  float M[8][3];
  float Y[8];
  // Create the inequality system
  // -sum_iC_j,iX_i<=O_j
  M[0][0] = -thoProj.comp[0][0];
  M[0][1] = -thoProj.comp[1][0];
  M[0][2] = -thoProj.comp[2][0];
  Y[0] = thoProj.orig[0];
  if (Y[0] < neg(M[0][0]) + neg(M[0][1]) + neg(M[0][2])) {
    return false;
  }
  M[1][0] = -thoProj.comp[0][1];
  M[1][1] = -thoProj.comp[1][1];
  M[1][2] = -thoProj.comp[2][1];
  Y[1] = thoProj.orig[1];
  if (Y[1] < neg(M[1][0]) + neg(M[1][1]) + neg(M[1][2])) {
    return false;
  }
  M[2][0] = -thoProj.comp[0][2];
  M[2][1] = -thoProj.comp[1][2];
  M[2][2] = -thoProj.comp[2][2];
  Y[2] = thoProj.orig[2];
  if (Y[2] < neg(M[2][0]) + neg(M[2][1]) + neg(M[2][2])) {
    return false;
  }
  // Variable to memorize the nb of rows in the system
  int nbRows = 8;
  // sum_j(sum_iC_j,iX_i)<=1.0-sum_iO_i
  M[3][0] =
    thoProj.comp[0][0] + thoProj.comp[0][1] + thoProj.comp[0][2];
  M[3][1] =
    thoProj.comp[1][0] + thoProj.comp[1][1] + thoProj.comp[1][2];
  M[3][2] =
    thoProj.comp[2][0] + thoProj.comp[2][1] + thoProj.comp[2][2];
  Y[3] =
    1.0 - thoProj.orig[0] - thoProj.orig[1] - thoProj.orig[2];
  if (
    Y[3] < neg(M[3][0]) + neg(M[3][1]) + neg(M[3][2])) {
    return false;
  }
  // sum_iX_i<=1.0
  M[4][0] = 1.0;
  M[4][1] = 1.0;
  M[4][2] = 1.0;
  Y[4] = 1.0;
  // -X_i <= 0.0
  M[5][0] = -1.0;
  M[5][1] = 0.0;
  M[5][2] = 0.0;
  Y[5] = 0.0;
  M[6][0] = 0.0;
  M[6][1] = -1.0;
  M[6][2] = 0.0;
  Y[6] = 0.0;
  M[7][0] = 0.0;
  M[7][1] = 0.0;
  M[7][2] = -1.0;
  Y[7] = 0.0;
  // Solve the system
  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of tho
  AABB3D bdgBoxLocal = {
    .min = {0.0, 0.0, 0.0},
    .max = {0.0, 0.0, 0.0}
  };
  // Declare variables to eliminate the first variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mp[20][3];
  float Yp[20];
  int nbRowsP;
  // Eliminate the first variable in the original system
  bool inconsistency =
    ElimVar3D(
      M,
      Y,
      nbRows,
      3,
      Mp,
      Yp,
      &nbRowsP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Declare variables to eliminate the second variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  float Mpp[55][3];
  float Ypp[55];
  int nbRowsPP;
  // Eliminate the second variable (which is the first in the new system)
  inconsistency =
    ElimVar3D(
      Mp,
      Yp,
      nbRowsP,
      2,
      Mpp,
      Ypp,
      &nbRowsPP);
  // If the system is inconsistent
  if (inconsistency == true) {
    // The two Frames are not in intersection
    return false;
  }
  // Get the bounds for the remaining third variable
  GetBoundLastVar3D(
    2,
    Mpp,
    Ypp,
    nbRowsPP,
    &bdgBoxLocal);
  // If the bounds are inconsistent
  if (bdgBoxLocal.min[2] >= bdgBoxLocal.max[2]) {
    // The two Frames are not in intersection
    return false;
  }
  // If we've reached here the two Frames are intersecting
  return true;
}

bool _FacoidIsInterFacoid(const Facoid* const that, 
  const Facoid* const tho) {
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
      "'that' and 'tho' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(tho));
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != 2 && ShapoidGetDim(that) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'tho' have invalid dimensions (2<=%d<=3)",
      ShapoidGetDim(that));
    PBErrCatch(ShapoidErr);
  }
#endif
  if (ShapoidGetDim(that) == 2) {
    return SATFF(that, tho);
  } else if (ShapoidGetDim(that) == 3) {
    return FMBFF(that, tho); 
  } else {
    return false;
  }
}

bool _FacoidIsInterPyramidoid(const Facoid* const that, 
  const Pyramidoid* const tho) {
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
      "'that' and 'tho' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(tho));
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != 2 && ShapoidGetDim(that) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'tho' have invalid dimensions (2<=%d<=3)",
      ShapoidGetDim(that));
    PBErrCatch(ShapoidErr);
  }
#endif
  if (ShapoidGetDim(that) == 2) {
    return SATFP(that, tho);
  } else if (ShapoidGetDim(that) == 3) {
    return FMBFP(that, tho); 
  } else {
    return false;
  }
}

bool _PyramidoidIsInterFacoid(const Pyramidoid* const that, 
  const Facoid* const tho) {
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
      "'that' and 'tho' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(tho));
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != 2 && ShapoidGetDim(that) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'tho' have invalid dimensions (2<=%d<=3)",
      ShapoidGetDim(that));
    PBErrCatch(ShapoidErr);
  }
#endif
  if (ShapoidGetDim(that) == 2) {
    return SATPF(that, tho);
  } else if (ShapoidGetDim(that) == 3) {
    return FMBPF(that, tho); 
  } else {
    return false;
  }
}

bool _PyramidoidIsInterPyramidoid(const Pyramidoid* const that, 
  const Pyramidoid* const tho) {
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
      "'that' and 'tho' have different dimensions (%d==%d)",
      ShapoidGetDim(that), ShapoidGetDim(tho));
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(that) != 2 && ShapoidGetDim(that) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' and 'tho' have invalid dimensions (2<=%d<=3)",
      ShapoidGetDim(that));
    PBErrCatch(ShapoidErr);
  }
#endif
  if (ShapoidGetDim(that) == 2) {
    return SATPP(that, tho);
  } else if (ShapoidGetDim(that) == 3) {
    return FMBPP(that, tho); 
  } else {
    return false;
  }
}

// Update the major and minor axis of the Spheroid 'that'
void SpheroidUpdateMajMinAxis(Spheroid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  that->_majAxis = 0;
  float maj = VecNorm(ShapoidAxis(that, 0));
  that->_minAxis = 0;
  float min = maj;
  for (int iAxis = ShapoidGetDim(that); iAxis-- && iAxis != 0;) {
    float n = VecNorm(ShapoidAxis(that, iAxis));
    if (n > maj) {
      maj = n;
      that->_majAxis = iAxis;
    } else if (n < min) {
      min = n;
      that->_minAxis = iAxis;
    }
  }
}

// Get the maximum distance from the center of the Shapoid 'that' and 
// its surface
// Currenty only defined for spheroid, return 0.0 else
float _ShapoidGetBoundingRadius(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  if (ShapoidGetType(that) == ShapoidTypeSpheroid) {
    return 
      VecNorm(ShapoidAxis(that, ((Spheroid*)that)->_majAxis)) * 0.5;
  }
  return 0.0;
}

// -------------- ShapoidIter

// ================ Functions declaration ====================

// Step the ShapoidIter 'that' for a Facoid
// Return false if the iterator is at its end and couldn't be stepped
bool _ShapoidIterStepFacoid(ShapoidIter* const that);

// Step the ShapoidIter 'that' for a Pyramidoid
// Return false if the iterator is at its end and couldn't be stepped
bool _ShapoidIterStepPyramidoid(ShapoidIter* const that);

// Step the ShapoidIter 'that' for a Spheroid
// Return false if the iterator is at its end and couldn't be stepped
bool _ShapoidIterStepSpheroid(ShapoidIter* const that);

// ================ Functions implementation ====================

// Create a new iterator on the Shapoid 'shap' with a step of 'delta'
// (step on the internal coordinates of the Shapoid)
// The iterator is initialized and ready to be stepped
ShapoidIter _ShapoidIterCreateStatic(const Shapoid* const shap, 
  const VecFloat* const delta) {
#if BUILDMODE == 0
  if (shap == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'shap' is null");
    PBErrCatch(ShapoidErr);
  }
  if (delta == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'delta' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(delta) != ShapoidGetDim(shap)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'delta' dimensions and 'shap' dimensions don't match (%ld==%d)",
      VecGetDim(delta), ShapoidGetDim(shap));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare the new iterator
  ShapoidIter iter;
  // Set properties
  iter._shap = shap;
  iter._delta = VecClone(delta);
  iter._pos = VecFloatCreate(VecGetDim(delta));
  // Init the position
  ShapoidIterInit(&iter);
  // Return the new iterator
  return iter;
}

// Free the memory used by the ShapoidIter 'that'
void ShapoidIterFreeStatic(ShapoidIter* const that) {
  // Check argument
  if (that == NULL)
    // Nothing to do
    return;
  // Free memory
  VecFree(&(that->_delta));
  VecFree(&(that->_pos));
}

// Reinitialise the ShapoidIter 'that' to its starting position
void ShapoidIterInit(ShapoidIter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Initialise according to the type of Shapoid
  switch(ShapoidGetType(that->_shap)) {
    case ShapoidTypeFacoid:
    case ShapoidTypePyramidoid:
      VecSetNull(that->_pos);
      break;
    case ShapoidTypeSpheroid:
      VecSetNull(that->_pos);
      VecSet(that->_pos, VecGetDim(that->_pos) - 1, -0.5);
      break;
    default:
      break;
  }
}

// Step the ShapoidIter 'that'
// Return false if the iterator is at its end and couldn't be stepped
bool ShapoidIterStep(ShapoidIter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a flag for the return value
  bool flag = true;
  // Step according to the type of Shapoid
  switch(ShapoidGetType(that->_shap)) {
    case ShapoidTypeFacoid:
      flag = _ShapoidIterStepFacoid(that);
      break;
    case ShapoidTypePyramidoid:
      flag = _ShapoidIterStepPyramidoid(that);
      break;
    case ShapoidTypeSpheroid:
      flag = _ShapoidIterStepSpheroid(that);
      break;
    default:
      break;
  }
  return flag;
}

// Step the ShapoidIter 'that' for a Facoid
// Return false if the iterator is at its end and couldn't be stepped
bool _ShapoidIterStepFacoid(ShapoidIter* const that) {
  // Declare a variable for the returned flag
  bool ret = true;
  // Declare a variable to memorise the dimension currently increasing
  long iDim = VecGetDim(that->_pos) - 1;
  // Declare a flag for the loop condition 
  bool flag = true;
  // Increment
  do {
    VecSetAdd(that->_pos, iDim, VecGet(that->_delta, iDim));
    if (VecGet(that->_pos, iDim) > 1.0 + PBMATH_EPSILON) {
      VecSet(that->_pos, iDim, 0.0);
      --iDim;
    } else {
      flag = false;
    }
  } while (iDim >= 0 && flag == true);
  if (iDim == -1)
    ret = false;
  // Return the flag
  return ret;
}

// Step the ShapoidIter 'that' for a Pyramidoid
// Return false if the iterator is at its end and couldn't be stepped
bool _ShapoidIterStepPyramidoid(ShapoidIter* const that) {
  // Declare a variable for the returned flag
  bool ret = true;
  // Declare a variable to memorise the dimension currently increasing
  long iDim = VecGetDim(that->_pos) - 1;
  // Declare a flag for the loop condition 
  bool flag = true;
  // Increment
  do {
    VecSetAdd(that->_pos, iDim, VecGet(that->_delta, iDim));
    float sum = 0.0;
    for (long iAxis = VecGetDim(that->_pos); iAxis--;)
      sum += VecGet(that->_pos, iAxis);
    if (sum > 1.0 + PBMATH_EPSILON) {
      VecSet(that->_pos, iDim, 0.0);
      --iDim;
    } else {
      flag = false;
    }
  } while (iDim >= 0 && flag == true);
  if (iDim == -1)
    ret = false;
  // Return the flag
  return ret;
}

// Step the ShapoidIter 'that' for a Spheroid
// Return false if the iterator is at its end and couldn't be stepped
bool _ShapoidIterStepSpheroid(ShapoidIter* const that) {
  // Declare a variable to memorise the dimension currently increasing
  long iDim = 0;
  // Declare a flag for the loop condition 
  bool flag = true;
  // Declare a variable to memorize the norm of the current position
  float norm = VecNorm(that->_pos);
  // Ladies and Gentleman, here comes the infamous "Worm Algorithm"
  // Increment from the first axis
  for (iDim = 0; iDim < VecGetDim(that->_pos) && flag == true; ++iDim) {
    float prevNorm = norm;
    // Try to step in this axis
    VecSetAdd(that->_pos, iDim, VecGet(that->_delta, iDim));
    // Get the norm of the new position
    norm = VecNorm(that->_pos);
    // If we have just jumped over the boundary
    if (prevNorm < 0.5 - PBMATH_EPSILON && 
      norm >= 0.5 + PBMATH_EPSILON) {
      // Correct the step to reach exactly the boundary
      // Set the current axis to relax the constraint
      VecSet(that->_pos, iDim, 0.0);
      // Calculate the value for this axis which put back the position
      // at the boundary of the Spheroid (on positive side as we want
      // the end of the boundary)
      norm = VecNorm(that->_pos);
      float val =  0.5 * sqrt(-4.0 * (fastpow(norm, 2) - 0.25));
      VecSet(that->_pos, iDim, val);
      // Correct the norm
      norm = 0.5;
      // We could step on this axis, stop here
      flag = false;
      // To cancel the increment in the loop
      --iDim;
    } else {
      // If the new position is out of bound it means we reach the 
      // boundary
      if (norm >= 0.5 + PBMATH_EPSILON) {
        // Set the current axis to 0.0 to relax the constraint on
        // other axis
        VecSet(that->_pos, iDim, 0.0);
      } else {
        // We could step on this axis, stop here
        flag = false;
        // To cancel the increment in the loop
        --iDim;
      }
    }
  }
  // If we could step, it has modified the constraint on the previous
  // axis which must then be updated
  if (flag == false) {
    --iDim;
    // If there is actually a previous axis
    if (iDim >= 0) {
      // Calculate the value for this axis which put back the position
      // at the boundary of the Spheroid (on negative side as we will
      // increment from there)
      float val = VecGet(that->_pos, iDim) + 
        0.5 * (-2.0 * VecGet(that->_pos, iDim) - 
        sqrt(4.0 * (fastpow(VecGet(that->_pos, iDim), 2) - 
        fastpow(norm, 2) + 0.25)));
      VecSet(that->_pos, iDim, val);
    }
  }
  // Return the negative of the flag
  return !flag;
}
