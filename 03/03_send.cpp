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

    for (string line; getline(cin, line); )
    {
      auto message = AmqpClient::BasicMessage::Create(line);
      channel->BasicPublish(EXCHANGE_NAME, "", message);
      clog << " [x] Sent '" << line << "'" << endl;
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
