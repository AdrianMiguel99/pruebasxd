// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Concurrent web server entry point

#ifdef WEBSERVER

#include <csignal>
#include <thread>
#include <iostream>
#include <atomic>

#include "HttpServer.hpp"
#include "HomeWebApp.hpp"
#include "FactWebApp.hpp"
#include "GoldbachWebApp.hpp"

// Flag atómico para manejar parada por señal (Ctrl+C)
std::atomic<bool> stop_requested(false);

// Manejador de señales
void handleSignal(int signal) {
  std::cout << "[SIGNAL] Caught signal " << signal
            << " in thread " << std::this_thread::get_id() << std::endl;
  stop_requested.store(true);
}

int main(int argc, char* argv[]) {
  // Registrar manejadores de señal
  std::signal(SIGINT, handleSignal);
  std::signal(SIGTERM, handleSignal);

  // Crear servidor
  HttpServer httpServer;

  // Crear y registrar las aplicaciones web
  HomeWebApp homeWebApp;
  FactWebApp factWebApp;
  GoldbachWebApp goldbachWebApp;

  httpServer.chainWebApp(&homeWebApp);
  httpServer.chainWebApp(&factWebApp);
  httpServer.chainWebApp(&goldbachWebApp);

  // Ejecutar el servidor (bloqueante)
  int result = httpServer.run(argc, argv);

  // Si se solicitó parada desde la señal, detener el servidor
  if (stop_requested.load()) {
    httpServer.stopListening();  // libera accept()
  }

  return result;
}

#endif  // WEBSERVER
