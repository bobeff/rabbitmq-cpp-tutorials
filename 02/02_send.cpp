#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>

using namespace std;

constexpr auto QUEUE_NAME = "task_queue";

int main(int argc, char* argv[])
{
  try
  {
    auto channel = AmqpClient::Channel::Create();
    channel->DeclareQueue(QUEUE_NAME, false, true, false, true);

    for (int i = 1; i < argc; ++i)
    {
      auto message = AmqpClient::BasicMessage::Create(argv[i]);
      channel->BasicPublish("", QUEUE_NAME, message, false, false);
      clog << " [x] Sent '" << argv[i] << "'" << endl;
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
