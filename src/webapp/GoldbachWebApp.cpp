#include "GoldbachWebApp.hpp"
#include "GoldbachCalc.hpp"
#include <sstream>

GoldbachWebApp::GoldbachWebApp() {}

std::string GoldbachWebApp::getTitle() const {
  return "Goldbach Conjecture";
}

const char* GoldbachWebApp::getPrefix1() const {
  return "/goldbach";
}

const char* GoldbachWebApp::getPrefix2() const {
  return "/goldbach?number=";
}

void GoldbachWebApp::processNumber(int64_t number, HttpResponse& httpResponse) {
  GoldbachCalc calc(number);
  if (!calc.isValid()) {
    httpResponse.body() << "    <li class=err>" << number
                        << ": not a valid even number > 2\n";
    return;
  }

  const auto& sums = calc.getSums();
  httpResponse.body() << "    <li>" << number << " = ";
  for (size_t i = 0; i < sums.size(); ++i) {
    if (i > 0) httpResponse.body() << " = ";
    httpResponse.body() << sums[i].first << " + " << sums[i].second;
  }
  httpResponse.body() << "\n";
}
