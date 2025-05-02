#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include "MathWebApp.hpp"

class GoldbachWebApp : public MathWebApp {
 public:
  GoldbachWebApp();
  ~GoldbachWebApp() override = default;

 protected:
  std::string getTitle() const override;
  const char* getPrefix1() const override;
  const char* getPrefix2() const override;
  void processNumber(int64_t number, HttpResponse& httpResponse) override;
};

#endif  // GOLDBACHWEBAPP_HPP
