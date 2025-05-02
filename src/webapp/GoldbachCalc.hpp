#ifndef GOLDBACHCALC_HPP
#define GOLDBACHCALC_HPP

#include <vector>
#include <cstdint>

class GoldbachCalc {
 public:
  explicit GoldbachCalc(int64_t number);
  const std::vector<std::pair<int64_t, int64_t>>& getSums() const;
  bool isValid() const;

 private:
  int64_t number;
  std::vector<std::pair<int64_t, int64_t>> sums;
  bool valid;
  void compute();
  bool isPrime(int64_t n) const;
};

#endif  // GOLDBACHCALC_HPP
