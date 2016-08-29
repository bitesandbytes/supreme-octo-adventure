#include <stdlib.h>
#include "utils.h"

double Utils::doubleRand() {
  double r, x;
  r = ((double) rand() / ((double) (RAND_MAX) + (double) (1)));
  return r;
}

int Utils::randInRange(int max) {
  double r, x;
  r = ((double) rand() / ((double) (RAND_MAX) + (double) (1)));
  x = (r * (max + 1));
  return (int) x;
}
