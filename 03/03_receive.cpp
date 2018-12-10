#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>

using namespace std;

constexpr auto EXCHANGE_NAME = "logs";

int main()
{
  try
  {
    auto channel = AmqpClient::Channel::Create();
    channel->DeclareExchange(EXCHANGE_NAME
      , AmqpClient::Channel::EXCHANGE_TYPE_FANOUT, false, true, true);
    auto queueName = channel->DeclareQueue("", false, true, true, true);
    clog << "Queue with name '" << queueName << "' has been declared." << endl;
    channel->BindQueue(queueName, EXCHANGE_NAME);
    auto consumerTag = channel->BasicConsume(queueName, "", true, true, true);

    while (true)
    {
      auto envelop = channel->BasicConsumeMessage(consumerTag);
      clog << " [x] Received: '" << envelop->Message()->Body() << "'" << endl;
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
