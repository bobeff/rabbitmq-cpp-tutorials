#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <sstream>

using namespace std;

constexpr auto QUEUE_NAME = "rpc_queue";

class RpcClient
{
public:
  RpcClient()
    : m_channel(AmqpClient::Channel::Create())
    , m_callbackQueue(m_channel->DeclareQueue("", false, true, false, true))
    , m_consumerTag(m_channel->BasicConsume(
        m_callbackQueue, "", true, true, false))
  {
  }

  uint64_t fibonacci(uint16_t n)
  {
    ostringstream guid;
    guid << boost::uuids::random_generator()();

    auto requestMessage = AmqpClient::BasicMessage::Create();
    requestMessage->Body(to_string(n));
    requestMessage->ReplyTo(m_callbackQueue);
    requestMessage->CorrelationId(guid.str());
    requestMessage->DeliveryMode(AmqpClient::BasicMessage::dm_persistent);
    m_channel->BasicPublish("", QUEUE_NAME, requestMessage);

    AmqpClient::Envelope::ptr_t envelop;
    bool success = m_channel->BasicConsumeMessage(
      m_consumerTag, envelop, 1000);
    if (success)
    {
      return boost::lexical_cast<uint64_t>(envelop->Message()->Body());
    }
    else
    {
      return 0;
    }
  }

private:
  AmqpClient::Channel::ptr_t m_channel;
  string m_callbackQueue;
  string m_consumerTag;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      cerr << "Usage: " << argv[0] << " <number>..." << endl;
      return -1;
    }

    RpcClient rpcClient;

    for (int i = 1; i < argc; ++i)
    {
      try
      {
        auto f = rpcClient.fibonacci(boost::lexical_cast<uint16_t>(argv[i]));
        cout << "Fibonacci(" << argv[i] << ") = ";
        if (f != 0)
        {
          cout << f << endl;
        }
        else
        {
          cout << "cannot be calculated!" << endl;
        }
      }
      catch (const boost::bad_lexical_cast& error)
      {
        cerr << error.what() << endl;
      }
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
