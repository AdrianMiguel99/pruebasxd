#ifndef HTTPCONNECTIONHANDLER_HPP
#define HTTPCONNECTIONHANDLER_HPP

#include <vector>

#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "Consumer.hpp"

class HttpApp;
class HttpServer;

class HttpConnectionHandler : public Consumer<Socket> {

 public:
  /// Constructor
  explicit HttpConnectionHandler(HttpServer* httpserver,
    std::vector<HttpApp*>& applications);
  /// Destructor
  virtual ~HttpConnectionHandler();

  void setServer(HttpServer* httpserver);

  int run() override;
  
  void consume(Socket socket) override;

  /// Conecta el semaforo can_produce de HttpServer con este objeto
  /// para mandar senal
  void connect_semaphore(Semaphore* can_produce);

  Semaphore* can_produce;
  protected:

  HttpServer *server;  /**< Puntero al servidor HTTP asociado. */
  std::vector<HttpApp*> httpApps;  /**< Vector de aplicaciones HTTP. */

  /// Called each time an HTTP request is received. Web server should analyze
  /// the request object and assemble a response with the response object.
  /// Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
  virtual bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse);
  /// Route, that provide an answer according to the URI value
  /// For example, home page is handled different than a number
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// Sends a page for a non found resource in this server. This method is
  /// called if none of the registered web applications handled the request.
  /// If you want to override this method, create a web app, e.g NotFoundWebApp
  /// that reacts to all URIs, and chain it as the last web app
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
};


#endif  // HTTPCONNECTIONHANDLER_H