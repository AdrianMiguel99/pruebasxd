#ifndef FACTWEBAPP_HPP
#define FACTWEBAPP_HPP

#include "MathWebApp.hpp"

/**
 * @brief Web application for computing prime factorizations.
 */
class FactWebApp : public MathWebApp {
 public:
  FactWebApp();
  ~FactWebApp() override = default;

 protected:
  const char* getPrefix1() const override;
  const char* getPrefix2() const override;
  std::string getTitle() const override;
  void processNumber(int64_t number, HttpResponse& httpResponse) override;
};

#endif  // FACTWEBAPP_HPP
