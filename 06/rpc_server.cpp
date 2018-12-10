#pragma warning(push, 0)
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#pragma warning(pop)

#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace std;

constexpr auto QUEUE_NAME = "rpc_queue";

constexpr uint8_t FIBONACCI_NUMBERS_COUNT = 93;
vector<uint64_t> g_fibonacciNumbers;

void generateFibonacci()
{
  g_fibonacciNumbers.resize(FIBONACCI_NUMBERS_COUNT);
  g_fibonacciNumbers[0] = 1;
  g_fibonacciNumbers[1] = 1;
  for (int i = 2; i < FIBONACCI_NUMBERS_COUNT; ++i)
  {
    g_fibonacciNumbers[i] =
      g_fibonacciNumbers[i - 1] + g_fibonacciNumbers[i - 2];
  }
}

int main()
{
  try
  {
    generateFibonacci();

    auto channel = AmqpClient::Channel::Create();
    channel->DeclareQueue(QUEUE_NAME, false, true, false, true);

    auto consumerTag = channel->BasicConsume(QUEUE_NAME, ""
      , true, false, false);
    clog << "Consumer tag: " << consumerTag << endl;

    while (true)
    {
      auto envelop = channel->BasicConsumeMessage(consumerTag);
      auto requestMessage = envelop->Message();
      auto messageBody = requestMessage->Body();
      clog << " [x] Received: " << messageBody << endl;

      uint16_t n = 0;
      try
      {
        n = boost::lexical_cast<uint16_t>(messageBody);
      }
      catch (const boost::bad_lexical_cast& error)
      {
        cerr << error.what() << endl;
        channel->BasicAck(envelop);
        continue;
      }

      if (n >= FIBONACCI_NUMBERS_COUNT)
      {
        cerr << "Fibonacci(" << n << ") is too big to be calculated." << endl;
        channel->BasicAck(envelop);
        continue;
      }

      auto fibN = g_fibonacciNumbers[n];
      clog << "Fibonacci(" << n << ") = " << fibN << endl;

      auto responseMessage = AmqpClient::BasicMessage::Create();
      responseMessage->Body(to_string(fibN));
      responseMessage->CorrelationId(requestMessage->CorrelationId());

      channel->BasicPublish("", requestMessage->ReplyTo(), responseMessage);
      channel->BasicAck(envelop);
    }
  }
  catch (const exception& error)
  {
    cerr << error.what() << endl;
  }

  return 0;
}
