#include "GoldbachCalc.hpp"
#include <cmath>

GoldbachCalc::GoldbachCalc(int64_t number) : number(number), valid(false) {
  this->compute();
}

bool GoldbachCalc::isValid() const {
  return this->valid;
}

const std::vector<std::pair<int64_t, int64_t>>& GoldbachCalc::getSums() const {
  return this->sums;
}

void GoldbachCalc::compute() {
  if (number <= 2 || number % 2 != 0) {
    valid = false;
    return;
  }

  valid = true;
  for (int64_t i = 2; i <= number / 2; ++i) {
    int64_t j = number - i;
    if (isPrime(i) && isPrime(j)) {
      sums.emplace_back(i, j);
    }
  }
}

bool GoldbachCalc::isPrime(int64_t n) const {
  if (n < 2) return false;
  for (int64_t i = 2; i * i <= n; ++i) {
    if (n % i == 0) return false;
  }
  return true;
}
