#include "FactCalc.hpp"

std::vector<std::pair<int64_t, int>> FactCalc::factorize(int64_t n) {
  std::vector<std::pair<int64_t, int>> result;

  if (n == 0 || n == 1 || n == -1) {
    // 0, 1 y -1 no tienen factorización útil
    return result;
  }

  if (n < 0) {
    result.emplace_back(-1, 1); // incluye el signo
    n = -n;
  }

  for (int64_t i = 2; i * i <= n; ++i) {
    int count = 0;
    while (n % i == 0) {
      ++count;
      n /= i;
    }
    if (count > 0) {
      result.emplace_back(i, count);
    }
  }

  if (n > 1) {
    result.emplace_back(n, 1); // número primo restante
  }

  return result;
}
