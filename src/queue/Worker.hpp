/**
 * Copyright 2011 Matija Osrecki
 *
 * TODO copy notice
 */

#ifndef EVAL_QUEUE_WORKER_HPP_
#define EVAL_QUEUE_WORKER_HPP_

#include <string>

  /**
   * Basic information about worker process - IP address and port.
   * 'localhost' can stand instead of the IP address.
   */
class Worker { 

 public:

  /**
   * Default contructor.
   */
  Worker() {}

  /**
   * Plain setter constructor.
   */
  Worker(std::string ip, int port) : ip_(ip), port_(port) {}
    
  /**
   * Returns worker's IP address.
   */
  inline std::string ip() {  return ip_;  }
 
  /**
   * Returns worker's port.
   */
  inline int port() {  return port_;  }
  
 private:

  std::string ip_;  
  int port_;
  
};

#endif // EVAL_QUEUE_WORKER_HPP_
