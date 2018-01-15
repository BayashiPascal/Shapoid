#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "pberr.h"
#include "shapoid.h"

#define RANDOMSEED 0

void UnitTestCreateCloneIsEqualFree() {
  int dim = 3;
  Shapoid *facoid = ShapoidCreate(dim, ShapoidTypeFacoid);
  if (facoid == NULL || facoid->_dim != dim ||
    facoid->_type != ShapoidTypeFacoid || facoid->_pos == NULL ||
    VecDim(facoid->_pos) != dim || facoid->_sysLinEqImport == NULL ||
    facoid->_axis == NULL) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidCreate failed");
    PBErrCatch(ShapoidErr);
  }
  for (int iDim = dim; iDim--;) {
    if (ISEQUALF(VecGet(facoid->_pos, iDim), 0.0) == false ||
      facoid->_axis[iDim] == NULL || 
      VecDim(facoid->_axis[iDim]) != dim) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidCreate failed");
      PBErrCatch(ShapoidErr);
    }
    for (int jDim = dim; jDim--;) {
      if ((iDim == jDim && 
        ISEQUALF(VecGet(facoid->_axis[iDim], jDim), 1.0) == false) ||
        (iDim != jDim && 
        ISEQUALF(VecGet(facoid->_axis[iDim], jDim), 0.0) == false)) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidCreate failed");
        PBErrCatch(ShapoidErr);
      }
    }
    if (ISEQUALF(VecGet(facoid->_sysLinEqImport->_V, iDim), 
      0.0) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidCreate failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecShort2D u = VecShortCreateStatic2D();
  VecSet(&u, 0, dim); VecSet(&u, 1, dim);
  VecShort2D v = VecShortCreateStatic2D();
  do {
    if ((VecGet(&v, 0) == VecGet(&v, 1) && 
      ISEQUALF(MatGet(facoid->_sysLinEqImport->_M, &v), 1.0) == false) ||
      (VecGet(&v, 0) != VecGet(&v, 1) && 
      ISEQUALF(MatGet(facoid->_sysLinEqImport->_M, &v), 0.0) == false) ||
      (VecGet(&v, 0) == VecGet(&v, 1) && 
      ISEQUALF(MatGet(facoid->_sysLinEqImport->_Minv, &v), 
        1.0) == false) ||
      (VecGet(&v, 0) != VecGet(&v, 1) && 
      ISEQUALF(MatGet(facoid->_sysLinEqImport->_Minv, &v), 
        0.0) == false)) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidCreate failed");
      PBErrCatch(ShapoidErr);
    }
  } while (VecStep(&v, &u));
  Shapoid *clone = ShapoidClone(facoid);
  if (facoid->_dim != clone->_dim ||
    facoid->_type != clone->_type ||
    VecIsEqual(facoid->_pos, clone->_pos) == false ||
    MatIsEqual(facoid->_sysLinEqImport->_M, 
      clone->_sysLinEqImport->_M) == false ||
    MatIsEqual(facoid->_sysLinEqImport->_Minv, 
      clone->_sysLinEqImport->_Minv) == false ||
    VecIsEqual(facoid->_sysLinEqImport->_V, 
      clone->_sysLinEqImport->_V) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidClone failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    if (VecIsEqual(facoid->_axis[i], clone->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidClone failed");
      PBErrCatch(ShapoidErr);
    }
  }
  if (ShapoidIsEqual(facoid, clone) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  clone->_type = ShapoidTypePyramidoid;
  if (ShapoidIsEqual(facoid, clone) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  clone->_type = facoid->_type;
  clone->_dim = dim + 1;
  if (ShapoidIsEqual(facoid, clone) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  clone->_dim = facoid->_dim;
  VecSet(clone->_pos, 0, 1.0);
  if (ShapoidIsEqual(facoid, clone) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(clone->_pos, 0, 0.0);
  VecSet(clone->_axis[0], 0, 2.0);
  if (ShapoidIsEqual(facoid, clone) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(clone->_axis[0], 0, 1.0);
  ShapoidFree(&facoid);
  if (facoid != NULL) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidFree failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&clone);
  printf("UnitTestCreateCloneIsEqualFree OK\n");
}

void UnitTestLoadSavePrint() {
  int dim = 3;
  Shapoid *facoid = FacoidCreate(dim);
  FILE *file = fopen("./facoid.txt", "w");
  if (ShapoidSave(facoid, file) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidSave failed");
    PBErrCatch(ShapoidErr);
  }
  fclose(file);
  file = fopen("./facoid.txt", "r");
  Shapoid *load = NULL;
  if (ShapoidLoad(&load, file) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidLoad failed");
    PBErrCatch(ShapoidErr);
  }
  fclose(file);
  if (ShapoidIsEqual(facoid, load) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidLoad/Save failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidPrintln(facoid, stdout);
  ShapoidFree(&facoid);
  ShapoidFree(&load);
  printf("UnitTestLoadSavePrint OK\n");
}

void UnitTestGetSetTypeDimPosAxis() {
  int dim = 3;
  Shapoid *facoid = FacoidCreate(dim);
  Shapoid *pyramidoid = PyramidoidCreate(dim);
  Shapoid *spheroid = SpheroidCreate(dim);
  if (ShapoidGetType(facoid) != ShapoidTypeFacoid ||
    ShapoidGetType(pyramidoid) != ShapoidTypePyramidoid ||
    ShapoidGetType(spheroid) != ShapoidTypeSpheroid) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetType failed");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidGetDim(facoid) != dim) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetDim failed");
    PBErrCatch(ShapoidErr);
  }
  VecFloat *v = VecFloatCreate(dim);
  VecFloat *u = ShapoidGetPos(facoid);
  if (VecIsEqual(v, u) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetPos failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&u);
  for (int i = dim; i--;) {
    u = ShapoidGetAxis(facoid, i);
    for (int j = dim; j--;)
      if ((i == j && ISEQUALF(VecGet(u, j), 1.0) == false) ||
        (i != j && ISEQUALF(VecGet(u, j), 0.0) == false)) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidGetAxis failed");
        PBErrCatch(ShapoidErr);
      }
    VecFree(&u);
  }
  for (int i = dim; i--;)
    VecSet(v, i, (float)i);
  ShapoidSetPos(facoid, v);
  if (VecIsEqual(v, facoid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidSetPos failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    VecSetNull(v);
    VecSet(v, i, 2.0);
    ShapoidSetAxis(facoid, i, v);
    if (VecIsEqual(v, facoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidSetAxis failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&v);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestGetSetTypeDimPosAxis OK\n");
}

void UnitTestTranslateScaleGrowRotate() {
  int dim = 2;
  Shapoid *facoid = FacoidCreate(dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int i = dim; i--;)
    VecSet(v, i, 1.0);
  ShapoidTranslate(facoid, v);
  if (VecIsEqual(v, facoid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidTranslate failed");
    PBErrCatch(ShapoidErr);
  }
  float scale = 2.0;
  ShapoidScale(facoid, scale);
  VecSetNull(v);
  VecSetNull(facoid->_pos);
  if (VecIsEqual(v, facoid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidScaleScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, facoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidScaleScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  for (int i = dim; i--;)
    VecSet(v, i, 1.0 + (float)i);
  ShapoidScale(facoid, v);
  VecSetNull(v);
  if (VecIsEqual(v, facoid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidScaleVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale * (1.0 + (float)i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, facoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidScaleVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  facoid = FacoidCreate(dim);
  scale = 2.0;
  ShapoidGrow(facoid, scale);
  for (int i = dim; i--;)
    VecSet(v, i, -0.5);
  if (VecIsEqual(v, facoid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGrowScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, facoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGrowScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Shapoid *pyramidoid = PyramidoidCreate(dim);
  VecFloat *centerA = ShapoidGetCenter(pyramidoid);
  ShapoidGrow(pyramidoid, scale);
  VecFloat *centerB = ShapoidGetCenter(pyramidoid);
  if (VecIsEqual(centerA, centerB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGrowScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, pyramidoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGrowScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&centerA);
  VecFree(&centerB);
  Shapoid *spheroid = SpheroidCreate(dim);
  ShapoidGrow(spheroid, scale);
  VecSetNull(v);
  if (VecIsEqual(v, spheroid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGrowScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, spheroid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGrowScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFloat *scalev = VecFloatCreate(dim);
  for (int i = dim; i--;)
    VecSet(scalev, i, 1.0 + (float)i);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  facoid = FacoidCreate(dim);
  ShapoidGrow(facoid, scalev);
  float pa[2] = {0.000,-0.500}; 
  for (int i = dim; i--;)
    VecSet(v, i, pa[i]);
  if (VecIsEqual(v, facoid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGrowVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, VecGet(scalev, i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, facoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGrowVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  pyramidoid = PyramidoidCreate(dim);
  centerA = ShapoidGetCenter(pyramidoid);
  ShapoidGrow(pyramidoid, scalev);
  centerB = ShapoidGetCenter(pyramidoid);
  if (VecIsEqual(centerA, centerB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGrowVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, VecGet(scalev, i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, pyramidoid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGrowVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&centerA);
  VecFree(&centerB);
  spheroid = SpheroidCreate(dim);
  ShapoidGrow(spheroid, scalev);
  VecSetNull(v);
  if (VecIsEqual(v, spheroid->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGrowVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, VecGet(scalev, i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, spheroid->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGrowVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&scalev);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  facoid = FacoidCreate(dim);
  pyramidoid = PyramidoidCreate(dim);
  spheroid = SpheroidCreate(dim);
  float theta = PBMATH_HALFPI;
  ShapoidRotate2D(facoid, theta);
  float pb[2] = {1.0, 0.0};
  float pc[2] = {0.0, 1.0};
  float pd[2] = {-1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(facoid->_pos, i), pb[i]) == false ||
      ISEQUALF(VecGet(facoid->_axis[0], i), pc[i]) == false ||
      ISEQUALF(VecGet(facoid->_axis[1], i), pd[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotate2D failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotate2D(pyramidoid, theta);
  float pe[2] = {0.6666667, 0.0};
  float pf[2] = {0.0, 1.0};
  float pg[2] = {-1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(pyramidoid->_pos, i), pe[i]) == false ||
      ISEQUALF(VecGet(pyramidoid->_axis[0], i), pf[i]) == false ||
      ISEQUALF(VecGet(pyramidoid->_axis[1], i), pg[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotate2D failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotate2D(spheroid, theta);
  float ph[2] = {0.0, 0.0};
  float pi[2] = {0.0, 1.0};
  float pj[2] = {-1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(spheroid->_pos, i), ph[i]) == false ||
      ISEQUALF(VecGet(spheroid->_axis[0], i), pi[i]) == false ||
      ISEQUALF(VecGet(spheroid->_axis[1], i), pj[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotate2D failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&v);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestTranslateScaleGrowRotate OK\n");
}

void UnitTestImportExportCoordIsPosInside() {
  int dim = 2;
  Shapoid *facoid = FacoidCreate(dim);
  Shapoid *pyramidoid = PyramidoidCreate(dim);
  Shapoid *spheroid = SpheroidCreate(dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int i = dim; i--;)
    VecSet(v, i, 1.0 + (float)i);
  ShapoidTranslate(facoid, v);
  ShapoidTranslate(pyramidoid, v);
  ShapoidTranslate(spheroid, v);
  float scale = -2.0;
  ShapoidScale(facoid, scale);
  ShapoidScale(pyramidoid, scale);
  ShapoidScale(spheroid, scale);
  int nbTest = 100;
  srandom(RANDOMSEED);
  for (int iTest = nbTest; iTest--;) {
    VecFloat *posReal = VecFloatCreate(dim);
    for (int i = dim; i--;)
      VecSet(posReal, i, (rnd() - 0.5) * 10.0);
    VecFloat *posShapoidA = ShapoidImportCoord(facoid, posReal);
    bool isInside = ShapoidIsPosInside(facoid, posReal);
    if (VecGet(posShapoidA, 0) >= 0.0 && 
      VecGet(posShapoidA, 0) <= 1.0 &&
      VecGet(posShapoidA, 1) >= 0.0 && 
      VecGet(posShapoidA, 1) <= 1.0) {
      if (isInside == false) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidIsPosInside failed");
        PBErrCatch(ShapoidErr);
      }
    } else {
      if (isInside == true) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidIsPosInside failed");
        PBErrCatch(ShapoidErr);
      }
    }
    VecOp(posShapoidA, scale, v, 1.0);
    if (VecIsEqual(posReal, posShapoidA) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidImportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posShapoidA);
    VecFloat *posShapoidB = ShapoidImportCoord(pyramidoid, posReal);
    isInside = ShapoidIsPosInside(pyramidoid, posReal);
    if (VecGet(posShapoidB, 0) >= 0.0 && 
      VecGet(posShapoidB, 0) <= 1.0 &&
      VecGet(posShapoidB, 1) >= 0.0 && 
      VecGet(posShapoidB, 1) <= 1.0 &&
      VecGet(posShapoidB, 0) + VecGet(posShapoidB, 1) <= 1.0) {
      if (isInside == false) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidIsPosInside failed");
        PBErrCatch(ShapoidErr);
      }
    } else {
      if (isInside == true) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidIsPosInside failed");
        PBErrCatch(ShapoidErr);
      }
    }
    VecOp(posShapoidB, scale, v, 1.0);
    if (VecIsEqual(posReal, posShapoidB) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidImportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posShapoidB);
    VecFloat *posShapoidC = ShapoidImportCoord(spheroid, posReal);
    isInside = ShapoidIsPosInside(spheroid, posReal);
    if (VecGet(posShapoidC, 0) >= -0.5 && 
      VecGet(posShapoidC, 0) <= 0.5 &&
      VecGet(posShapoidC, 1) >= -0.5 && 
      VecGet(posShapoidC, 1) <= 0.5 &&
      pow(VecGet(posShapoidC, 0), 2.0) + 
      pow(VecGet(posShapoidC, 1), 2.0) <= 0.25) {
      if (isInside == false) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidIsPosInside failed");
        PBErrCatch(ShapoidErr);
      }
    } else {
      if (isInside == true) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidIsPosInside failed");
        PBErrCatch(ShapoidErr);
      }
    }
    VecOp(posShapoidC, scale, v, 1.0);
    if (VecIsEqual(posReal, posShapoidC) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidImportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posShapoidC);
    VecFree(&posReal);
  }
  for (int iTest = nbTest; iTest--;) {
    VecFloat *posShapoid = VecFloatCreate(dim);
    for (int i = dim; i--;)
      VecSet(posShapoid, i, (rnd() - 0.5) * 10.0);
    VecFloat *posRealA = ShapoidExportCoord(facoid, posShapoid);
    VecOp(posRealA, 1.0, v, -1.0);
    VecScale(posRealA, 1.0 / scale);
    if (VecIsEqual(posRealA, posShapoid) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidExportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posRealA);
    VecFloat *posRealB = ShapoidExportCoord(pyramidoid, posShapoid);
    VecOp(posRealB, 1.0, v, -1.0);
    VecScale(posRealB, 1.0 / scale);
    if (VecIsEqual(posRealB, posShapoid) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidExportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posRealB);
    VecFloat *posRealC = ShapoidExportCoord(facoid, posShapoid);
    VecOp(posRealC, 1.0, v, -1.0);
    VecScale(posRealC, 1.0 / scale);
    if (VecIsEqual(posRealC, posShapoid) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidExportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posRealC);
    VecFree(&posShapoid);
  }
  VecFree(&v);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestImportExportCoordIsPosInside OK\n");
}

void UnitTestGetBoundingBox() {
  int dim = 2;
  Shapoid *facoid = FacoidCreate(dim);
  Shapoid *pyramidoid = PyramidoidCreate(dim);
  Shapoid *spheroid = SpheroidCreate(dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int i = dim; i--;)
    VecSet(v, i, 1.0 + (float)i);
  ShapoidTranslate(facoid, v);
  ShapoidTranslate(pyramidoid, v);
  ShapoidTranslate(spheroid, v);
  float scale = -2.0;
  ShapoidScale(facoid, scale);
  ShapoidScale(pyramidoid, scale);
  ShapoidScale(spheroid, scale);
  float theta = PBMATH_QUARTERPI;
  ShapoidRotate2D(facoid, theta);
  ShapoidRotate2D(pyramidoid, theta);
  ShapoidRotate2D(spheroid, theta);
  Shapoid *boundA = ShapoidGetBoundingBox(facoid);
  float pa[2] = {-1.414214, -0.414213};
  float pb[2] = {2.828427, 0.0};
  float pc[2] = {0.0, 2.828427};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(boundA->_pos, i), pa[i]) == false ||
      ISEQUALF(VecGet(boundA->_axis[0], i), pb[i]) == false ||
      ISEQUALF(VecGet(boundA->_axis[1], i), pc[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Shapoid *boundB = ShapoidGetBoundingBox(pyramidoid);
  float pd[2] = {-1.414214, -1.4142143};
  float pe[2] = {2.828427, 0.0};
  float pf[2] = {0.0, 3.690356};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(boundB->_pos, i), pd[i]) == false ||
      ISEQUALF(VecGet(boundB->_axis[0], i), pe[i]) == false ||
      ISEQUALF(VecGet(boundB->_axis[1], i), pf[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Shapoid *boundC = ShapoidGetBoundingBox(spheroid);
  float pg[2] = {-0.414214, 0.585786};
  float ph[2] = {2.828427, 0.0};
  float pi[2] = {0.0, 2.828427};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(boundC->_pos, i), pg[i]) == false ||
      ISEQUALF(VecGet(boundC->_axis[0], i), ph[i]) == false ||
      ISEQUALF(VecGet(boundC->_axis[1], i), pi[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  GSet set = GSetCreateStatic();
  GSetPush(&set, facoid);
  GSetPush(&set, pyramidoid);
  GSetPush(&set, spheroid);
  Shapoid *boundD = ShapoidGetBoundingBox(&set);
  float pj[2] = {-1.414214, -1.4142143};
  float pk[2] = {3.828427, 0.0};
  float pl[2] = {0.0, 4.828427};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(boundD->_pos, i), pj[i]) == false ||
      ISEQUALF(VecGet(boundD->_axis[0], i), pk[i]) == false ||
      ISEQUALF(VecGet(boundD->_axis[1], i), pl[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  GSetFlush(&set);
  ShapoidFree(&boundA);
  ShapoidFree(&boundB);
  ShapoidFree(&boundC);
  ShapoidFree(&boundD);
  VecFree(&v);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestGetBoundingBox OK\n");
}

void UnitTestGetPosDepthCenterCoverage() {
  int dim = 2;
  Shapoid *facoid = FacoidCreate(dim);
  Shapoid *pyramidoid = PyramidoidCreate(dim);
  Shapoid *spheroid = SpheroidCreate(dim);
  VecFloat *center = ShapoidGetCenter(facoid);
  if (ISEQUALF(VecGet(center, 0), 0.5) == false ||
    ISEQUALF(VecGet(center, 1), 0.5) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCenter failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&center);
  center = ShapoidGetCenter(pyramidoid);
  if (ISEQUALF(VecGet(center, 0), 0.333333) == false ||
    ISEQUALF(VecGet(center, 1), 0.333333) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCenter failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&center);
  center = ShapoidGetCenter(spheroid);
  if (ISEQUALF(VecGet(center, 0), 0.0) == false ||
    ISEQUALF(VecGet(center, 1), 0.0) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCenter failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&center);
  float coverage = ShapoidGetCoverageDelta(facoid, pyramidoid, 0.001);
  if (ISEQUALF(coverage, 1.0) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCoverage failed");
    PBErrCatch(ShapoidErr);
  }
  coverage = ShapoidGetCoverageDelta(pyramidoid, facoid, 0.001);
  if (ISEQUALF(coverage, 0.500499) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCoverage failed");
    PBErrCatch(ShapoidErr);
  }
  coverage = ShapoidGetCoverageDelta(pyramidoid, spheroid, 0.001);
  if (ISEQUALF(coverage, 0.24937) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCoverage failed");
    PBErrCatch(ShapoidErr);
  }
  coverage = ShapoidGetCoverageDelta(spheroid, pyramidoid, 0.001);
  if (ISEQUALF(coverage, 0.39251) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCoverage failed");
    PBErrCatch(ShapoidErr);
  }
  coverage = ShapoidGetCoverageDelta(facoid, spheroid, 0.001);
  if (ISEQUALF(coverage, 0.24937) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCoverage failed");
    PBErrCatch(ShapoidErr);
  }
  coverage = ShapoidGetCoverageDelta(spheroid, facoid, 0.001);
  if (ISEQUALF(coverage, 0.196451) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetCoverage failed");
    PBErrCatch(ShapoidErr);
  }
  VecFloat2D pos = VecFloatCreateStatic2D();
  VecSet(&pos, 0, 0.333333); VecSet(&pos, 1, 0.333333);
  float depth = ShapoidGetPosDepth(facoid, (VecFloat*)&pos);
  if (ISEQUALF(depth, 0.790123) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetPosDepth failed");
    PBErrCatch(ShapoidErr);
  }
  depth = ShapoidGetPosDepth(pyramidoid, (VecFloat*)&pos);
  if (ISEQUALF(depth, 1.0) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetPosDepth failed");
    PBErrCatch(ShapoidErr);
  }
  depth = ShapoidGetPosDepth(spheroid, (VecFloat*)&pos);
  if (ISEQUALF(depth, 0.057192) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetPosDepth failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestGetPosDepthCenterCoverage OK\n");
}

void UnitTestAll() {
  UnitTestCreateCloneIsEqualFree();
  UnitTestLoadSavePrint();
  UnitTestGetSetTypeDimPosAxis();
  UnitTestTranslateScaleGrowRotate();
  UnitTestImportExportCoordIsPosInside();
  UnitTestGetBoundingBox();
  UnitTestGetPosDepthCenterCoverage();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

