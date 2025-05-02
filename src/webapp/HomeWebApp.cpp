#include "HomeWebApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

bool HomeWebApp::handleHttpRequest(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  if (httpRequest.getMethod() != "GET" || httpRequest.getURI() != "/") {
    return false;
  }

  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  httpResponse.body()
    << "<!DOCTYPE html>\n"
    << "<html lang='en'>\n"
    << "<head>\n"
    << "  <meta charset='UTF-8'>\n"
    << "  <title>Math Web Server</title>\n"
    << "  <style>\n"
    << "    body { font-family: Arial, sans-serif; text-align: center; margin-top: 5em; background-color: #f4f4f4; }\n"
    << "    input[type='text'] { padding: 0.5em; font-size: 1em; width: 300px; }\n"
    << "    button { margin: 0.5em; padding: 0.5em 1em; font-size: 1em; cursor: pointer; }\n"
    << "    form { display: inline-block; margin-top: 2em; }\n"
    << "  </style>\n"
    << "</head>\n"
    << "<body>\n"
    << "  <h1>Math Web Server</h1>\n"
    << "  <form onsubmit=\"return false;\">\n"
    << "    <input type='text' id='numberInput' placeholder='Enter one or more numbers' required>\n"
    << "    <br>\n"
    << "    <button onclick=\"submitTo('/fact')\">Prime Factorization</button>\n"
    << "    <button onclick=\"submitTo('/goldbach')\">Goldbach Conjecture</button>\n"
    << "  </form>\n"
    << "  <script>\n"
    << "    function submitTo(endpoint) {\n"
    << "      const input = document.getElementById('numberInput').value;\n"
    << "      if (!input.trim()) {\n"
    << "        alert('Please enter a number.');\n"
    << "        return;\n"
    << "      }\n"
    << "      window.location.href = endpoint + '?number=' + encodeURIComponent(input);\n"
    << "    }\n"
    << "  </script>\n"
    << "</body>\n"
    << "</html>\n";

  return httpResponse.send();
}
