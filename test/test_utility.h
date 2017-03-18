#ifndef PGM_TEST_UTILITY_H
#define PGM_TEST_UTILITY_H

template<typename T>
bool equal(T f1, T f2) { 
  return (std::abs(f1 - f2) <= 0.0001);
}

#endif