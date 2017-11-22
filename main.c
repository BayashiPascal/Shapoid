#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "shapoid.h"

int main(int argc, char **argv) {
  // Initialise the random generator
  srandom(time(NULL));

  Shapoid* facoidA = FacoidCreate(2);
  ShapoidPrint(facoidA, stdout);
  VecFloat *v = VecFloatCreate(2);
  if (v == NULL) {
    fprintf(stderr, "malloc failed\n");
    return 28;
  }
  VecSet(v, 0, 2.0);
  VecSet(v, 1, 3.0);
  ShapoidScale(facoidA, v);
  fprintf(stdout, "scale by ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  ShapoidPrint(facoidA, stdout);
  ShapoidRotate2D(facoidA, PBMATH_PI * 0.5);
  fprintf(stdout, "rotate by %.3f\n", PBMATH_PI * 0.5);
  ShapoidPrint(facoidA, stdout);
  VecSet(v, 0, 1.0);
  VecSet(v, 1, -1.0);
  ShapoidTranslate(facoidA, v);
  fprintf(stdout, "translate by ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  ShapoidPrint(facoidA, stdout);
  VecSet(v, 0, 0.0);
  VecSet(v, 1, 1.0);
  ShapoidSetAxis(facoidA, 0, v);
  fprintf(stdout, "set axis 0 to ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  ShapoidPrint(facoidA, stdout);
  fprintf(stdout, "save shapoid to shapoid.txt\n");
  FILE *f = fopen("./shapoid.txt", "w");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 29;
  }
  ShapoidSave(facoidA, f);
  fclose(f);
  fprintf(stdout, "load shapoid from shapoid.txt\n");
  Shapoid* facoidB = NULL;
  f = fopen("./shapoid.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 30;
  }
  ShapoidLoad(&facoidB, f);
  fclose(f);
  ShapoidPrint(facoidB, stdout);
  VecSet(v, 0, 1.0);
  VecSet(v, 1, 1.0);
  VecFloat *coordEx = ShapoidExportCoord(facoidB, v);
  if (coordEx == NULL) {
    fprintf(stderr, "ShapoidExport failed\n");
    return 31;
  }
  fprintf(stdout,"coordinates ");
  VecPrint(v, stdout);
  fprintf(stdout," in the shapoid becomes ");
  VecPrint(coordEx, stdout);
  fprintf(stdout," in the standard coordinate system\n");
  VecCopy(v, coordEx);
  VecFloat *coordIm = ShapoidImportCoord(facoidB, v);
  if (coordIm == NULL) {
    fprintf(stderr, "ShapoidImport failed\n");
    return 32;
  }
  fprintf(stdout,"coordinates ");
  VecPrint(v, stdout);
  fprintf(stdout," in the standard coordinate system becomes ");
  VecPrint(coordIm, stdout);
  fprintf(stdout," in the shapoid\n");
  VecSet(v, 0, 0.0);
  VecSet(v, 1, 0.0);
  VecPrint(v, stdout);
  if (ShapoidIsPosInside(facoidB, v) == true)
    fprintf(stdout, " is in the facoid\n");
  else
    fprintf(stdout, " is not in the facoid\n");
  VecSet(v, 0, 1.0);
  VecSet(v, 1, -4.0);
  VecPrint(v, stdout);
  if (ShapoidIsPosInside(facoidB, v) == true)
    fprintf(stdout, " is in the facoid\n");
  else
    fprintf(stdout, " is not in the facoid\n");
  ShapoidRotate2D(facoidB, -PBMATH_QUARTERPI);
  Shapoid *bounding = ShapoidGetBoundingBox(facoidB);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 33;
  }
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  ShapoidPrint(bounding, stdout);
  ShapoidFree(&bounding);
  VecFloat *center = ShapoidGetCenter(facoidB);
  if (center == NULL) {
    fprintf(stderr, "ShapoidGetCenter failed\n");
    return 34;
  }
  fprintf(stdout, "center of the facoid is ");
  VecPrint(center, stdout);
  fprintf(stdout, "\n");
  VecFree(&center);
  facoidB->_type = ShapoidTypePyramidoid;
  bounding = ShapoidGetBoundingBox(facoidB);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 35;
  }
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  ShapoidPrint(bounding, stdout);
  ShapoidFree(&bounding);
  center = ShapoidGetCenter(facoidB);
  if (center == NULL) {
    fprintf(stderr, "ShapoidGetCenter failed\n");
    return 36;
  }
  fprintf(stdout, "center of the facoid is ");
  VecPrint(center, stdout);
  fprintf(stdout, "\n");
  VecFree(&center);
  facoidB->_type = ShapoidTypeSpheroid;
  bounding = ShapoidGetBoundingBox(facoidB);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 37;
  }
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  ShapoidPrint(bounding, stdout);
  center = ShapoidGetCenter(facoidB);
  if (center == NULL) {
    fprintf(stderr, "ShapoidGetCenter failed\n");
    return 38;
  }
  fprintf(stdout, "center of the shapoid is ");
  VecPrint(center, stdout);
  fprintf(stdout, "\n");
  VecFree(&center);

  GSet *setBounding = GSetCreate();
  if (setBounding == NULL) {
    fprintf(stderr, "GSetCreate failed\n");
    return 39;
  }
  GSetAppend(setBounding, facoidA);
  GSetAppend(setBounding, facoidB);
  facoidB->_type = ShapoidTypeFacoid;
  VecSet(facoidB->_pos, 0, 2.0);
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidA, stdout);
  fprintf(stdout, "and\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  bounding = ShapoidGetBoundingBox(setBounding);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 40;
  }
  ShapoidPrint(bounding, stdout);
  // Grow
  fprintf(stdout, "Grow the facoid:\n");
  ShapoidPrint(facoidA, stdout);
  fprintf(stdout, "by 2.0:\n");
  VecSet(v, 0, 2.0); VecSet(v, 1, 2.0);
  ShapoidGrow(facoidA, v);
  ShapoidPrint(facoidA, stdout);
  // Coverage ratio
  fprintf(stdout, "Percentage of :\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "included in :\n");
  ShapoidPrint(facoidA, stdout);
  float ratio = ShapoidGetCoverage(facoidA, facoidB);
  fprintf(stdout, "is %f\n", ratio);
  // Free memory
  ShapoidFree(&bounding);
  GSetFree(&setBounding);
  VecFree(&coordEx);
  VecFree(&coordIm);
  VecFree(&v);
  ShapoidFree(&facoidA);
  ShapoidFree(&facoidB);
  ShapoidFree(&bounding);

  // Return success code
  return 0;
}

