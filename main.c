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
  Shapoid* facoid = ShapoidCreate(dim, ShapoidTypeFacoid);
  if (facoid == NULL || facoid->_dim != dim ||
    facoid->_type != ShapoidTypeFacoid || facoid->_pos == NULL ||
    VecGetDim(facoid->_pos) != dim || facoid->_sysLinEqImport == NULL ||
    facoid->_axis == NULL) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidCreate failed");
    PBErrCatch(ShapoidErr);
  }
  for (int iDim = dim; iDim--;) {
    if (ISEQUALF(VecGet(facoid->_pos, iDim), 0.0) == false ||
      facoid->_axis[iDim] == NULL || 
      VecGetDim(facoid->_axis[iDim]) != dim) {
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
  Shapoid* clone = ShapoidClone(facoid);
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
  *(ShapoidType*)&(clone->_type) = ShapoidTypePyramidoid;
  if (ShapoidIsEqual(facoid, clone) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  *(ShapoidType*)&(clone->_type) = facoid->_type;
  *(int*)&(clone->_dim) = dim + 1;
  if (ShapoidIsEqual(facoid, clone) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsEqual failed");
    PBErrCatch(ShapoidErr);
  }
  *(int*)&(clone->_dim) = facoid->_dim;
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
  Facoid* facoid = FacoidCreate(dim);
  FILE* file = fopen("./facoid.txt", "w");
  if (ShapoidSave(facoid, file, false) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidSave failed");
    PBErrCatch(ShapoidErr);
  }
  fclose(file);
  file = fopen("./facoid.txt", "r");
  Facoid* load = FacoidCreate(dim);
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
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
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
  VecFloat* v = VecFloatCreate(dim);
  VecFloat* u = ShapoidGetPos(facoid);
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
  if (VecIsEqual(v, ShapoidPos(facoid)) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidSetPos failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    VecSetNull(v);
    VecSet(v, i, 2.0);
    ShapoidSetAxis(facoid, i, v);
    if (VecIsEqual(v, ShapoidAxis(facoid, i)) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidSetAxis failed");
      PBErrCatch(ShapoidErr);
    }
  }
  for (int i = dim; i--;)
    VecSet(v, i, i);
  ShapoidSetCenterPos(facoid, v);
  VecFloat* center = ShapoidGetCenter(facoid);
  if (VecIsEqual(v, center) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidSetCenterPos failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&center);
  VecSet(v, 0, 1.0); VecSet(v, 1, 2.0); VecSet(v, 2, 0.5);
  ShapoidScale(spheroid, v); 
  if (!ISEQUALF(ShapoidGetBoundingRadius(spheroid), 1.0)) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidGetBoundingRadius failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&v);
  ShapoidPosSet(facoid, 1, -1.0);
  if (!ISEQUALF(((Shapoid*)facoid)->_pos->_val[1], -1.0)) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidPosSet failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidPosSetAdd(facoid, 1, -1.0);
  if (!ISEQUALF(((Shapoid*)facoid)->_pos->_val[1], -2.0)) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidPosSet failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidAxisSet(facoid, 2, 1, -1.0);
  if (!ISEQUALF(((Shapoid*)facoid)->_axis[2]->_val[1], -1.0)) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidAxisSet failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidAxisSetAdd(facoid, 2, 1, -1.0);
  if (!ISEQUALF(((Shapoid*)facoid)->_axis[2]->_val[1], -2.0)) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidAxisSetAdd failed");
    PBErrCatch(ShapoidErr);
  }
  VecFloat* w = ShapoidGetAxis(facoid, 2);
  VecScale(w, 2.0);
  ShapoidAxisScale(facoid, 2, 2.0);
  if (!VecIsEqual(ShapoidAxis(facoid, 2), w)) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidAxisScale failed");
    PBErrCatch(ShapoidErr);
  }
  VecFree(&w);
  for (int i = dim; i--;) {
    if (!ISEQUALF(ShapoidPosGet(facoid, i), 
      VecGet(ShapoidGetPos(facoid), i))) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidPosGet failed");
      PBErrCatch(ShapoidErr);
    }
    for (int j = dim; j--;) {
      if (!ISEQUALF(ShapoidAxisGet(facoid, i, j), 
        VecGet(ShapoidGetAxis(facoid, i), j))) {
        ShapoidErr->_type = PBErrTypeUnitTestFailed;
        sprintf(ShapoidErr->_msg, "ShapoidAxisGet failed");
        PBErrCatch(ShapoidErr);
      }
    }
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestGetSetTypeDimPosAxis OK\n");
}

void UnitTestTranslateScaleGrow() {
  int dim = 2;
  Facoid* facoid = FacoidCreate(dim);
  VecFloat* v = VecFloatCreate(dim);
  for (int i = dim; i--;)
    VecSet(v, i, 1.0);
  ShapoidTranslate(facoid, v);
  if (VecIsEqual(v, ((Shapoid*)facoid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidTranslate failed");
    PBErrCatch(ShapoidErr);
  }
  float scale = 2.0;
  ShapoidScale(facoid, scale);
  VecSetNull(v);
  VecSetNull(((Shapoid*)facoid)->_pos);
  if (VecIsEqual(v, ((Shapoid*)facoid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidScaleScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)facoid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidScaleScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  for (int i = dim; i--;)
    VecSet(v, i, 1.0 + (float)i);
  ShapoidScale(facoid, v);
  VecSetNull(v);
  if (VecIsEqual(v, ((Shapoid*)facoid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidScaleVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale * (1.0 + (float)i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)facoid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidScaleVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  facoid = FacoidCreate(dim);
  scale = 2.0;
  ShapoidGrow(facoid, scale);
  for (int i = dim; i--;)
    VecSet(v, i, -0.5);
  if (VecIsEqual(v, ((Shapoid*)facoid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidGrowScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)facoid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidGrowScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  VecFloat* centerA = ShapoidGetCenter(pyramidoid);
  ShapoidGrow(pyramidoid, scale);
  VecFloat* centerB = ShapoidGetCenter(pyramidoid);
  if (VecIsEqual(centerA, centerB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidGrowScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)pyramidoid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidGrowScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&centerA);
  VecFree(&centerB);
  Spheroid* spheroid = SpheroidCreate(dim);
  ShapoidGrow(spheroid, scale);
  VecSetNull(v);
  if (VecIsEqual(v, ((Shapoid*)spheroid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidGrowScalar failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, scale);
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)spheroid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidGrowScalar failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFloat* scalev = VecFloatCreate(dim);
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
  if (VecIsEqual(v, ((Shapoid*)facoid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidGrowVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, VecGet(scalev, i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)facoid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidGrowVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  pyramidoid = PyramidoidCreate(dim);
  centerA = ShapoidGetCenter(pyramidoid);
  ShapoidGrow(pyramidoid, scalev);
  centerB = ShapoidGetCenter(pyramidoid);
  if (VecIsEqual(centerA, centerB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidGrowVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, VecGet(scalev, i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)pyramidoid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidGrowVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&centerA);
  VecFree(&centerB);
  spheroid = SpheroidCreate(dim);
  ShapoidGrow(spheroid, scalev);
  VecSetNull(v);
  if (VecIsEqual(v, ((Shapoid*)spheroid)->_pos) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "_ShapoidGrowVector failed");
    PBErrCatch(ShapoidErr);
  }
  for (int i = dim; i--;) {
    for (int j = dim; j--;)
      if (i == j)
        VecSet(v, j, VecGet(scalev, i));
      else
        VecSet(v, j, 0.0);
    if (VecIsEqual(v, ((Shapoid*)spheroid)->_axis[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "_ShapoidGrowVector failed");
      PBErrCatch(ShapoidErr);
    }
  }
  VecFree(&scalev);
  VecFree(&v);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestTranslateScaleGrow OK\n");
}

void UnitTestRotate() {
  int dim = 2;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  float theta = PBMATH_HALFPI;
  ShapoidRotCenter(facoid, theta);
  float pb[2] = {1.0, 0.0};
  float pc[2] = {0.0, 1.0};
  float pd[2] = {-1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pb[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pc[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pd[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotCenter(pyramidoid, theta);
  float pe[2] = {0.6666667, 0.0};
  float pf[2] = {0.0, 1.0};
  float pg[2] = {-1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pe[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      pf[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pg[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotCenter(spheroid, theta);
  float ph[2] = {0.0, 0.0};
  float pi[2] = {0.0, 1.0};
  float pj[2] = {-1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      ph[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pi[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pj[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotOrigin(facoid, theta);
  float pk[2] = {0.0, 1.0};
  float pl[2] = {-1.0, 0.0};
  float pm[2] = {0.0, -1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pk[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pl[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pm[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotOrigin(pyramidoid, theta);
  float pn[2] = {0.0, 0.6666667};
  float po[2] = {-1.0, 0.0};
  float pp[2] = {0.0, -1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pn[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      po[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pp[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotOrigin(spheroid, theta);
  float pq[2] = {0.0, 0.0};
  float pr[2] = {-1.0, 0.0};
  float ps[2] = {0.0, -1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pq[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pr[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      ps[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotStart(facoid, theta);
  float pt[2] = {0.0, 1.0};
  float pu[2] = {0.0, -1.0};
  float pv[2] = {1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pt[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pu[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pv[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotStart(pyramidoid, theta);
  float pw[2] = {0.0, 0.6666667};
  float px[2] = {0.0, -1.0};
  float py[2] = {1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pw[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      px[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      py[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotStart(spheroid, theta);
  float pz[2] = {0.0, 0.0};
  float paa[2] = {0.0, -1.0};
  float pab[2] = {1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pz[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      paa[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pab[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestRotate OK\n");
}

void UnitTestRotateAxis() {
  int dim = 3;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  float theta = PBMATH_HALFPI;
  VecFloat3D axis = VecFloatCreateStatic3D();
  VecSet(&axis, 0, 1.0); VecSet(&axis, 1, 1.0); VecSet(&axis, 2, 1.0); 
  VecNormalise(&axis);
  ShapoidRotAxisCenter(facoid, &axis, theta);
  float pb[3] = {0.0, 0.0, 0.0};
  float pc[3] = {0.333333, 0.910684, -0.244017};
  float pd[3] = {-0.244017, 0.333333, 0.910684};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pb[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pc[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pd[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisCenter(pyramidoid, &axis, theta);
  float pe[3] = {0.000000, 0.000000, 0.000000};
  float pf[3] = {0.333333, 0.910684, -0.244017};
  float pg[3] = {-0.244017, 0.333333, 0.910684};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pe[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      pf[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pg[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisCenter(spheroid, &axis, theta);
  float ph[3] = {0.0, 0.0, 0.0};
  float pi[3] = {0.333333, 0.910684, -0.244017};
  float pj[3] = {-0.244017, 0.333333, 0.910684};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      ph[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pi[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pj[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisOrigin(facoid, &axis, theta);
  float pk[3] = {0.0, 0.0, 0.0};
  float pl[3] = {-0.333333, 0.666667, 0.666667};
  float pm[3] = {0.666667, -0.333333, 0.666667};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pk[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pl[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pm[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisOrigin(pyramidoid, &axis, theta);
  float pn[3] = {0.0, 0.0, 0.0};
  float po[3] = {-0.333333, 0.666667, 0.666667};
  float pp[3] = {0.666667, -0.333333, 0.666667};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pn[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      po[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pp[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisOrigin(spheroid, &axis, theta);
  float pq[3] = {0.0, 0.0, 0.0};
  float pr[3] = {-0.333333, 0.666667, 0.666667};
  float ps[3] = {0.666667, -0.333333, 0.666667};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pq[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pr[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      ps[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisStart(facoid, &axis, theta);
  float pt[3] = {0.0, 0.0, 0.0};
  float pu[3] = {0.333333, -0.244017, 0.910683};
  float pv[3] = {0.910683, 0.333333, -0.244017};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pt[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pu[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pv[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisStart(pyramidoid, &axis, theta);
  float pw[3] = {0.0, 0.0, 0.0};
  float px[3] = {0.333333, -0.244017, 0.910683};
  float py[3] = {0.910683, 0.333333, -0.244017};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pw[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      px[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      py[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotAxisStart(spheroid, &axis, theta);
  float pz[3] = {0.0, 0.0, 0.0};
  float paa[3] = {0.333333, -0.244017, 0.910683};
  float pab[3] = {0.910683, 0.333333, -0.244017};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pz[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      paa[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pab[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotAxisStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestRotateAxis OK\n");
}

void UnitTestRotateX() {
  int dim = 3;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  float theta = PBMATH_HALFPI;
  ShapoidRotXCenter(facoid, theta);
  float pb[3] = {0.0, 1.0, 0.0};
  float pc[3] = {1.0, 0.0, 0.0};
  float pd[3] = {0.0, 0.0, 1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pb[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pc[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pd[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXCenter(pyramidoid, theta);
  float pe[3] = {0.0, 0.5, 0.0};
  float pf[3] = {1.0, 0.0, 0.0};
  float pg[3] = {0.0, 0.0, 1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pe[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      pf[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pg[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXCenter(spheroid, theta);
  float ph[3] = {0.0, 0.0, 0.0};
  float pi[3] = {1.0, 0.0, 0.0};
  float pj[3] = {0.0, 0.0, 1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      ph[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pi[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pj[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXOrigin(facoid, theta);
  float pk[3] = {0.0, 0.0, 1.0};
  float pl[3] = {1.0, 0.0, 0.0};
  float pm[3] = {0.0, -1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pk[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pl[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pm[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXOrigin(pyramidoid, theta);
  float pn[3] = {0.0, 0.0, 0.5};
  float po[3] = {1.0, 0.0, 0.0};
  float pp[3] = {0.0, -1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pn[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      po[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pp[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXOrigin(spheroid, theta);
  float pq[3] = {0.0, 0.0, 0.0};
  float pr[3] = {1.0, 0.0, 0.0};
  float ps[3] = {0.0, -1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pq[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pr[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      ps[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXStart(facoid, theta);
  float pt[3] = {0.0, 0.0, 1.0};
  float pu[3] = {1.0, 0.0, 0.0};
  float pv[3] = {0.0, 0.0, -1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pt[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pu[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pv[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXStart(pyramidoid, theta);
  float pw[3] = {0.0, 0.0, 0.5};
  float px[3] = {1.0, 0.0, 0.0};
  float py[3] = {0.0, 0.0, -1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pw[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      px[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      py[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotXStart(spheroid, theta);
  float pz[3] = {0.0, 0.0, 0.0};
  float paa[3] = {1.0, 0.0, 0.0};
  float pab[3] = {0.0, 0.0, -1.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pz[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      paa[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pab[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotXStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestRotateX OK\n");
}

void UnitTestRotateY() {
  int dim = 3;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  float theta = PBMATH_HALFPI;
  ShapoidRotYCenter(facoid, theta);
  float pb[3] = {0.0, 0.0, 1.0};
  float pc[3] = {0.0, 0.0, -1.0};
  float pd[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pb[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pc[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pd[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYCenter(pyramidoid, theta);
  float pe[3] = {0.0, 0.0, 0.5};
  float pf[3] = {0.0, 0.0, -1.0};
  float pg[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pe[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      pf[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pg[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYCenter(spheroid, theta);
  float ph[3] = {0.0, 0.0, 0.0};
  float pi[3] = {0.0, 0.0, -1.0};
  float pj[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      ph[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pi[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pj[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYOrigin(facoid, theta);
  float pk[3] = {1.0, 0.0, 0.0};
  float pl[3] = {-1.0, 0.0, 0.0};
  float pm[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pk[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pl[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pm[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYOrigin(pyramidoid, theta);
  float pn[3] = {0.5, 0.0, 0.0};
  float po[3] = {-1.0, 0.0, 0.0};
  float pp[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pn[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      po[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pp[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYOrigin(spheroid, theta);
  float pq[3] = {0.0, 0.0, 0.0};
  float pr[3] = {-1.0, 0.0, 0.0};
  float ps[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pq[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pr[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      ps[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYStart(facoid, theta);
  float pt[3] = {1.0, 0.0, 0.0};
  float pu[3] = {0.0, 0.0, 1.0};
  float pv[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pt[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pu[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pv[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYStart(pyramidoid, theta);
  float pw[3] = {0.5, 0.0, 0.0};
  float px[3] = {0.0, 0.0, 1.0};
  float py[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pw[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      px[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      py[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotYStart(spheroid, theta);
  float pz[3] = {0.0, 0.0, 0.0};
  float paa[3] = {0.0, 0.0, 1.0};
  float pab[3] = {0.0, 1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pz[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      paa[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pab[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotYStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestRotateY OK\n");
}

void UnitTestRotateZ() {
  int dim = 3;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  float theta = PBMATH_HALFPI;
  ShapoidRotZCenter(facoid, theta);
  float pb[3] = {1.0, 0.0, 0.0};
  float pc[3] = {0.0, 1.0, 0.0};
  float pd[3] = {-1.0, 0.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pb[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pc[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pd[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZCenter(pyramidoid, theta);
  float pe[3] = {0.5, 0.0, 0.0};
  float pf[3] = {0.0, 1.0, 0.0};
  float pg[3] = {-1.0, 0.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pe[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      pf[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pg[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZCenter(spheroid, theta);
  float ph[3] = {0.0, 0.0, 0.0};
  float pi[3] = {0.0, 1.0, 0.0};
  float pj[3] = {-1.0, 0.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      ph[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pi[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pj[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZCenter failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZOrigin(facoid, theta);
  float pk[3] = {0.0, 1.0, 0.0};
  float pl[3] = {-1.0, 0.0, 0.0};
  float pm[3] = {0.0, -1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pk[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pl[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pm[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZOrigin(pyramidoid, theta);
  float pn[3] = {0.0, 0.5, 0.0};
  float po[3] = {-1.0, 0.0, 0.0};
  float pp[3] = {0.0, -1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pn[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      po[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      pp[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZOrigin(spheroid, theta);
  float pq[3] = {0.0, 0.0, 0.0};
  float pr[3] = {-1.0, 0.0, 0.0};
  float ps[3] = {0.0, -1.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pq[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      pr[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      ps[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZOrigin failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZStart(facoid, theta);
  float pt[3] = {0.0, 1.0, 0.0};
  float pu[3] = {0.0, -1.0, 0.0};
  float pv[3] = {1.0, 0.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)facoid)->_pos, i), pt[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[0], i), 
      pu[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)facoid)->_axis[1], i), 
      pv[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZStart(pyramidoid, theta);
  float pw[3] = {0.0, 0.5, 0.0};
  float px[3] = {0.0, -1.0, 0.0};
  float py[3] = {1.0, 0.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_pos, i), 
      pw[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[0], i), 
      px[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)pyramidoid)->_axis[1], i), 
      py[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidRotZStart(spheroid, theta);
  float pz[3] = {0.0, 0.0, 0.0};
  float paa[3] = {0.0, -1.0, 0.0};
  float pab[3] = {1.0, 0.0, 0.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)spheroid)->_pos, i), 
      pz[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[0], i), 
      paa[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)spheroid)->_axis[1], i), 
      pab[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidRotZStart failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestRotateZ OK\n");
}

void UnitTestImportExportCoordIsPosInside() {
  int dim = 2;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  VecFloat* v = VecFloatCreate(dim);
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
    VecFloat* posReal = VecFloatCreate(dim);
    for (int i = dim; i--;)
      VecSet(posReal, i, (rnd() - 0.5) * 10.0);
    VecFloat* posShapoidA = ShapoidImportCoord(facoid, posReal);
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
    VecFloat* posShapoidB = ShapoidImportCoord(pyramidoid, posReal);
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
    VecFloat* posShapoidC = ShapoidImportCoord(spheroid, posReal);
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
    VecFloat* posShapoid = VecFloatCreate(dim);
    for (int i = dim; i--;)
      VecSet(posShapoid, i, (rnd() - 0.5) * 10.0);
    VecFloat* posRealA = ShapoidExportCoord(facoid, posShapoid);
    VecOp(posRealA, 1.0, v, -1.0);
    VecScale(posRealA, 1.0 / scale);
    if (VecIsEqual(posRealA, posShapoid) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidExportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posRealA);
    VecFloat* posRealB = ShapoidExportCoord(pyramidoid, posShapoid);
    VecOp(posRealB, 1.0, v, -1.0);
    VecScale(posRealB, 1.0 / scale);
    if (VecIsEqual(posRealB, posShapoid) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidExportCoord failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&posRealB);
    VecFloat* posRealC = ShapoidExportCoord(facoid, posShapoid);
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
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  VecFloat* v = VecFloatCreate(dim);
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
  ShapoidRotCenter(facoid, theta);
  ShapoidRotCenter(pyramidoid, theta);
  ShapoidRotCenter(spheroid, theta);
  Facoid* boundA = ShapoidGetBoundingBox(facoid);
  float pa[2] = {-1.414214, -0.414213};
  float pb[2] = {2.828427, 0.0};
  float pc[2] = {0.0, 2.828427};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)boundA)->_pos, i), 
      pa[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundA)->_axis[0], i), 
      pb[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundA)->_axis[1], i), 
      pc[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Facoid* boundB = ShapoidGetBoundingBox(pyramidoid);
  float pd[2] = {-1.08088, 0.86193};
  float pe[2] = {2.82843, 0.0};
  float pf[2] = {0.0, 1.41421};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)boundB)->_pos, i), pd[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundB)->_axis[0], i), 
      pe[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundB)->_axis[1], i), 
      pf[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Facoid* boundC = ShapoidGetBoundingBox(spheroid);
  float pg[2] = {-0.414214, 0.585786};
  float ph[2] = {2.828427, 0.0};
  float pi[2] = {0.0, 2.828427};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)boundC)->_pos, i), pg[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundC)->_axis[0], i), 
      ph[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundC)->_axis[1], i), 
      pi[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  GSetShapoid set = GSetShapoidCreateStatic();
  GSetPush(&set, facoid);
  GSetPush(&set, pyramidoid);
  GSetPush(&set, spheroid);
  Facoid* boundD = ShapoidGetBoundingBox(&set);
  float pj[2] = {-1.41421,-0.41421};
  float pk[2] = {3.828427, 0.0};
  float pl[2] = {0.0, 3.828427};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)boundD)->_pos, i), pj[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundD)->_axis[0], i), 
      pk[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundD)->_axis[1], i), 
      pl[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  GSetFlush(&set);
  VecSet(v, 0, 2.0); VecSet(v, 1, 4.0);
  ShapoidSetPos(facoid, v);
  ShapoidSetPos(pyramidoid, v);
  VecSet(v, 0, 7.0); VecSet(v, 1, 0.0);
  ShapoidSetAxis(facoid, 0, v);
  ShapoidSetAxis(pyramidoid, 0, v);
  VecSet(v, 0, 0.0); VecSet(v, 1, 4.0);
  ShapoidSetAxis(facoid, 1, v);
  ShapoidSetAxis(pyramidoid, 1, v);
  Facoid* boundE = ShapoidGetBoundingBox(facoid);
  float pm[2] = {2.0, 4.0};
  float pn[2] = {7.0, 0.0};
  float po[2] = {0.0, 4.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)boundE)->_pos, i), pm[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundE)->_axis[0], i), 
      pn[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundE)->_axis[1], i), 
      po[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  Facoid* boundF = ShapoidGetBoundingBox(pyramidoid);
  float pp[2] = {2.0, 4.0};
  float pq[2] = {7.0, 0.0};
  float pr[2] = {0.0, 4.0};
  for (int i = dim; i--;) {
    if (ISEQUALF(VecGet(((Shapoid*)boundF)->_pos, i), pp[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundF)->_axis[0], i), 
      pq[i]) == false ||
      ISEQUALF(VecGet(((Shapoid*)boundF)->_axis[1], i), 
      pr[i]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidGetBoundingBox failed");
      PBErrCatch(ShapoidErr);
    }
  }
  ShapoidFree(&boundA);
  ShapoidFree(&boundB);
  ShapoidFree(&boundC);
  ShapoidFree(&boundD);
  ShapoidFree(&boundE);
  ShapoidFree(&boundF);
  VecFree(&v);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestGetBoundingBox OK\n");
}

void UnitTestGetPosDepthCenterCoverage() {
  int dim = 2;
  Facoid* facoid = FacoidCreate(dim);
  Pyramidoid* pyramidoid = PyramidoidCreate(dim);
  Spheroid* spheroid = SpheroidCreate(dim);
  VecFloat* center = ShapoidGetCenter(facoid);
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

void UnitTestFacoidAlignedIsInsideFacoidAligned() {
  Facoid* facA = FacoidCreate(2);
  Facoid* facB = FacoidCreate(2);
  VecFloat2D p = VecFloatCreateStatic2D();
  VecFloat2D u = VecFloatCreateStatic2D();
  VecFloat2D v = VecFloatCreateStatic2D();
  VecSet(&p, 0, 0.0); VecSet(&p, 1, 0.0); 
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetPos(facA, &p);
  ShapoidSetAxis(facA, 0, &u);
  ShapoidSetAxis(facA, 1, &v);
  VecSet(&p, 0, 2.0); VecSet(&p, 1, 2.0); 
  VecSet(&u, 0, 0.5); VecSet(&v, 1, 0.5); 
  ShapoidSetPos(facB, &p);
  ShapoidSetAxis(facB, 0, &u);
  ShapoidSetAxis(facB, 1, &v);
  if (FacoidAlignedIsInsideFacoidAligned(facA, facB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedIsInsideFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&p, 0, 0.1); VecSet(&p, 1, 0.1); 
  ShapoidSetPos(facB, &p);
  if (FacoidAlignedIsInsideFacoidAligned(facB, facA) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedIsInsideFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetAxis(facB, 0, &u);
  ShapoidSetAxis(facB, 1, &v);
  if (FacoidAlignedIsInsideFacoidAligned(facB, facA) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedIsInsideFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&facA);
  ShapoidFree(&facB);
  printf("UnitTestFacoidAlignedIsInsideFacoidAligned OK\n");
}

void UnitTestFacoidAlignedIsOutsideFacoidAligned() {
  Facoid* facA = FacoidCreate(2);
  Facoid* facB = FacoidCreate(2);
  VecFloat2D p = VecFloatCreateStatic2D();
  VecFloat2D u = VecFloatCreateStatic2D();
  VecFloat2D v = VecFloatCreateStatic2D();
  VecSet(&p, 0, 0.0); VecSet(&p, 1, 0.0); 
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetPos(facA, &p);
  ShapoidSetAxis(facA, 0, &u);
  ShapoidSetAxis(facA, 1, &v);
  VecSet(&p, 0, 2.0); VecSet(&p, 1, 2.0); 
  VecSet(&u, 0, 0.5); VecSet(&v, 1, 0.5); 
  ShapoidSetPos(facB, &p);
  ShapoidSetAxis(facB, 0, &u);
  ShapoidSetAxis(facB, 1, &v);
  if (FacoidAlignedIsOutsideFacoidAligned(facA, facB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedIsOutsideFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&p, 0, 0.1); VecSet(&p, 1, 0.1); 
  ShapoidSetPos(facB, &p);
  if (FacoidAlignedIsOutsideFacoidAligned(facB, facA) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedIsOutsideFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetAxis(facB, 0, &u);
  ShapoidSetAxis(facB, 1, &v);
  if (FacoidAlignedIsOutsideFacoidAligned(facB, facA) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedIsOutsideFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&facA);
  ShapoidFree(&facB);
  printf("UnitTestFacoidAlignedIsOutsideFacoidAligned OK\n");
}

void UnitTestFacoidAlignedSplitExcludingFacoidAligned() {
  Facoid* facA = FacoidCreate(2);
  Facoid* facB = FacoidCreate(2);
  VecFloat2D p = VecFloatCreateStatic2D();
  VecFloat2D u = VecFloatCreateStatic2D();
  VecFloat2D v = VecFloatCreateStatic2D();
  VecSet(&p, 0, 0.0); VecSet(&p, 1, 0.0); 
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetPos(facA, &p);
  ShapoidSetAxis(facA, 0, &u);
  ShapoidSetAxis(facA, 1, &v);
  VecSet(&p, 0, 0.5); VecSet(&p, 1, 0.5); 
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetPos(facB, &p);
  ShapoidSetAxis(facB, 0, &u);
  ShapoidSetAxis(facB, 1, &v);
  GSetShapoid* split = 
    FacoidAlignedSplitExcludingFacoidAligned(facA, facB);
  Facoid* facC = (Facoid*)GSetPop(split);
  VecSet(&p, 0, 0.0); VecSet(&p, 1, 0.0); 
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 0.5); 
  if (VecIsEqual(ShapoidPos(facC), &p) == false ||
    VecIsEqual(ShapoidAxis(facC, 0), &u) == false ||
    VecIsEqual(ShapoidAxis(facC, 1), &v) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedSplitExcludingFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  Facoid* facD = (Facoid*)GSetPop(split);
  VecSet(&p, 0, 0.0); VecSet(&p, 1, 0.5); 
  VecSet(&u, 0, 0.5); VecSet(&v, 1, 0.5); 
  if (VecIsEqual(ShapoidPos(facD), &p) == false ||
    VecIsEqual(ShapoidAxis(facD, 0), &u) == false ||
    VecIsEqual(ShapoidAxis(facD, 1), &v) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedSplitExcludingFacoidAligned failed");
    PBErrCatch(ShapoidErr);
  }
  GSetFree(&split);
  ShapoidFree(&facA);
  ShapoidFree(&facB);
  ShapoidFree(&facC);
  ShapoidFree(&facD);
  printf("UnitTestFacoidAlignedSplitExcludingFacoidAligned OK\n");
}

void UnitTestFacoidAlignedAddClippedToSet() {
  Facoid* facA = FacoidCreate(2);
  VecFloat2D p = VecFloatCreateStatic2D();
  VecFloat2D u = VecFloatCreateStatic2D();
  VecFloat2D v = VecFloatCreateStatic2D();
  VecSet(&p, 0, 0.0); VecSet(&p, 1, 0.0); 
  VecSet(&u, 0, 10.0); VecSet(&v, 1, 10.0); 
  ShapoidSetPos(facA, &p);
  ShapoidSetAxis(facA, 0, &u);
  ShapoidSetAxis(facA, 1, &v);
  GSetShapoid set = GSetShapoidCreateStatic();
  FacoidAlignedAddClippedToSet(facA, &set);
  VecSet(&p, 0, 15.0); VecSet(&p, 1, 15.0); 
  ShapoidSetPos(facA, &p);
  FacoidAlignedAddClippedToSet(facA, &set);
  VecSet(&p, 0, 8.0); VecSet(&p, 1, 8.0); 
  ShapoidSetPos(facA, &p);
  FacoidAlignedAddClippedToSet(facA, &set);
  VecSet(&p, 0, 12.0); VecSet(&p, 1, 9.0); 
  ShapoidSetPos(facA, &p);
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 10.0); 
  ShapoidSetAxis(facA, 0, &u);
  ShapoidSetAxis(facA, 1, &v);
  FacoidAlignedAddClippedToSet(facA, &set);
  VecSet(&p, 0, 5.0); VecSet(&p, 1, 5.0); 
  ShapoidSetPos(facA, &p);
  VecSet(&u, 0, 1.0); VecSet(&v, 1, 1.0); 
  ShapoidSetAxis(facA, 0, &u);
  ShapoidSetAxis(facA, 1, &v);
  FacoidAlignedAddClippedToSet(facA, &set);
  if (GSetNbElem(&set) != 6) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, 
      "FacoidAlignedAddClippedToSet failed");
    PBErrCatch(ShapoidErr);
  }
  int iCheck = 0;
  float checkp[12] = {
      0.0, 0.0, 15.0, 15.0, 10.0, 8.0, 8.0, 
      10.0, 8.0, 15.0, 12.0, 18.0};
  float checku[6] = {10.0, 10.0, 8.0, 10.0, 7.0, 1.0};
  float checkv[6] = {10.0, 10.0, 2.0, 5.0, 3.0, 1.0};
  do {
    Facoid* fac = (Facoid*)GSetPop(&set);
    VecSet(&p, 0, checkp[2 * iCheck]); 
    VecSet(&p, 1, checkp[2 * iCheck + 1]); 
    VecSet(&u, 0, checku[iCheck]); VecSet(&v, 1, checkv[iCheck]); 
    if (VecIsEqual(ShapoidPos(fac), &p) == false ||
      VecIsEqual(ShapoidAxis(fac, 0), &u) == false ||
      VecIsEqual(ShapoidAxis(fac, 1), &v) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, 
        "FacoidAlignedAddClippedToSet failed");
      PBErrCatch(ShapoidErr);
    }
    ShapoidFree(&fac);
    ++iCheck;
  } while(GSetNbElem(&set) > 0);
  ShapoidFree(&facA);
  printf("UnitTestFacoidAlignedAddClippedToSet OK\n");
}

void UnitTestIsInter() {
  Spheroid* spheroidA = SpheroidCreate(3);
  Spheroid* spheroidB = SpheroidCreate(3);
  VecFloat3D v = VecFloatCreateStatic3D();
  if (ShapoidIsInter(spheroidA, spheroidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&v, 0, 1.1);
  ShapoidSetPos(spheroidB, &v);
  if (ShapoidIsInter(spheroidB, spheroidA) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&v, 1, 1.0);
  ShapoidSetPos(spheroidB, &v);
  if (ShapoidIsInter(spheroidA, spheroidB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.1);
  ShapoidSetPos(spheroidB, &v);
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 2.0); VecSet(&v, 2, 1.0);
  ShapoidScale(spheroidB, (VecFloat*)&v);
  if (ShapoidIsInter(spheroidA, spheroidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidRotZCenter(spheroidB, -PBMATH_QUARTERPI);
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0);
  ShapoidSetPos(spheroidB, &v);
  if (ShapoidIsInter(spheroidA, spheroidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&spheroidA);
  ShapoidFree(&spheroidB);

  Pyramidoid* pyramidoidA = PyramidoidCreate(2);
  Pyramidoid* pyramidoidB = PyramidoidCreate(2);
  Facoid* facoidA = FacoidCreate(2);
  Facoid* facoidB = FacoidCreate(2);
  VecFloat2D w = VecFloatCreateStatic2D();
  if (ShapoidIsInter(pyramidoidA, pyramidoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPP 1)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidA, facoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPF 1)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidA, pyramidoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATFP 1)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidA, facoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATFF 1)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&w, 0, 0.51);
  ShapoidSetPos(pyramidoidB, &w);
  if (ShapoidIsInter(pyramidoidA, pyramidoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPP 2)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidB, facoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPF 2)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidB, pyramidoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATFP 2)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&w, 1, 0.51);
  ShapoidSetPos(pyramidoidB, &w);
  if (ShapoidIsInter(pyramidoidA, pyramidoidB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPP 3)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidB, facoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPF 3)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidB, pyramidoidB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATFP 3)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&w, 0, 1.5);
  ShapoidSetPos(pyramidoidB, &w);
  if (ShapoidIsInter(pyramidoidA, pyramidoidB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPP 4)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidB, facoidB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATPF 4)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidB, pyramidoidB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATFP 4)");
    PBErrCatch(ShapoidErr);
  }
  ShapoidSetPos(facoidB, &w);
  if (ShapoidIsInter(facoidA, facoidB) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (SATFF 5)");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&pyramidoidA);
  ShapoidFree(&pyramidoidB);
  ShapoidFree(&facoidA);
  ShapoidFree(&facoidB);

  Pyramidoid* pyramidoidC = PyramidoidCreate(3);
  Pyramidoid* pyramidoidD = PyramidoidCreate(3);
  Facoid* facoidC = FacoidCreate(3);
  Facoid* facoidD = FacoidCreate(3);
  VecFloat3D u = VecFloatCreateStatic3D();
  if (ShapoidIsInter(pyramidoidC, pyramidoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPP 1)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidC, pyramidoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBFP 1)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidC, facoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPF 1)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidC, facoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBFF 1)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&u, 0, 0.51);
  ShapoidSetPos(pyramidoidC, &u);
  if (ShapoidIsInter(pyramidoidC, pyramidoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPP 2)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidC, pyramidoidC) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBFP 2)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidC, facoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPF 2)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&u, 1, 0.51);
  VecSet(&u, 2, 0.51);
  ShapoidSetPos(pyramidoidC, &u);
  if (ShapoidIsInter(pyramidoidC, pyramidoidD) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPP 3)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidC, pyramidoidC) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBFP 3)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidC, facoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPF 3)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&u, 0, 1.51);
  ShapoidSetPos(pyramidoidC, &u);
  if (ShapoidIsInter(pyramidoidC, pyramidoidD) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPP 4)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(facoidC, pyramidoidC) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBFP 4)");
    PBErrCatch(ShapoidErr);
  }
  if (ShapoidIsInter(pyramidoidC, facoidD) == true) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPF 4)");
    PBErrCatch(ShapoidErr);
  }
  VecSet(&u, 0, -0.5);
  VecSet(&u, 1, -0.5);
  VecSet(&u, 2, -0.5);
  ShapoidSetPos(pyramidoidC, &u);
  VecSet(&u, 0, 1.0);
  VecSet(&u, 1, 1.0);
  VecSet(&u, 2, -1.0);
  ShapoidSetAxis(pyramidoidC, 0, &u);
  VecSet(&u, 0, 0.0);
  VecSet(&u, 1, 1.0);
  VecSet(&u, 2, -1.0);
  ShapoidSetAxis(pyramidoidC, 1, &u);
  VecSet(&u, 0, 1.0);
  VecSet(&u, 1, 1.0);
  VecSet(&u, 2, 1.0);
  ShapoidSetAxis(pyramidoidC, 2, &u);
  if (ShapoidIsInter(pyramidoidC, facoidD) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIsInter failed (FMBPF 5)");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&pyramidoidC);
  ShapoidFree(&pyramidoidD);
  ShapoidFree(&facoidC);
  ShapoidFree(&facoidD);

  printf("UnitTestIsInter OK\n");
}

void UnitTestShapoidIterCreateFree() {
  Facoid* facoid = FacoidCreate(2);
  VecFloat2D delta = VecFloatCreateStatic2D();
  ShapoidIter iter = ShapoidIterCreateStatic(facoid, &delta);
  if (iter._shap != (Shapoid*)facoid ||
    iter._pos == NULL ||
    iter._delta == NULL ||
    VecGetDim(iter._pos) != 2 ||
    VecGetDim(iter._delta) != 2) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIterFreeStatic failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&facoid);
  ShapoidIterFreeStatic(&iter);
  if (iter._pos != NULL ||
    iter._delta != NULL) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIterFreeStatic failed");
    PBErrCatch(ShapoidErr);
  }
  printf("UnitTestShapoidIterCreateFree OK\n");
}

void UnitTestShapoidIterGetSet() {
  Facoid* facoidA = FacoidCreate(2);
  Facoid* facoidB = FacoidCreate(2);
  VecFloat2D deltaA = VecFloatCreateStatic2D();
  VecFloat2D deltaB = VecFloatCreateStatic2D();
  for (int i = 2; i--;) {
    VecSet(&deltaA, i, 0.1);
    VecSet(&deltaB, i, 0.2);
  }
  ShapoidIter iter = ShapoidIterCreateStatic(facoidA, &deltaA);
  if (ShapoidIterShapoid(&iter) != (Shapoid*)facoidA) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIterShapoid failed");
    PBErrCatch(ShapoidErr);
  }
  if (VecIsEqual(ShapoidIterDelta(&iter), &deltaA) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIterDelta failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidIterSetShapoid(&iter, facoidB);
  ShapoidIterSetDelta(&iter, &deltaB);
  if (ShapoidIterShapoid(&iter) != (Shapoid*)facoidB) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIterSetShapoid failed");
    PBErrCatch(ShapoidErr);
  }
  if (VecIsEqual(ShapoidIterDelta(&iter), &deltaB) == false) {
    ShapoidErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ShapoidErr->_msg, "ShapoidIterSetDelta failed");
    PBErrCatch(ShapoidErr);
  }
  ShapoidFree(&facoidA);
  ShapoidFree(&facoidB);
  ShapoidIterFreeStatic(&iter);
  printf("UnitTestShapoidIterGetSet OK\n");
}

void UnitTestShapoidIterStepFacoid() {
  Facoid* facoid = FacoidCreate(2);
  VecFloat2D delta = VecFloatCreateStatic2D();
  for (int i = 2; i--;)
    VecSet(&delta, i, 0.25);
  ShapoidIter iter = ShapoidIterCreateStatic(facoid, &delta);
  int iCheck = 0;
  float check[50] = {0.000,0.000,0.000,0.250,0.000,0.500,0.000,0.750,
    0.000,1.000,0.250,0.000,0.250,0.250,0.250,0.500,0.250,0.750,0.250,
    1.000,0.500,0.000,0.500,0.250,0.500,0.500,0.500,0.750,0.500,1.000,
    0.750,0.000,0.750,0.250,0.750,0.500,0.750,0.750,0.750,1.000,1.000,
    0.000,1.000,0.250,1.000,0.500,1.000,0.750,1.000,1.000
    };
  do {
    VecFloat* v = ShapoidIterGetInternalPos(&iter);
    if (ISEQUALF(VecGet(v, 0), check[2 * iCheck]) == false ||
      ISEQUALF(VecGet(v, 1), check[2 * iCheck + 1]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidIterStep failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&v);
    ++iCheck;
  } while (ShapoidIterStep(&iter));
  ShapoidFree(&facoid);
  ShapoidIterFreeStatic(&iter);
  printf("UnitTestShapoidIterStepFacoid OK\n");
}

void UnitTestShapoidIterStepPyramidoid() {
  Pyramidoid* pyramidoid = PyramidoidCreate(3);
  VecFloat3D delta = VecFloatCreateStatic3D();
  for (int i = 3; i--;)
    VecSet(&delta, i, 0.25);
  ShapoidIter iter = ShapoidIterCreateStatic(pyramidoid, &delta);
  int iCheck = 0;
  float check[105] = {0.000,0.000,0.000,0.000,0.000,0.250,0.000,0.000,
    0.500,0.000,0.000,0.750,0.000,0.000,1.000,0.000,0.250,0.000,0.000,
    0.250,0.250,0.000,0.250,0.500,0.000,0.250,0.750,0.000,0.500,0.000,
    0.000,0.500,0.250,0.000,0.500,0.500,0.000,0.750,0.000,0.000,0.750,
    0.250,0.000,1.000,0.000,0.250,0.000,0.000,0.250,0.000,0.250,0.250,
    0.000,0.500,0.250,0.000,0.750,0.250,0.250,0.000,0.250,0.250,0.250,
    0.250,0.250,0.500,0.250,0.500,0.000,0.250,0.500,0.250,0.250,0.750,
    0.000,0.500,0.000,0.000,0.500,0.000,0.250,0.500,0.000,0.500,0.500,
    0.250,0.000,0.500,0.250,0.250,0.500,0.500,0.000,0.750,0.000,0.000,
    0.750,0.000,0.250,0.750,0.250,0.000,1.000,0.000,0.000
    };
  do {
    VecFloat* v = ShapoidIterGetInternalPos(&iter);
    if (ISEQUALF(VecGet(v, 0), check[3 * iCheck]) == false ||
      ISEQUALF(VecGet(v, 1), check[3 * iCheck + 1]) == false ||
      ISEQUALF(VecGet(v, 2), check[3 * iCheck + 2]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidIterStep failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&v);
    ++iCheck;
  } while (ShapoidIterStep(&iter));
  ShapoidFree(&pyramidoid);
  ShapoidIterFreeStatic(&iter);
  printf("UnitTestShapoidIterStepPyramidoid OK\n");
}

void UnitTestShapoidIterStepSpheroid() {
  int dim = 3;
  Spheroid* spheroid = SpheroidCreate(dim);
  VecFloat* delta = VecFloatCreate(dim);
  for (int i = dim; i--;)
    VecSet(delta, i, 0.25);
  ShapoidIter iter = ShapoidIterCreateStatic(spheroid, delta);
  int iCheck = 0;
  float check[147] = {
    0.00000,0.00000,-0.50000,0.00000,-0.43301,-0.25000,-0.39244,
    -0.18301,-0.25000,-0.14244,-0.18301,-0.25000,0.10756,-0.18301,
    -0.25000,0.35756,-0.18301,-0.25000,0.39244,-0.18301,-0.25000,
    -0.42780,0.06699,-0.25000,-0.17780,0.06699,-0.25000,0.07220,
    0.06699,-0.25000,0.32220,0.06699,-0.25000,0.42780,0.06699,
    -0.25000,-0.29499,0.31699,-0.25000,-0.04499,0.31699,-0.25000,
    0.20501,0.31699,-0.25000,0.29499,0.31699,-0.25000,0.00000,
    -0.50000,0.00000,-0.43301,-0.25000,0.00000,-0.18301,-0.25000,
    0.00000,0.06699,-0.25000,0.00000,0.31699,-0.25000,0.00000,0.43301,
    -0.25000,0.00000,-0.50000,0.00000,0.00000,-0.25000,0.00000,0.00000,
    0.00000,0.00000,0.00000,0.25000,0.00000,0.00000,0.50000,0.00000,
    0.00000,-0.43301,0.25000,0.00000,-0.18301,0.25000,0.00000,0.06699,
    0.25000,0.00000,0.31699,0.25000,0.00000,0.43301,0.25000,0.00000,
    0.00000,0.50000,0.00000,0.00000,-0.43301,0.25000,-0.39244,-0.18301,
    0.25000,-0.14244,-0.18301,0.25000,0.10756,-0.18301,0.25000,0.35756,
    -0.18301,0.25000,0.39244,-0.18301,0.25000,-0.42780,0.06699,0.25000,
    -0.17780,0.06699,0.25000,0.07220,0.06699,0.25000,0.32220,0.06699,
    0.25000,0.42780,0.06699,0.25000,-0.29499,0.31699,0.25000,-0.04499,
    0.31699,0.25000,0.20501,0.31699,0.25000,0.29499,0.31699,0.25000,
    0.00000,0.00000,0.50000
    };
  do {
    VecFloat* v = ShapoidIterGetInternalPos(&iter);
    if (ISEQUALF(VecGet(v, 0), check[3 * iCheck]) == false ||
      ISEQUALF(VecGet(v, 1), check[3 * iCheck + 1]) == false ||
      ISEQUALF(VecGet(v, 2), check[3 * iCheck + 2]) == false) {
      ShapoidErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ShapoidErr->_msg, "ShapoidIterStep failed");
      PBErrCatch(ShapoidErr);
    }
    VecFree(&v);
    ++iCheck;
  } while (ShapoidIterStep(&iter));
  ShapoidFree(&spheroid);
  ShapoidIterFreeStatic(&iter);
  VecFree(&delta);
  printf("UnitTestShapoidIterStepSpheroid OK\n");
}

void UnitTestShapoidIter() {
  UnitTestShapoidIterCreateFree();
  UnitTestShapoidIterGetSet();
  UnitTestShapoidIterStepFacoid();
  UnitTestShapoidIterStepPyramidoid();
  UnitTestShapoidIterStepSpheroid();
  
  printf("UnitTestShapoidIter OK\n");
}

void UnitTestAll() {
  UnitTestCreateCloneIsEqualFree();
  UnitTestLoadSavePrint();
  UnitTestGetSetTypeDimPosAxis();
  UnitTestTranslateScaleGrow();
  UnitTestRotate();
  UnitTestRotateAxis();
  UnitTestRotateX();
  UnitTestRotateY();
  UnitTestRotateZ();
  UnitTestImportExportCoordIsPosInside();
  UnitTestGetBoundingBox();
  UnitTestGetPosDepthCenterCoverage();
  UnitTestFacoidAlignedIsInsideFacoidAligned();
  UnitTestFacoidAlignedIsOutsideFacoidAligned();
  UnitTestFacoidAlignedSplitExcludingFacoidAligned();
  UnitTestFacoidAlignedAddClippedToSet();
  UnitTestIsInter();
  UnitTestShapoidIter();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

