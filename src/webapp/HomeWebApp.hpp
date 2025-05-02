#ifndef HOMEWEBAPP_HPP
#define HOMEWEBAPP_HPP

#include "HttpApp.hpp"

class HomeWebApp : public HttpApp {
 public:
  HomeWebApp() = default;
  ~HomeWebApp() = default;

  void start() override {}
  void stop() override {}

  bool handleHttpRequest(HttpRequest& httpRequest, HttpResponse& httpResponse) override;
};

#endif  // HOMEWEBAPP_HPP
