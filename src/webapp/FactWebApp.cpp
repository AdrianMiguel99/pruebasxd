#include "FactWebApp.hpp"
#include "FactCalc.hpp"
#include <sstream>

FactWebApp::FactWebApp() {
  
}

std::string FactWebApp::getTitle() const {
  return "Prime Factorization";
}

const char* FactWebApp::getPrefix1() const {
  return "/fact";
}

const char* FactWebApp::getPrefix2() const {
  return "/fact?number=";
}

void FactWebApp::processNumber(int64_t number, HttpResponse& httpResponse) {
  const auto& factors = FactCalc::factorize(number);

  std::ostringstream output;
  output << number << " = ";

  for (size_t i = 0; i < factors.size(); ++i) {
    if (i > 0) output << " * ";
    output << factors[i].first;
    if (factors[i].second > 1) {
      output << "<sup>" << factors[i].second << "</sup>";
  }

  }

  httpResponse.body() << "    <li>" << output.str() << "</li>\n";
}
