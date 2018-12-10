#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>
#include <thread>

using namespace std;

constexpr auto QUEUE_NAME = "task_queue";

int main()
{
  try
  {
    auto channel = AmqpClient::Channel::Create();
    channel->DeclareQueue(QUEUE_NAME, false, true, false, true);

    auto consumerTag = channel->BasicConsume(QUEUE_NAME, ""
      , true, false, false);
    clog << "Consumer tag: " << consumerTag << endl;

    while (true)
    {
      auto envelop = channel->BasicConsumeMessage(consumerTag);
      auto messagePayload = envelop->Message()->Body();
      clog << " [x] Received " << messagePayload << endl;

      try
      {
        this_thread::sleep_for(chrono::seconds(stoi(messagePayload)));
      }
      catch (const exception& error)
      {
        cerr << error.what() << endl;
      }

      clog << " [x] Done" << endl;
      channel->BasicAck(envelop);
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
