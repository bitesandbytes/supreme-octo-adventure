#ifndef UTILS_H
#define UTILS_H

#include <vector>

#define DBG_LEVEL 3

#define dbg(x, ...) if( DBG_LEVEL <= x )printf( __VA_ARGS__ );

namespace Utils {

double doubleRand();

int randInRange(int max);

template<class T, unsigned ... RestD>
struct array;

template<class T, unsigned PrimaryD>
class array<T, PrimaryD> {
 public:
  std::vector<T> data;

  array() {
    data = std::vector<T>(PrimaryD, T());
  }

  T &operator[](unsigned i) { return data[i]; }

};

template<class T, unsigned PrimaryD, unsigned ... RestD>
class array<T, PrimaryD, RestD...> {
 public:
  std::vector<array<T, RestD...>> data;

  array() {
    data = std::vector<array<T, RestD...>>(PrimaryD, array<T, RestD...>());
  }

  array<T, RestD...> &operator[](unsigned i) { return data[i]; }
};

} // namespace Utils
#endif

