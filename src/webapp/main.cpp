// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "HttpServer.hpp"
#include "HomeWebApp.hpp"
#include "FactWebApp.hpp"
#include "GoldbachWebApp.hpp"

// TODO(you): Register a signal handler for Ctrl+C and kill, and stop the server
// TODO(you): Make your signal handler to print the thread id running it

/// Start the web server
int main(int argc, char* argv[]) {
  // Create the web server
  HttpServer httpServer;
  // Create web applications
  HomeWebApp homeWebApp;
  FactWebApp factWebApp;
  GoldbachWebApp goldbachWebApp;
  // Register the web application(s) with the web server
  httpServer.chainWebApp(&homeWebApp);
  httpServer.chainWebApp(&factWebApp);
  httpServer.chainWebApp(&goldbachWebApp);
  // Run the web server
  return httpServer.run(argc, argv);
}

#endif  // WEBSERVER
