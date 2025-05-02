#ifndef MATHWEBAPP_HPP
#define MATHWEBAPP_HPP

#include "HttpApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Util.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <iostream>  // Para depuración

/**
 * @brief Abstract base class for math-related web applications.
 *        Provides reusable logic for handling number-based HTTP queries.
 */
class MathWebApp : public HttpApp {
  DISABLE_COPY(MathWebApp);

 public:
  MathWebApp() = default;
  virtual ~MathWebApp() = default;

  void start() override {}
  void stop() override {}

  bool handleHttpRequest(HttpRequest& httpRequest,
                         HttpResponse& httpResponse) override final {
  std::cout << "[DEBUG] MathWebApp::handleHttpRequest recibido: "<< httpRequest.getURI() << std::endl;
    if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
      return serveHomepage(httpRequest, httpResponse);
    }

    const std::string& uri = httpRequest.getURI();
    if (uri.rfind(getPrefix1(), 0) == 0 || uri.rfind(getPrefix2(), 0) == 0) {
      return serveApp(httpRequest, httpResponse);
    }

    return false;
  }

 protected:
  virtual std::string getTitle() const = 0;
  virtual const char* getPrefix1() const = 0;
  virtual const char* getPrefix2() const = 0;
  virtual void processNumber(int64_t number, HttpResponse& httpResponse) = 0;

  void serveHeader(HttpResponse& httpResponse, const std::string& title) const {
    httpResponse.setHeader("Server", "AttoServer v1.0");
    httpResponse.setHeader("Content-type", "text/html; charset=ascii");
    httpResponse.body()
      << "<!DOCTYPE html>\n"
      << "<html lang=en>\n"
      << "  <meta charset=utf-8>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1>" << title << "</h1>\n";
  }

  virtual bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse) final {
    (void)httpRequest;
    serveHeader(httpResponse, getTitle());
    httpResponse.body()
      << "  <form method=get action=" << getPrefix1() << ">\n"
      << "    <label for=number>Number</label>\n"
      << "    <input type=text id=number name=number pattern=\"[+\\-]?\\d+(,[+\\-]?\\d+)*\" required>\n"
      << "    <button type=submit>Submit</button>\n"
      << "  </form>\n"
      << "</html>\n";
    return httpResponse.send();
  }

  virtual bool serveApp(HttpRequest& httpRequest, HttpResponse& httpResponse) final {
    const std::string& rawUri = httpRequest.getURI();
    const std::string& uri = Util::decodeURI(rawUri);

    size_t prefixLength = std::strlen(getPrefix1());
    if (uri.rfind(getPrefix2(), 0) == 0) {
      prefixLength = std::strlen(getPrefix2());
    }

    if (uri.length() <= prefixLength) {
      httpResponse.body()
        << "<p class=err>Missing numbers in URI</p>\n</html>\n";
      return httpResponse.send();
    }

    const std::string numbersPart = uri.substr(prefixLength);
    std::cout << "[DEBUG] URI recibida: " << uri
              << ", números extraídos: '" << numbersPart << "'" << std::endl;

    const std::vector<std::string>& tokens = Util::split(numbersPart, ",", true);

    serveHeader(httpResponse, getTitle());
    httpResponse.body()
      << "  <p>Request: " << uri << "</p>\n"
      << "  <ol>\n";

    for (const std::string& text : tokens) {
      try {
        std::cout << "[DEBUG] Token recibido: '" << text << "'" << std::endl;
        size_t end = 0;
        const int64_t number = std::stoll(text, &end);
        if (end != text.length()) {
          throw std::runtime_error("Invalid number: " + text);
        }
        std::cout << "[DEBUG] Enviando a processNumber: " << number << std::endl;
        processNumber(number, httpResponse);
      } catch (const std::exception&) {
        httpResponse.body()
          << "    <li class=err>" << text << ": invalid number</li>\n";
      }
    }

    httpResponse.body()
      << "  </ol>\n"
      << "  <hr>\n"
      << "  <p><a href=\"/\">Back</a></p>\n"
      << "</html>\n";

    return httpResponse.send();
  }
};

#endif  // MATHWEBAPP_HPP
