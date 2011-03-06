#ifndef EVAL_TUNA_CLIENT_H_
#define EVAL_TUNA_CLIENT_H_

#include <string>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/Tuna.h"

namespace eval { 

class TunaClient {
 public:

  TunaClient(const std::string &ip, const int port) {
    boost::shared_ptr<apache::thrift::transport::TTransport> socket(
      new apache::thrift::transport::TSocket(ip, port));

    boost::shared_ptr<apache::thrift::transport::TTransport> transport(
      new apache::thrift::transport::TBufferedTransport(socket));

    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(
      new apache::thrift::protocol::TBinaryProtocol(transport_));

    boost::shared_ptr< ::TunaClient> client(
      new ::TunaClient(protocol));

    client_ = client;
    transport_ = transport;
    transport_->open();
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

  simpleQ simpleQuery(const std::string& qname, const std::vector< std::string
    >& data) {
    return client_->simpleQuery(qname, data);
  }

  doubleQ doubleQuery(const std::string& qname, const std::vector< std::string
    >& data) {
    return client_->doubleQuery(qname, data);
  }

  fullQ fullQuery(const std::string& qname, const std::vector< std::string >&
    data) {
    return client_->fullQuery(qname, data);
  }

  simpleQ reserveFrom(const std::string& qname, const std::vector< std::string
    >& data) {
    return client_->reserveFrom(qname, data);
  }

  void reserve(const std::vector< int >& ids) {
    client_->reserve(ids);
  }

  bool destroy(const int id) {
    return client_->destroy(id);
  }

  models.Task getTask(const int id) {
    return client_->getTask(id);
  }

  void getTasks(std::vector< models.Task > & return_, const std::vector< int >&
    ids) {
    client_->getTasks(return_, ids);
  }

  void getTasksFrom(std::vector< models.Task > & return_, const std::string&
    qname, const std::vector< std::string >& data) {
    client_->getTasksFrom(return_, qname, data);
  }

  bool updateTask(const models.Task task) {
    return client_->updateTask(task);
  }

  int insertTask(const models.Task task) {
    return client_->insertTask(task);
  }

  models.TestCase getTestCase(const int id) {
    return client_->getTestCase(id);
  }

  void getTestCases(std::vector< models.TestCase > & return_, const std::vector<
    int >& ids) {
    client_->getTestCases(return_, ids);
  }

  void getTestCasesFrom(std::vector< models.TestCase > & return_, const
    std::string& qname, const std::vector< std::string >& data) {
    client_->getTestCasesFrom(return_, qname, data);
  }

  bool updateTestCase(const models.TestCase test_case) {
    return client_->updateTestCase(test_case);
  }

  int insertTestCase(const models.TestCase test_case) {
    return client_->insertTestCase(test_case);
  }

  models.Submission getSubmission(const int id) {
    return client_->getSubmission(id);
  }

  void getSubmissions(std::vector< models.Submission > & return_, const
    std::vector< int >& ids) {
    client_->getSubmissions(return_, ids);
  }

  void getSubmissionsFrom(std::vector< models.Submission > & return_, const
    std::string& qname, const std::vector< std::string >& data) {
    client_->getSubmissionsFrom(return_, qname, data);
  }

  bool updateSubmission(const models.Submission submission) {
    return client_->updateSubmission(submission);
  }

  int insertSubmission(const models.Submission submission) {
    return client_->insertSubmission(submission);
  }

  models.Job getJob(const int id) {
    return client_->getJob(id);
  }

  void getJobs(std::vector< models.Job > & return_, const std::vector< int >&
    ids) {
    client_->getJobs(return_, ids);
  }

  void getJobsFrom(std::vector< models.Job > & return_, const std::string&
    qname, const std::vector< std::string >& data) {
    client_->getJobsFrom(return_, qname, data);
  }

  bool updateJob(const models.Job job) {
    return client_->updateJob(job);
  }

  int insertJob(const models.Job job) {
    return client_->insertJob(job);
  }

 private:
  boost::shared_ptr< ::TunaClient> client_;
  boost::shared_ptr<apache::thrift::transport::TTransport> transport_;

};

}  // namespace

#endif  // EVAL_TUNA_CLIENT_H_
