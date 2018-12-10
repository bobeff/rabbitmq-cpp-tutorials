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
    if (argc < 2)
    {
      cerr << "Usage: " << argv[0] << " <binding_key>..." << endl;
      return -1;
    }

    auto channel = AmqpClient::Channel::Create();
    channel->DeclareExchange(EXCHANGE_NAME
      , AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true, true);

    auto queueName = channel->DeclareQueue("", false, true, true, true);
    clog << "Queue with name '" << queueName << "' has been declared."
         << endl;

    for (int i = 1; i < argc; ++i)
    {
      channel->BindQueue(queueName, EXCHANGE_NAME, argv[i]);
    }

    auto consumerTag = channel->BasicConsume(queueName, "", true, true, true);

    while (true)
    {
      auto envelop = channel->BasicConsumeMessage(consumerTag);
      clog << " [x] Received: '" << envelop->RoutingKey() << ":"
           << envelop->Message()->Body() << "'" << endl;
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
