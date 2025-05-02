#ifndef FACTCALC_HPP
#define FACTCALC_HPP

#include <vector>
#include <cstdint>

class FactCalc {
 public:
  // Retorna un vector de pares (factor primo, exponente)
  static std::vector<std::pair<int64_t, int>> factorize(int64_t n);
};

#endif // FACTCALC_HPP
