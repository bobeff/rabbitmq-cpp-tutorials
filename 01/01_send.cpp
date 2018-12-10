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
    auto message = AmqpClient::BasicMessage::Create("Hello World!");
    channel->BasicPublish("", QUEUE_NAME, message);
    clog << " [x] Sent 'Hello World!'" << endl;
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }
 
  return 0; 
}
