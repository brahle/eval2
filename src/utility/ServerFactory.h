/**
 * Copyright 2011 Matija Osrecki
 *
 * This file is part of Evaluator.
 * 
 * Evaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * Evaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public
 * License along with Evaluator. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include <protocol/TBinaryProtocol.h>
#include <transport/TBufferTransports.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>


using apache::thrift::TProcessor;
using apache::thrift::server::TServerTransport;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TTransportFactory;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using boost::shared_ptr;

namespace eval { namespace util {

/**
   Templated class for creating thrift servers. 

   H is handler class (e.g. eval::WorkerHandler - implements WorkerIf, 
   as created in the server skeleton).
   
   P is processor class (e.g. WorkerProcessor)

   S is server class (e.g. apache::thrift::server::TSimpleServer)
 */
template <class H, class P, class S>
class ServerFactory {
 public:

  static shared_ptr<S> createServer(const int port) {
    shared_ptr<H> handler(new H());

    shared_ptr<TProcessor> processor(
        new P(handler));
    
    shared_ptr<TServerTransport> serverTransport(
        new TServerSocket(port));

    shared_ptr<TTransportFactory> transportFactory(
        new TBufferedTransportFactory());

    shared_ptr<TProtocolFactory> protocolFactory(
        new TBinaryProtocolFactory());

    shared_ptr<S> server(
      new S(
        processor, 
        serverTransport, 
        transportFactory, 
        protocolFactory
      )
    );

    return server;
  }
  
};

} }  // namespace
