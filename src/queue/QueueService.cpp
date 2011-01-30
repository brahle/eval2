/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#include "QueueService.h"



uint32_t QueueService_ping_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t QueueService_ping_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("QueueService_ping_args");
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t QueueService_ping_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("QueueService_ping_pargs");
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t QueueService_ping_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->success);
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t QueueService_ping_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("QueueService_ping_result");

  if (this->__isset.success) {
    xfer += oprot->writeFieldBegin("success", ::apache::thrift::protocol::T_BOOL, 0);
    xfer += oprot->writeBool(this->success);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t QueueService_ping_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool((*(this->success)));
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t QueueService_addToQueue_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->submissionID);
          this->__isset.submissionID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->taskType);
          this->__isset.taskType = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->payload);
          this->__isset.payload = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t QueueService_addToQueue_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("QueueService_addToQueue_args");
  xfer += oprot->writeFieldBegin("submissionID", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->submissionID);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("taskType", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->taskType);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("payload", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->payload);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t QueueService_addToQueue_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("QueueService_addToQueue_pargs");
  xfer += oprot->writeFieldBegin("submissionID", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((*(this->submissionID)));
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("taskType", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((*(this->taskType)));
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("payload", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString((*(this->payload)));
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t QueueService_addToQueue_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t QueueService_addToQueue_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("QueueService_addToQueue_result");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t QueueService_addToQueue_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

bool QueueServiceClient::ping()
{
  send_ping();
  return recv_ping();
}

void QueueServiceClient::send_ping()
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("ping", ::apache::thrift::protocol::T_CALL, cseqid);

  QueueService_ping_pargs args;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->flush();
  oprot_->getTransport()->writeEnd();
}

bool QueueServiceClient::recv_ping()
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::INVALID_MESSAGE_TYPE);
  }
  if (fname.compare("ping") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::WRONG_METHOD_NAME);
  }
  bool _return;
  QueueService_ping_presult result;
  result.success = &_return;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  if (result.__isset.success) {
    return _return;
  }
  throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::MISSING_RESULT, "ping failed: unknown result");
}

void QueueServiceClient::addToQueue(const int32_t submissionID, const int32_t taskType, const std::string& payload)
{
  send_addToQueue(submissionID, taskType, payload);
  recv_addToQueue();
}

void QueueServiceClient::send_addToQueue(const int32_t submissionID, const int32_t taskType, const std::string& payload)
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("addToQueue", ::apache::thrift::protocol::T_CALL, cseqid);

  QueueService_addToQueue_pargs args;
  args.submissionID = &submissionID;
  args.taskType = &taskType;
  args.payload = &payload;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->flush();
  oprot_->getTransport()->writeEnd();
}

void QueueServiceClient::recv_addToQueue()
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::INVALID_MESSAGE_TYPE);
  }
  if (fname.compare("addToQueue") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::WRONG_METHOD_NAME);
  }
  QueueService_addToQueue_presult result;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  return;
}

bool QueueServiceProcessor::process(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot) {

  ::apache::thrift::protocol::TProtocol* iprot = piprot.get();
  ::apache::thrift::protocol::TProtocol* oprot = poprot.get();
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;
  int32_t seqid;

  iprot->readMessageBegin(fname, mtype, seqid);

  if (mtype != ::apache::thrift::protocol::T_CALL && mtype != ::apache::thrift::protocol::T_ONEWAY) {
    iprot->skip(::apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::INVALID_MESSAGE_TYPE);
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->flush();
    oprot->getTransport()->writeEnd();
    return true;
  }

  return process_fn(iprot, oprot, fname, seqid);
}

bool QueueServiceProcessor::process_fn(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid) {
  std::map<std::string, void (QueueServiceProcessor::*)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*)>::iterator pfn;
  pfn = processMap_.find(fname);
  if (pfn == processMap_.end()) {
    iprot->skip(::apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::UNKNOWN_METHOD, "Invalid method name: '"+fname+"'");
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->flush();
    oprot->getTransport()->writeEnd();
    return true;
  }
  (this->*(pfn->second))(seqid, iprot, oprot);
  return true;
}

void QueueServiceProcessor::process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot)
{
  QueueService_ping_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  iprot->getTransport()->readEnd();

  QueueService_ping_result result;
  try {
    result.success = iface_->ping();
    result.__isset.success = true;
  } catch (const std::exception& e) {
    ::apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("ping", ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->flush();
    oprot->getTransport()->writeEnd();
    return;
  }

  oprot->writeMessageBegin("ping", ::apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  oprot->getTransport()->flush();
  oprot->getTransport()->writeEnd();
}

void QueueServiceProcessor::process_addToQueue(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot)
{
  QueueService_addToQueue_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  iprot->getTransport()->readEnd();

  QueueService_addToQueue_result result;
  try {
    iface_->addToQueue(args.submissionID, args.taskType, args.payload);
  } catch (const std::exception& e) {
    ::apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("addToQueue", ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->flush();
    oprot->getTransport()->writeEnd();
    return;
  }

  oprot->writeMessageBegin("addToQueue", ::apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  oprot->getTransport()->flush();
  oprot->getTransport()->writeEnd();
}



