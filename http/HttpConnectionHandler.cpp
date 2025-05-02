#include "HttpConnectionHandler.hpp"

#include "HttpApp.hpp" // se incluye para el uso de la clase HttpApp
#include "Log.hpp" // se incluye para el uso de la clase Log
#include "NetworkAddress.hpp" // se incluye para el uso de la clase NetworkAddress


// TODO(): Agarrar las conexiones de la cola

/// Constructor
HttpConnectionHandler::HttpConnectionHandler(HttpServer* httpserver,
    std::vector<HttpApp*>& applications) { 
    setServer(httpserver);

    // Conoce las aplicaciones del servidor
    for (size_t index = 0; index < applications.size(); ++index) {
        HttpApp* app = applications[index];
        if (app != nullptr) {
            this->httpApps.push_back(app);
        }
    }
}
/// Destructor
HttpConnectionHandler::~HttpConnectionHandler() {
}

int HttpConnectionHandler::run() { // empiezan las conexiones
  this->consumeLoop(); // metodo que consume las conexiones

  // TODO(): Revisar que tipo de registro se debe guardar en el log
}

void HttpConnectionHandler::setServer(HttpServer* httpserver) {
    this->server = httpserver;
    setConsumingQueue(this->server->getSocketQueue());
}

void HttpConnectionHandler::consume(Socket client) {
  // incrementar el semaforo de can_produce
  this->can_produce->signal();
  std::ostringstream oss;
  oss << "Hilo " << std::this_thread::get_id() << " está procesando socket " << client.getNetworkAddress().getIP();
  Log::append(Log::INFO, "handler", oss.str());
  // While the same client asks for HTTP requests in the same connection
  while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest httpRequest(client);

    // If the request is not valid or an error happened
    if (!httpRequest.parse()) {
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse httpResponse(client);

    // Give subclass a chance to respond the HTTP request
    const bool handled = this->handleHttpRequest(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      client.close();
      break;
    }
  }
}

void HttpConnectionHandler::connect_semaphore(Semaphore* can_produce) {
  this->can_produce = can_produce;
}
  
bool HttpConnectionHandler::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  Log::append(Log::INFO, "connection",
    std::string("connection established with client ") + address.getIP()
    + " port " + std::to_string(address.getPort()));

  // Print HTTP request
  Log::append(Log::INFO, "request", httpRequest.getMethod()
    + ' ' + httpRequest.getURI()
    + ' ' + httpRequest.getHttpVersion());

  return this->route(httpRequest, httpResponse);
}

bool HttpConnectionHandler::route(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  // Traverse the chain of applications
  for (size_t index = 0; index < this->httpApps.size(); ++index) {
    // If this application handles the request
    HttpApp* app = this->httpApps[index];
    if (app->handleHttpRequest(httpRequest, httpResponse)) {
      return true;
    }
  }

  // Unrecognized request
  return this->serveNotFound(httpRequest, httpResponse);
}

bool HttpConnectionHandler::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // TODO(you): 404 not found page should be served by a NotFoundWebApp object
  // Set HTTP response metadata (headers)
  httpResponse.setStatusCode(404);
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Not found";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resource was not found on this server.</p>\n"
    << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}