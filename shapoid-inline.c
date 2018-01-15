// ============ SHAPOID-INLINE.C ================

// ================ Functions implementation ====================

// Get the dimension of the Shapoid
#if BUILDMODE != 0
inline
#endif 
int ShapoidGetDim(Shapoid *that) {
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
inline
#endif 
ShapoidType ShapoidGetType(Shapoid *that) {
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
inline
#endif 
const char* ShapoidGetTypeAsString(Shapoid *that) {
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

// Return a VecFloat equal to the position of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidGetPos(Shapoid *that) {
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

// Return a VecFloat equal to the 'dim'-th axis of the Shapoid
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidGetAxis(Shapoid *that, int dim) {
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

// Set the position of the Shapoid to 'pos'
#if BUILDMODE != 0
inline
#endif 
void ShapoidSetPos(Shapoid *that, VecFloat *pos) {
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
  if (VecDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%d)", 
      VecDim(pos), that->_dim);
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the position
  VecCopy(that->_pos, pos);
}

// Set the 'dim'-th axis of the Shapoid to 'v'
#if BUILDMODE != 0
inline
#endif 
void ShapoidSetAxis(Shapoid *that, int dim, VecFloat *v) {
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
  if (VecDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%d)", 
      dim, VecDim(v));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Set the axis
  VecCopy(that->_axis[dim], v);
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Translate the Shapoid by 'v'
#if BUILDMODE != 0
inline
#endif 
void ShapoidTranslate(Shapoid *that, VecFloat *v) {
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
  if (VecDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%d)", 
      that->_dim, VecDim(v));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Translate the position
  VecOp(that->_pos, 1.0, v, 1.0);
}

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
#if BUILDMODE != 0
inline
#endif 
void ShapoidScaleVector(Shapoid *that, VecFloat *v) {
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
  if (VecDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%d)", 
      that->_dim, VecDim(v));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Scale each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecScale(that->_axis[iAxis], VecGet(v, iAxis));
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Scale the Shapoid by 'c'
#if BUILDMODE != 0
inline
#endif 
void ShapoidScaleScalar(Shapoid *that, float c) {
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
}

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// and translate the Shapoid such as its center after scaling
// is at the same position than before scaling
#if BUILDMODE != 0
inline
#endif 
void ShapoidGrowVector(Shapoid *that, VecFloat *v) {
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
  if (VecDim(v) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'v' 's dimension is invalid (%d==%d)", 
      that->_dim, VecDim(v));
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
  // Else, the shapoid is not a spheroid
  } else {
    // Memorize the center
    VecFloat *centerA = ShapoidGetCenter(that);
    // Scale
    ShapoidScale(that, v);
    // Reposition to keep center at the same position
    VecFloat *centerB = ShapoidGetCenter(that);
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
inline
#endif 
void ShapoidGrowScalar(Shapoid *that, float c) {
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
  // Else, the shapoid is not a spheroid
  } else {
    // Memorize the center
    VecFloat *centerA = ShapoidGetCenter(that);
    // Scale
    ShapoidScale(that, c);
    // Reposition to keep center at the same position
    VecFloat *centerB = ShapoidGetCenter(that);
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
inline
#endif 
void ShapoidRotate2D(Shapoid *that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
  if (that->_dim != 2) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'that' 's dimension is invalid (%d==2)", 
      that->_dim);
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
    VecFloat *center = ShapoidGetCenter(that);
    // Rotate each axis
    for (int iAxis = that->_dim; iAxis--;)
      VecRot(that->_axis[iAxis], theta);
    // Reposition the origin
    VecFloat *v = VecGetOp(that->_pos, 1.0, center, -1.0);
    VecRot(v, theta);
    VecOp(v, 1.0, center, 1.0);
    VecCopy(that->_pos, v);
    VecFree(&center);
    VecFree(&v);
  }
  // Update the SysLinEq
  ShapoidUpdateSysLinEqImport(that);
}

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidImportCoord(Shapoid *that, VecFloat *pos) {
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
  if (VecDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%d)", 
      that->_dim, VecDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Update the system solver for the requested position
  VecFloat *v = VecGetOp(pos, 1.0, that->_pos, -1.0);
  SysLinEqSetV(that->_sysLinEqImport, v);
  // Solve the system
  VecFloat *res = SysLinEqSolve(that->_sysLinEqImport);
  // Free memory
  VecFree(&v);
  // return the result
  return res;
}

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidExportCoord(Shapoid *that, VecFloat *pos) {
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
  if (VecDim(pos) != that->_dim) {
    ShapoidErr->_type = PBErrTypeInvalidArg;
    sprintf(ShapoidErr->_msg, "'pos' 's dimension is invalid (%d==%d)", 
      that->_dim, VecDim(pos));
    PBErrCatch(ShapoidErr);
  }
#endif
  // Allocate memory for the result
  VecFloat *res = VecClone(that->_pos);
  for (int dim = that->_dim; dim--;)
    VecOp(res, 1.0, that->_axis[dim], VecGet(pos, dim));
  // Return the result
  return res;
}

// Get the center of the shapoid in standard coordinate system
#if BUILDMODE != 0
inline
#endif 
VecFloat* ShapoidGetCenter(Shapoid *that) {
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
  // Declare a variable to memorize the result in Shapoid 
  // coordinate system
  VecFloat *coord = VecFloatCreate(ShapoidGetDim(that));
  // If we could allocate memory
  if (coord != NULL) {
    // For each dimension
    for (int dim = ShapoidGetDim(that); dim--;) {
      if (ShapoidGetType(that) == ShapoidTypeFacoid)
        VecSet(coord, dim, 0.5);
      else if (ShapoidGetType(that) == ShapoidTypePyramidoid)
        VecSet(coord, dim, 1.0 / (1.0 + ShapoidGetDim(that)));
      else if (ShapoidGetType(that) == ShapoidTypeSpheroid)
        VecSet(coord, dim, 0.0);
    }
  }
  // Convert the coordinates in standard coordinate system
  VecFloat *res = ShapoidExportCoord(that, coord);
  // Free memory
  VecFree(&coord);
  // Return the result
  return res;
}

// Check if shapoid 'that' and 'tho' are equals
#if BUILDMODE != 0
inline
#endif 
bool ShapoidIsEqual(Shapoid *that, Shapoid *tho) {
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
  // Return the success code
  return true;
}

// Update the system of linear equation used to import coordinates
#if BUILDMODE != 0
inline
#endif 
void ShapoidUpdateSysLinEqImport(Shapoid *that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ShapoidErr->_type = PBErrTypeNullPointer;
    sprintf(ShapoidErr->_msg, "'that' is null");
    PBErrCatch(ShapoidErr);
  }
#endif
  VecShort2D dim = VecShortCreateStatic2D();
  // Set a pointer to the matrix in the SysLinEq
  MatFloat *mat = MatClone(that->_sysLinEqImport->_M);
  // Set the values of the matrix
  for (VecSet(&dim, 0, 0); VecGet(&dim, 0) < that->_dim; 
    VecSet(&dim, 0, VecGet(&dim, 0) + 1)) {
    for (VecSet(&dim, 1, 0); VecGet(&dim, 1) < that->_dim; 
      VecSet(&dim, 1, VecGet(&dim, 1) + 1)) {
      MatSet(mat, &dim, VecGet(that->_axis[VecGet(&dim, 0)], 
        VecGet(&dim, 1)));
    }
  }
  // Update the SysLinEq
  SysLinEqSetM(that->_sysLinEqImport, mat);
  // Free memory
  MatFree(&mat);
}

