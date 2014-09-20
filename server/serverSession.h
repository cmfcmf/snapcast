#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include <boost/asio.hpp>
#include <condition_variable>
#include <set>
#include "common/message.h"
#include "common/queue.h"


using boost::asio::ip::tcp;


class ServerSession;

class MessageReceiver
{
public:
	virtual void onMessageReceived(ServerSession* connection, const BaseMessage& baseMessage, char* buffer) = 0;
};


class ServerSession
{
public:
	ServerSession(MessageReceiver* _receiver, std::shared_ptr<tcp::socket> _socket);
	void start();
	bool send(BaseMessage* message);
	void add(std::shared_ptr<BaseMessage> message);
	virtual bool connected()
	{
		return (socket != 0);
//		return (connected_ && socket);
	}

	virtual bool active()
	{
		return active_;
	}


protected:
	void socketRead(void* _to, size_t _bytes);
	void getNextMessage();
	void reader();
	void writer();

	std::atomic<bool> active_;
	mutable std::mutex mutex_;
	std::thread* readerThread;
	std::thread* writerThread;
	std::shared_ptr<tcp::socket> socket;
	MessageReceiver* messageReceiver;
	Queue<std::shared_ptr<BaseMessage>> messages;
};




#endif



