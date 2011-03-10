#ifndef EVAL_TUNA_CLIENT_H_
#define EVAL_TUNA_CLIENT_H_

#include <string>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/Tuna.h"

using namespace eval::tuna;
using namespace eval::model;

namespace eval { 

class TunaClient {
 public:

  TunaClient(const std::string &ip, const int port) {
    boost::shared_ptr<apache::thrift::transport::TTransport> socket(
      new apache::thrift::transport::TSocket(ip, port));

    transport_ = boost::shared_ptr<apache::thrift::transport::TTransport>(
      new apache::thrift::transport::TBufferedTransport(socket));

    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(
      new apache::thrift::protocol::TBinaryProtocol(transport_));

    client_ = boost::shared_ptr< ::TunaClient>(
      new ::TunaClient(protocol));

    try {
      transport_->open();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this
    }
  }

  ~TunaClient() {
    try {
      transport_->close();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this
    }
  }

  bool ping() {
    return client_->ping();
  }

  boost::shared_ptr<simpleQ> simpleQuery(const std::string & qname, const
      std::vector< std::string > & data) {
    boost::shared_ptr<simpleQ> return_(
      new simpleQ());

    client_->simpleQuery(*return_, qname, data);
    return return_;
  }

  boost::shared_ptr<doubleQ> doubleQuery(const std::string & qname, const
      std::vector< std::string > & data) {
    boost::shared_ptr<doubleQ> return_(
      new doubleQ());

    client_->doubleQuery(*return_, qname, data);
    return return_;
  }

  boost::shared_ptr<fullQ> fullQuery(const std::string & qname, const
      std::vector< std::string > & data) {
    boost::shared_ptr<fullQ> return_(
      new fullQ());

    client_->fullQuery(*return_, qname, data);
    return return_;
  }

  boost::shared_ptr<simpleQ> reserveFrom(const std::string & qname, const
      std::vector< std::string > & data) {
    boost::shared_ptr<simpleQ> return_(
      new simpleQ());

    client_->reserveFrom(*return_, qname, data);
    return return_;
  }

  void reserve(const std::vector< int > & ids) {
    client_->reserve(ids);
  }

  bool destroy(const int id) {
    return client_->destroy(id);
  }

  boost::shared_ptr<Task> getTask(const int id) {
    boost::shared_ptr<Task> return_(
      new Task());

    client_->getTask(*return_, id);
    return return_;
  }

  boost::shared_ptr< std::vector< Task > > getTasks(const std::vector< int > &
      ids) {
    boost::shared_ptr< std::vector< Task > > return_(
      new std::vector< Task >());

    client_->getTasks(*return_, ids);
    return return_;
  }

  boost::shared_ptr< std::vector< Task > > getTasksFrom(const std::string &
      qname, const std::vector< std::string > & data) {
    boost::shared_ptr< std::vector< Task > > return_(
      new std::vector< Task >());

    client_->getTasksFrom(*return_, qname, data);
    return return_;
  }

  bool updateTask(const Task & task) {
    return client_->updateTask(task);
  }

  int insertTask(const Task & task) {
    return client_->insertTask(task);
  }

  boost::shared_ptr<TestCase> getTestCase(const int id) {
    boost::shared_ptr<TestCase> return_(
      new TestCase());

    client_->getTestCase(*return_, id);
    return return_;
  }

  boost::shared_ptr< std::vector< TestCase > > getTestCases(const std::vector<
      int > & ids) {
    boost::shared_ptr< std::vector< TestCase > > return_(
      new std::vector< TestCase >());

    client_->getTestCases(*return_, ids);
    return return_;
  }

  boost::shared_ptr< std::vector< TestCase > > getTestCasesFrom(const
      std::string & qname, const std::vector< std::string > & data) {
    boost::shared_ptr< std::vector< TestCase > > return_(
      new std::vector< TestCase >());

    client_->getTestCasesFrom(*return_, qname, data);
    return return_;
  }

  bool updateTestCase(const TestCase & test_case) {
    return client_->updateTestCase(test_case);
  }

  int insertTestCase(const TestCase & test_case) {
    return client_->insertTestCase(test_case);
  }

  boost::shared_ptr<Submission> getSubmission(const int id) {
    boost::shared_ptr<Submission> return_(
      new Submission());

    client_->getSubmission(*return_, id);
    return return_;
  }

  boost::shared_ptr< std::vector< Submission > > getSubmissions(const
      std::vector< int > & ids) {
    boost::shared_ptr< std::vector< Submission > > return_(
      new std::vector< Submission >());

    client_->getSubmissions(*return_, ids);
    return return_;
  }

  boost::shared_ptr< std::vector< Submission > > getSubmissionsFrom(const
      std::string & qname, const std::vector< std::string > & data) {
    boost::shared_ptr< std::vector< Submission > > return_(
      new std::vector< Submission >());

    client_->getSubmissionsFrom(*return_, qname, data);
    return return_;
  }

  bool updateSubmission(const Submission & submission) {
    return client_->updateSubmission(submission);
  }

  int insertSubmission(const Submission & submission) {
    return client_->insertSubmission(submission);
  }

  boost::shared_ptr<Job> getJob(const int id) {
    boost::shared_ptr<Job> return_(
      new Job());

    client_->getJob(*return_, id);
    return return_;
  }

  boost::shared_ptr< std::vector< Job > > getJobs(const std::vector< int > &
      ids) {
    boost::shared_ptr< std::vector< Job > > return_(
      new std::vector< Job >());

    client_->getJobs(*return_, ids);
    return return_;
  }

  boost::shared_ptr< std::vector< Job > > getJobsFrom(const std::string & qname,
      const std::vector< std::string > & data) {
    boost::shared_ptr< std::vector< Job > > return_(
      new std::vector< Job >());

    client_->getJobsFrom(*return_, qname, data);
    return return_;
  }

  bool updateJob(const Job & job) {
    return client_->updateJob(job);
  }

  int insertJob(const Job & job) {
    return client_->insertJob(job);
  }

 private:
  boost::shared_ptr< ::TunaClient> client_;
  boost::shared_ptr<apache::thrift::transport::TTransport> transport_;

};

}  // namespace

#endif  // EVAL_TUNA_CLIENT_H_
