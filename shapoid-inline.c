// ============ SHAPOID-static inline.C ================

// -------------- Shapoid

// ================ Functions implementation ====================

// Get the dimension of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
int _ShapoidGetDim(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return the dimension
  return that->_dim;
}

// Get the dimension of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
ShapoidType _ShapoidGetType(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return the type
  return that->_type;
}

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
#if BUILDMODE != 0
static inline
#endif 
const char* _ShapoidGetTypeAsString(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return the type
  return ShapoidTypeString[that->_type];
}

// Return a VecFloat equals to the position of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
VecFloat* _ShapoidGetPos(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return a clone of the position
  return VecClone(that->_pos);
}

// Return a VecFloat equals to the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
VecFloat* _ShapoidGetAxis(const Shapoid* const that, const int dim) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return a clone of the axis
  return VecClone(that->_axis[dim]);
}

// Return the position of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
const VecFloat* _ShapoidPos(const Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return the position
  return that->_pos;
}

// Return the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
const VecFloat* _ShapoidAxis(const Shapoid* const that, const int dim) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Return the axis
  return that->_axis[dim];
}

// Set the position of the Shapoid to 'pos'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidSetPos(Shapoid* const that, const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%ld==%d)", 
      VecGetDim(pos), that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the position
  VecCopy(that->_pos, pos);
}

// Set the 'iElem'-th value of the position of the Shapoid to 'val'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidPosSet(Shapoid* const that, const int iElem, 
  const float val) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (iElem < 0 || iElem >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'iElem' is invalid (0<=%d<%d)", 
      iElem, that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the position
  VecSet(that->_pos, iElem, val);
}

// Set the 'iElem'-th value of the position of the Shapoid to 'val' 
// added to its current value
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidPosSetAdd(Shapoid* const that, const int iElem, 
  const float val) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (iElem < 0 || iElem >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'iElem' is invalid (0<=%d<%d)", 
      iElem, that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the position
  VecSetAdd(that->_pos, iElem, val);
}

// Set the 'iElem'-th value of the position of the Shapoid to 'val'
#if BUILDMODE != 0
static inline
#endif 
float _ShapoidPosGet(const Shapoid* const that, const int iElem) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (iElem < 0 || iElem >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'iElem' is invalid (0<=%d<%d)", 
      iElem, that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the position
  return VecGet(that->_pos, iElem);
}

// Set the position of the Shapoid such as its center is at 'pos'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidSetCenterPos(Shapoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%ld==%d)", 
      VecGetDim(pos), that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  VecFloat* v = ShapoidGetCenter(that);
  VecOp(v, -1.0, ShapoidPos(that), 1.0);
  VecOp(v, 1.0, pos, 1.0);
  ShapoidSetPos(that, v);
  VecFree(&v);
}

// Set the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidSetAxis(Shapoid* const that, const int dim, 
  const VecFloat* const v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (v == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'v' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%ld)", 
      dim, VecGetDim(v));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the axis
  VecCopy(that->_axis[dim], v);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}

// Set all the axis of the Shapoid to vectors in 'set' (axis in 
// dimensions order
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidSetAllAxis(Shapoid* const that, GSetVecFloat* const set) {
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
  if (GSetNbElem(set) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "Number of elements in set is invalid (%ld!=%d)", 
      GSetNbElem(set), that->_dim);
    PBErrCatch(ShapoidErr);
  }
  for (int iDim = that->_dim; iDim--;) {
    VecFloat* vec = GSetGet(set, iDim);
    if (VecGetDim(vec) != that->_dim) {
      ShapoidErr->_type = PBErrTypeInvalidArg;
      sprintf(ShapoidErr->_msg, 
        "%d-th axis' dimension is invalid (%ld!=%d)", 
        iDim, VecGetDim(vec), that->_dim);
      PBErrCatch(ShapoidErr);
    }
  }
#endif
  // Set the axis
  GSetIterForward iter = GSetIterForwardCreateStatic(set);
  int iDim = 0;
  do {
    VecFloat* axis = GSetIterGet(&iter);
    VecCopy(that->_axis[iDim], axis);
    ++iDim;
  } while (GSetIterStep(&iter));
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}


// Set the 'iElem'-th element of the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidAxisSet(Shapoid* const that, const int dim, 
  const int iElem, const float v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
  if (iElem < 0 || iElem >= VecGetDim(ShapoidAxis(that, dim))) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "iElem is invalid (0<=%d<%ld)", 
      iElem, VecGetDim(ShapoidAxis(that, dim)));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the axis
  VecSet(that->_axis[dim], iElem, v);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}

// Set the 'iElem'-th element of the 'dim'-th axis of the Shapoid to 
// 'v' added to its current value
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidAxisSetAdd(Shapoid* const that, const int dim, 
  const int iElem, const float v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
  if (iElem < 0 || iElem >= VecGetDim(ShapoidAxis(that, dim))) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "iElem is invalid (0<=%d<%ld)", 
      iElem, VecGetDim(ShapoidAxis(that, dim)));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the axis
  VecSetAdd(that->_axis[dim], iElem, v);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}

// Get the 'iElem'-th element of the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
static inline
#endif 
float _ShapoidAxisGet(const Shapoid* const that, const int dim, 
  const int iElem) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
  if (iElem < 0 || iElem >= VecGetDim(ShapoidAxis(that, dim))) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "iElem is invalid (0<=%d<%ld)", 
      iElem, VecGetDim(ShapoidAxis(that, dim)));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the axis
  return VecGet(that->_axis[dim], iElem);
}

// Scale the 'dim'-th axis of the Shapoid by 'v'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidAxisScale(Shapoid* const that, const int dim, 
  const float v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (dim < 0 || dim >= that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "Axis' index is invalid (0<=%d<%d)", 
      dim, that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the axis
  VecScale(that->_axis[dim], v);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}

// Translate the Shapoid by 'v'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidTranslate(Shapoid* const that, const VecFloat* const v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (v == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'v' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(v));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Translate the position
  VecOp(that->_pos, 1.0, v, 1.0);
}

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidScaleVector(Shapoid* const that, const VecFloat* const v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (v == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'v' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(v));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Scale each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecScale(that->_axis[iAxis], VecGet(v, iAxis));
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}

// Scale the Shapoid by 'c'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidScaleScalar(Shapoid* const that, const float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Scale each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecScale(that->_axis[iAxis], c);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
  // If it's a Spheroid
  if (that->_type == ShapoidTypeSpheroid)
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
}

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidGrowVector(Shapoid* const that, const VecFloat* const v) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (v == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'v' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(v));
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
  // If the shapoid is a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
    // Scale
    ShapoidScale(that, v);
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
  // Else, the shapoid is not a spheroid
  } else {
    // Memorize the center
    VecFloat* centerA = ShapoidGetCenter(that);
    // Scale
    ShapoidScale(that, v);
    // Reposition to keep center at the same position
    VecFloat* centerB = ShapoidGetCenter(that);
    VecOp(centerA, 1.0, centerB, -1.0);
    VecOp(that->_pos, 1.0, centerA, 1.0);
    VecFree(&centerA);
    VecFree(&centerB);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Scale the Shapoid by 'c'
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidGrowScalar(Shapoid* const that, const float c) {
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
  // If the shapoid is a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
    // Scale
    ShapoidScale(that, c);
    // Update the major and minor axis
    SpheroidUpdateMajMinAxis((Spheroid*)that);
  // Else, the shapoid is not a spheroid
  } else {
    // Memorize the center
    VecFloat* centerA = ShapoidGetCenter(that);
    // Scale
    ShapoidScale(that, c);
    // Reposition to keep center at the same position
    VecFloat* centerB = ShapoidGetCenter(that);
    VecOp(centerA, 1.0, centerB, -1.0);
    VecOp(that->_pos, 1.0, centerA, 1.0);
    VecFree(&centerA);
    VecFree(&centerB);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its center
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotCenter(Shapoid* const that, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 2) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==2)", that->_dim);
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
  // If it's a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRot(that->_axis[iAxis], theta);
  // Else, it's not a spheroid
  } else {
    VecFloat* center = ShapoidGetCenter(that);
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRot(that->_axis[iAxis], theta);
    // Reposition the origin
    VecOp(that->_pos, 1.0, center, -1.0);
    VecRot(that->_pos, theta);

// ???????
// In BUILD_MODE == 1, that->_pos is not updated for an unknown reason
// Adding a dummy fflush here makes it work normally
// ???????
#if BUILDMODE == 1
fflush(stdout);
#endif

    VecOp(that->_pos, 1.0, center, 1.0);
    VecFree(&center);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to its position
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotStart(Shapoid* const that, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 2) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==2)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRot(that->_axis[iAxis], theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 2 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotOrigin(Shapoid* const that, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 2) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==2)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRot(that->_axis[iAxis], theta);
  // Reposition the origin
  VecRot(that->_pos, theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotAxisCenter(Shapoid* const that, 
  const VecFloat3D* const axis, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (axis == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'axis' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(axis) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'axis' 's dimension is invalid (%ld==3)", VecGetDim(axis));
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
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
  // If it's a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotAxis(that->_axis[iAxis], axis, theta);
  // Else, it's not a spheroid
  } else {
    VecFloat* center = ShapoidGetCenter(that);
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRotAxis(that->_axis[iAxis], axis, theta);
    // Reposition the origin
    VecFloat* v = VecGetOp(that->_pos, 1.0, center, -1.0);
    VecRotAxis(v, axis, theta);
    VecOp(v, 1.0, center, 1.0);
    VecCopy(that->_pos, v);
    VecFree(&center);
    VecFree(&v);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotAxisStart(Shapoid* const that, 
  const VecFloat3D* const axis, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (axis == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'axis' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(axis) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'axis' 's dimension is invalid (%ld==3)", VecGetDim(axis));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotAxis(that->_axis[iAxis], axis, theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around 'axis'
// 'axis' must be normalized
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotAxisOrigin(Shapoid* const that, 
  const VecFloat3D* const axis, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (axis == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'axis' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(axis) != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'axis' 's dimension is invalid (%ld==3)", VecGetDim(axis));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotAxis(that->_axis[iAxis], axis, theta);
  // Reposition the origin
  VecRotAxis(that->_pos, axis, theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around X
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotXCenter(Shapoid* const that, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
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
  // If it's a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotX(that->_axis[iAxis], theta);
  // Else, it's not a spheroid
  } else {
    VecFloat* center = ShapoidGetCenter(that);
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRotX(that->_axis[iAxis], theta);
    // Reposition the origin
    VecFloat* v = VecGetOp(that->_pos, 1.0, center, -1.0);
    VecRotX(v, theta);
    VecOp(v, 1.0, center, 1.0);
    VecCopy(that->_pos, v);
    VecFree(&center);
    VecFree(&v);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around X
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotXStart(Shapoid* const that, 
  float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotX(that->_axis[iAxis], theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around X
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotXOrigin(Shapoid* const that, 
  float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotX(that->_axis[iAxis], theta);
  // Reposition the origin
  VecRotX(that->_pos, theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around Y
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotYCenter(Shapoid* const that, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
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
  // If it's a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotY(that->_axis[iAxis], theta);
  // Else, it's not a spheroid
  } else {
    VecFloat* center = ShapoidGetCenter(that);
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRotY(that->_axis[iAxis], theta);
    // Reposition the origin
    VecFloat* v = VecGetOp(that->_pos, 1.0, center, -1.0);
    VecRotY(v, theta);
    VecOp(v, 1.0, center, 1.0);
    VecCopy(that->_pos, v);
    VecFree(&center);
    VecFree(&v);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around Y
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotYStart(Shapoid* const that, 
  float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotY(that->_axis[iAxis], theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around Y
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotYOrigin(Shapoid* const that, 
  float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotY(that->_axis[iAxis], theta);
  // Reposition the origin
  VecRotY(that->_pos, theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its center around Z
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotZCenter(Shapoid* const that, const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
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
  // If it's a spheroid
  if (that->_type == ShapoidTypeSpheroid) {
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotZ(that->_axis[iAxis], theta);
  // Else, it's not a spheroid
  } else {
    VecFloat* center = ShapoidGetCenter(that);
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRotZ(that->_axis[iAxis], theta);
    // Reposition the origin
    VecFloat* v = VecGetOp(that->_pos, 1.0, center, -1.0);
    VecRotZ(v, theta);
    VecOp(v, 1.0, center, 1.0);
    VecCopy(that->_pos, v);
    VecFree(&center);
    VecFree(&v);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to its position around Z
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotZStart(Shapoid* const that, 
  float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotZ(that->_axis[iAxis], theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Rotate the Shapoid of dimension 3 by 'theta' (in radians, CCW)
// relatively to the origin of the global coordinates system
// around Z
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidRotZOrigin(Shapoid* const that, 
  float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 3) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, 
      "'that' 's dimension is invalid (%d==3)", that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRotZ(that->_axis[iAxis], theta);
  // Reposition the origin
  VecRotZ(that->_pos, theta);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
#if BUILDMODE != 0
static inline
#endif 
VecFloat* _ShapoidImportCoord(const Shapoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Update the system solver for the requested position
  VecFloat* v = VecGetOp(pos, 1.0, that->_pos, -1.0);
  SysLinEqSetV(that->_sysLinEqImport, v);
  // Solve the system
  VecFloat* res = SysLinEqSolve(that->_sysLinEqImport);
  // Free memory
  VecFree(&v);
  // return the result
  return res;
}

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
#if BUILDMODE != 0
static inline
#endif 
VecFloat* _ShapoidExportCoord(const Shapoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Allocate memory for the result
  VecFloat* res = VecClone(that->_pos);
  for (int dim = that->_dim; dim--;)
    VecOp(res, 1.0, that->_axis[dim], VecGet(pos, dim));
  // Return the result
  return res;
}

// Get the center of the shapoid in standard coordinate system
#if BUILDMODE != 0
static inline
#endif 
VecFloat* _ShapoidGetCenter(const Shapoid* const that) {
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
  if (ShapoidGetType(that) == ShapoidTypeFacoid)
    return FacoidGetCenter((Facoid*)that);
  else if (ShapoidGetType(that) == ShapoidTypePyramidoid)
    return PyramidoidGetCenter((Pyramidoid*)that);
  else if (ShapoidGetType(that) == ShapoidTypeSpheroid)
    return SpheroidGetCenter((Spheroid*)that);
  else
    return NULL;
}

#if BUILDMODE != 0
static inline
#endif 
VecFloat* FacoidGetCenter(const Facoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a variable to memorize the result in Shapoid 
  // coordinate system
  VecFloat* coord = VecFloatCreate(ShapoidGetDim(that));
  // For each dimension
  for (int dim = ShapoidGetDim(that); dim--;)
    VecSet(coord, dim, 0.5);
  // Convert the coordinates in standard coordinate system
  VecFloat* res = ShapoidExportCoord(that, coord);
  // Free memory
  VecFree(&coord);
  // Return the result
  return res;
}

#if BUILDMODE != 0
static inline
#endif 
VecFloat* PyramidoidGetCenter(const Pyramidoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a variable to memorize the result in Shapoid 
  // coordinate system
  VecFloat* coord = VecFloatCreate(ShapoidGetDim(that));
  // For each dimension
  for (int dim = ShapoidGetDim(that); dim--;)
    VecSet(coord, dim, 1.0 / (1.0 + ShapoidGetDim(that)));
  // Convert the coordinates in standard coordinate system
  VecFloat* res = ShapoidExportCoord(that, coord);
  // Free memory
  VecFree(&coord);
  // Return the result
  return res;
}

#if BUILDMODE != 0
static inline
#endif 
VecFloat* SpheroidGetCenter(const Spheroid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  // Declare a variable to memorize the result in Shapoid 
  // coordinate system
  VecFloat* coord = VecFloatCreate(ShapoidGetDim(that));
  // Convert the coordinates in standard coordinate system
  VecFloat* res = ShapoidExportCoord(that, coord);
  // Free memory
  VecFree(&coord);
  // Return the result
  return res;
}

// Check if shapoid 'that' and 'tho' are equals
#if BUILDMODE != 0
static inline
#endif 
bool _ShapoidIsEqual(const Shapoid* const that, 
  const Shapoid* const tho) {
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
#endif
  // Check the dimension, type and position
  if (that->_dim != tho->_dim ||
    that->_type != tho->_type ||
    VecIsEqual(that->_pos, tho->_pos) == false)
    return false;
  // Check the axis
  for (int i = that->_dim; i--;)
    if (VecIsEqual(that->_axis[i], tho->_axis[i]) == false)
      return false;
  // If the Shapoid is a Spheroid, check Spheroid properties
  if (that->_type == ShapoidTypeSpheroid) {
    if (((Spheroid*)that)->_majAxis != ((Spheroid*)tho)->_majAxis ||
      ((Spheroid*)that)->_minAxis != ((Spheroid*)tho)->_minAxis)
      return false;
  }
  // Return the success code
  return true;
}

// Update the system of linear equation used to import coordinates
#if BUILDMODE != 0
static inline
#endif 
void ShapoidUpdateSysLinEqImport(Shapoid* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  VecShort2D dim = VecShortCreateStatic2D();
  // Set a pointer to the matrix in the SysLinEq
  MatFloat* mat = MatClone(that->_sysLinEqImport->_M);
  // Set the values of the matrix
  for (VecSet(&dim, 0, 0); VecGet(&dim, 0) < that->_dim; 
    VecSetAdd(&dim, 0, 1)) {
    for (VecSet(&dim, 1, 0); VecGet(&dim, 1) < that->_dim; 
      VecSetAdd(&dim, 1, 1)) {
      MatSet(mat, &dim, VecGet(that->_axis[VecGet(&dim, 0)], 
        VecGet(&dim, 1)));
    }
  }
  // Update the SysLinEq
  SysLinEqSetM(that->_sysLinEqImport, mat);
  // Free memory
  MatFree(&mat);
}

// Return true if 'pos' (in stand coordinate system) is inside the 
// Shapoid
// Else return false
#if BUILDMODE != 0
static inline
#endif 
bool _ShapoidIsPosInside(const Shapoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(pos));
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
    return FacoidIsPosInside((Facoid*)that, pos);
  // Else, if the Shapoid is a Pyramidoid
  } else if (that->_type == ShapoidTypePyramidoid) {
    return PyramidoidIsPosInside((Pyramidoid*)that, pos);
  // Else, if the Shapoid is a Spheroid
  } else if (that->_type == ShapoidTypeSpheroid) {
    return SpheroidIsPosInside((Spheroid*)that, pos);
  } else 
    return false;
}

#if BUILDMODE != 0
static inline
#endif 
bool FacoidIsPosInside(const Facoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != ShapoidGetDim(that)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      ShapoidGetDim(that), VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat* coord = ShapoidImportCoord(that, pos);
  // Declare a variable to memorize the result
  bool ret = false;
  // pos is in the Shapoid if all the coord in Shapoid coord 
  // system are in [0.0, 1.0]
  ret = true;
  for (int dim = ShapoidGetDim(that); dim-- && ret == true;) {
    float v = VecGet(coord, dim);
    if (v < 0.0 || v > 1.0)
      ret = false;
  }
  // Free memory
  VecFree(&coord);
  // Return the result
  return ret;
}

#if BUILDMODE != 0
static inline
#endif 
bool PyramidoidIsPosInside(const Pyramidoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != ShapoidGetDim(that)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      ShapoidGetDim(that), VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat* coord = ShapoidImportCoord(that, pos);
  // Declare a variable to memorize the result
  bool ret = false;
  // pos is in the Shapoid if all the coord in Shapoid coord 
  // system are in [0.0, 1.0] and their sum is in [0.0, 1.0]
  ret = true;
  float sum = 0.0;
  for (int dim = ShapoidGetDim(that); dim-- && ret == true;) {
    float v = VecGet(coord, dim);
    sum += v;
    if (v < 0.0 || v > 1.0)
      ret = false;
  }
  if (ret == true && sum > 1.0)
    ret = false;
  // Free memory
  VecFree(&coord);
  // Return the result
  return ret;
}

#if BUILDMODE != 0
static inline
#endif 
bool SpheroidIsPosInside(const Spheroid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != ShapoidGetDim(that)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      ShapoidGetDim(that), VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat* coord = ShapoidImportCoord(that, pos);
  // Declare a variable to memorize the result
  bool ret = false;
  // pos is in the Shapoid if its norm is in [0.0, 0.5]
  float norm = VecNorm(coord);
  if (norm <= 0.5)
    ret = true;
  // Free memory
  VecFree(&coord);
  // Return the result
  return ret;
}

// Get the depth value in the Shapoid of 'pos'
// The depth is defined as follow: the point with depth equals 1.0 is 
// the farthest point from the surface of the Shapoid (inside it),
// points with depth equals to 0.0 are point on the surface of the
// Shapoid. Depth is continuous and derivable over the volume of the
// Shapoid
#if BUILDMODE != 0
static inline
#endif 
float _ShapoidGetPosDepth(const Shapoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      that->_dim, VecGetDim(pos));
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
    return FacoidGetPosDepth((Facoid*)that, pos);
  // Else, if the Shapoid is a Pyramidoid
  } else if (that->_type == ShapoidTypePyramidoid) {
    return PyramidoidGetPosDepth((Pyramidoid*)that, pos);
  // Else, if the Shapoid is a Spheroid
  } else if (that->_type == ShapoidTypeSpheroid) {
    return SpheroidGetPosDepth((Spheroid*)that, pos);
  } else {
    return 0.0;
  }
}

#if BUILDMODE != 0
static inline
#endif 
float FacoidGetPosDepth(const Facoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != ShapoidGetDim(that)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      ShapoidGetDim(that), VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat* coord = ShapoidImportCoord(that, pos);
  // Declare a variable to memorize the result
  float ret = 1.0;
  for (int dim = ShapoidGetDim(that); dim-- && ret > PBMATH_EPSILON;) {
    float v = VecGet(coord, dim);
    if (v < 0.0 || VecGet(coord, dim) > 1.0)
      ret = 0.0;
    else
      ret *= 1.0 - pow(0.5 - v, 2.0) * 4.0;
  }
  // Free memory
  VecFree(&coord);
  // Return the result
  return ret;
}

#if BUILDMODE != 0
static inline
#endif 
float PyramidoidGetPosDepth(const Pyramidoid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != ShapoidGetDim(that)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      ShapoidGetDim(that), VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat* coord = ShapoidImportCoord(that, pos);
  // Declare a variable to memorize the result
  float ret = 1.0;
  float sum = 0.0;
  bool flag = true;
  for (int dim = ShapoidGetDim(that); dim-- && ret > PBMATH_EPSILON;) {
    float v = VecGet(coord, dim);
    sum += v;
    if (v < 0.0 || v > 1.0)
      flag = false;
  }
  if (flag == true && sum > 1.0)
    flag = false;
  if (flag == false)
    ret = 0.0;
  else {
    ret = 1.0;
    for (int dim = ShapoidGetDim(that); dim--;) {
      float z = 0.0;
      for (int d = ShapoidGetDim(that); d--;)
        if (d != dim)
          z += VecGet(coord, d);
      ret *= 
        (1.0 - 4.0 * pow(0.5 - VecGet(coord, dim) / (1.0 - z), 2.0));
    }
  }
  // Free memory
  VecFree(&coord);
  // Return the result
  return ret;
}

#if BUILDMODE != 0
static inline
#endif 
float SpheroidGetPosDepth(const Spheroid* const that, 
  const VecFloat* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (pos == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'pos' is null");
    PBErrCatch(ShapoidErr);
  }
  if (VecGetDim(pos) != ShapoidGetDim(that)) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%ld)", 
      ShapoidGetDim(that), VecGetDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat* coord = ShapoidImportCoord(that, pos);
  // Declare a variable to memorize the result
  float ret = 0.0;
  float norm = VecNorm(coord);
  if (norm <= 0.5)
    ret = 1.0 - norm * 2.0;
  // Free memory
  VecFree(&coord);
  // Return the result
  return ret;
}

#if BUILDMODE != 0
static inline
#endif 
bool FacoidLoad(Facoid** that, FILE* const stream) {
  bool ret = _ShapoidLoad((Shapoid**)that, stream);
  if (!ret || ShapoidGetType(*that) != ShapoidTypeFacoid) {
    ShapoidFree(that);
    return false;
  }
  return true;
}

#if BUILDMODE != 0
static inline
#endif 
bool PyramidoidLoad(Pyramidoid** that, FILE* const stream) {
  bool ret = _ShapoidLoad((Shapoid**)that, stream);
  if (!ret || ShapoidGetType(*that) != ShapoidTypePyramidoid) {
    ShapoidFree(that);
    return false;
  }
  return true;
}

#if BUILDMODE != 0
static inline
#endif 
bool SpheroidLoad(Spheroid** that, FILE* const stream) {
  bool ret = _ShapoidLoad((Shapoid**)that, stream);
  if (!ret || ShapoidGetType(*that) != ShapoidTypeSpheroid) {
    ShapoidFree(that);
    return false;
  }
  return true;
}

// -------------- ShapoidIter

// ================ Functions implementation ====================

// Return the current position in Shapoid coordinates of the 
// ShapoidIter 'that'
#if BUILDMODE != 0
static inline
#endif 
VecFloat* ShapoidIterGetInternalPos(const ShapoidIter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  return VecClone(that->_pos);
}

// Return the current position in standard coordinates of the 
// ShapoidIter 'that'
#if BUILDMODE != 0
static inline
#endif 
VecFloat* ShapoidIterGetExternalPos(const ShapoidIter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  return ShapoidExportCoord(that->_shap, that->_pos);
}

// Set the attached Shapoid of the ShapoidIter 'that' to 'shap'
// The iterator is reset to its initial position
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidIterSetShapoid(ShapoidIter* const that, 
  const Shapoid* const shap) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  that->_shap = shap;
}

// Get the Shapoid of the ShapoidIter 'that'
#if BUILDMODE != 0
static inline
#endif 
const Shapoid* ShapoidIterShapoid(const ShapoidIter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  return that->_shap;
}

// Set the delta of the ShapoidIter 'that' to a copy of 'delta'
#if BUILDMODE != 0
static inline
#endif 
void _ShapoidIterSetDelta(ShapoidIter* const that, 
  const VecFloat* const delta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  VecFree(&(that->_delta));
  that->_delta = VecClone(delta);
}

// Get the delta of the ShapoidIter 'that'
#if BUILDMODE != 0
static inline
#endif 
const VecFloat* ShapoidIterDelta(const ShapoidIter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  return that->_delta;
}
