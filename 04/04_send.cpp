#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <iostream>
#include <algorithm>

using namespace std;

constexpr auto EXCHANGE_NAME = "direct_logs";

int main(int argc, char* argv[])
{
  try
  {
    string severities[] = { "info", "warning", "error" };

    if (argc != 3)
    {
      cerr << "Usage: " << argv[0]
           << " <info | warning | error> <message_payload>" << endl;
      return -1;
    }

    auto severity = string(argv[1]);
    if (find(cbegin(severities), cend(severities), severity) ==
        cend(severities))
    {
      cerr << "Invalid severity: " << severity << endl;
      return -2;
    }

    auto messagePayload = string(argv[2]);

    auto channel = AmqpClient::Channel::Create();
    channel->DeclareExchange(EXCHANGE_NAME
      , AmqpClient::Channel::EXCHANGE_TYPE_DIRECT, false, true, true);

    auto message = AmqpClient::BasicMessage::Create(messagePayload);
    channel->BasicPublish(EXCHANGE_NAME, severity, message);
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
