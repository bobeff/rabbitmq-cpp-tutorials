#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>

using namespace std;

constexpr auto EXCHANGE_NAME = "topic_logs";

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      cerr << "Usage: " << argv[0] << " <topic> <message>" << endl;
      return -1;
    }

    auto channel = AmqpClient::Channel::Create();
    channel->DeclareExchange(EXCHANGE_NAME
      , AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true, true);

    auto message = AmqpClient::BasicMessage::Create(argv[2]);
    channel->BasicPublish(EXCHANGE_NAME, argv[1], message);
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
