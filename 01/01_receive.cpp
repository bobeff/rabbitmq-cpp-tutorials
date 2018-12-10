#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>

using namespace std;

constexpr auto QUEUE_NAME = "hello";

int main()
{
  try
  {
    auto channel = AmqpClient::Channel::Create();
    channel->DeclareQueue(QUEUE_NAME, false, true, false, true);
    auto consumerTag = channel->BasicConsume(QUEUE_NAME);
    clog << "Consumer tag: " << consumerTag << endl;
    auto envelop = channel->BasicConsumeMessage(consumerTag);
    clog << " [x] Received " << envelop->Message()->Body() << endl;
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0; 
}
