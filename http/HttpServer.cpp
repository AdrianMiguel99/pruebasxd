// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "Queue.hpp"
#include "HttpApp.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"

// TODO(you): Implement connection handlers argument
const char* const usage =
  "Usage: webserv [port] [handlers]\n"
  "\n"
  "  port        Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  handlers     Number of connection handler theads\n"
  "  queue        Max capacity of connections in queue\n";

HttpServer::HttpServer() {
}

HttpServer::~HttpServer() {
  delete this->connectionQueue;
  for (HttpConnectionHandler* handler : this->connectionHandlers) {
    delete handler;
  }
  this->connectionHandlers.clear();
  
  if (this->connectionQueue != nullptr) {
    delete this->connectionQueue;
  }
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->applications.push_back(application);
}

int HttpServer::run(int argc, char* argv[]) {
  bool stopApps = false;
  try {
    if (this->analyzeArguments(argc, argv)) {
      // Create the objects required to respond to the client
      stopApps = this->startServer();
      // Accept all client connections. The main process will get blocked
      // running this method and will not return. When HttpServer::stopListening
      // is called from another execution thread, an exception will be launched
      // that stops the acceptAllConnections() invocation and enters in the
      // catch below. Then, the main thread can continue stopping apps,
      /// finishing the server and any further cleaning it requires.
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }
  // Destroy objects created by this server
  this->stopServer(stopApps);
  return EXIT_SUCCESS;
}

bool HttpServer::startServer() {
  // Start the log service
  Log::getInstance().start();
  // Start all web applications
  this->startApps();
  // Start waiting for connections
  // TODO(you): Optionally log the main thread id

  // TODO(): Inicializar la cola de clientes.
  this->connectionQueue = new Queue<Socket>();

  /// Se inicializa el semaforo con la capacidad maxima de la cola
  this->can_produce = new Semaphore(maxQueueCapacity);

  // TODO(): Inicializar la cola de connecctionHandler.
  // Crear y lanzar los threads manejadores de conexión
  for (int i = 0; i < this->maxConnectionHandlers; ++i) {
    HttpConnectionHandler* handler = new HttpConnectionHandler(this, this->applications);
    handler->connect_semaphore(this->can_produce); // conecta can_produce semaforo
    this->connectionHandlers.push_back(handler);
    handler->startThread();  // inicia el hilo
  }
  
  // TODO(): Conectar la cola de clientes con connectionHandler 
  // (llamar a la funcion setServer en loop).

  this->listenForConnections(this->port);
  const NetworkAddress& address = this->getNetworkAddress();
  Log::append(Log::INFO, "webserver", "Listening on " + address.getIP()
    + " port " + std::to_string(address.getPort()));
  return true;
}

void HttpServer::startApps() {
  for (size_t index = 0; index < this->applications.size(); ++index) {
    this->applications[index]->start();
  }
}

void HttpServer::stopApps() {
  // Stop web applications. Give them an opportunity to clean up
  for (size_t index = 0; index < this->applications.size(); ++index) {
    this->applications[index]->stop();
  }
}

void HttpServer::stopServer(const bool stopApps) {
  // Stop listening for incoming client connection requests. When stopListing()
  // method is called -maybe by a secondary thread-, the web server -running
  // by the main thread- will stop executing the acceptAllConnections() method.
  // TODO(you): stopListening must be called from a signal handler, not here
  this->stopListening();

  // If applications were started
  if (stopApps) {
    this->stopApps();
  }
  // Stop the log service
  Log::getInstance().stop();
}

bool HttpServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
    this->maxConnectionHandlers = std::stoi(argv[2]);
    this->maxQueueCapacity = std::stoi(argv[3]);
  }

  return true;
}

void HttpServer::handleClientConnection(Socket& client) {
  // TODO(x): Meter el cliente a la cola.
  // Suponemos que tenemos un semaforo
  if (this->can_produce->try_wait()) {
    this->connectionQueue->enqueue(client);
  } else {
    // TODO(): Agregar un mensaje de error al log (cola de espera llena, el paquete se perdio).
    const NetworkAddress& address = client.getNetworkAddress();
    std::ostringstream oss;
    oss << "Conexión rechazada de " << address.getIP()
        << ":" << address.getPort()
        << " → cola de espera llena, paquete descartado";
    Log::append(Log::WARNING, "connection", oss.str());

    client.close();  // Muy importante: cerrar el socket para evitar fugas
  }
}

// TODO(you): Move the following methods to your HttpConnectionHandler

bool HttpServer::handleHttpRequest(HttpRequest& httpRequest,
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

// TODO(you): Provide HttpConnectionHandler access to the array of web apps

bool HttpServer::route(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  // Traverse the chain of applications
  for (size_t index = 0; index < this->applications.size(); ++index) {
    // If this application handles the request
    HttpApp* app = this->applications[index];
    if (app->handleHttpRequest(httpRequest, httpResponse)) {
      return true;
    }
  }

  // Unrecognized request
  return this->serveNotFound(httpRequest, httpResponse);
}

Queue<Socket>* HttpServer::getSocketQueue() {
  return this->connectionQueue;
}

bool HttpServer::serveNotFound(HttpRequest& httpRequest
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
