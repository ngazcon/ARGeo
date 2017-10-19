#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_set>

namespace argeo
{
	typedef void(*Callback)(void *);

	struct MessageQueueEventArgs
	{
		MessageQueueEventArgs(void* message)
			: message(message)
		{ }

		void* message;
	};

	class MessageQueueListener
	{
	public:
		virtual void on_received(MessageQueueEventArgs args) = 0;
	};

	class MessageInfo
	{
	public:

		MessageInfo()
			: callback(nullptr)
			, message(nullptr)
			, notified(false)
		{ }

		MessageInfo(
			Callback callback, 
			void* message)
			: callback(callback)
			, message(message)
			, notified(false)
		{ }

		Callback callback;
		void* message;

		std::mutex done;
		std::condition_variable condition;
		bool notified;
	};

	class MessageQueue
	{
		enum State
		{
			Stopped,
			Running,
			Stopping,
		};

	public:
		MessageQueue();
		~MessageQueue();

		void start_in_current_thread();
		void start_in_another_thread();

		void process_queue();

		void terminate();
		void terminate_and_wait();

		void post(Callback callback, void* data);
		void post(void* data);

		void send(Callback callback, void* data);
		void send(void* data);

		void wait_for_message();

		bool remove_listener(MessageQueueListener* listener);
		bool add_listener(MessageQueueListener* listener);

	private:

		void run();
		void process_current_queue(std::vector<MessageInfo*> queue);
		void process_message(MessageInfo* message);

		static void stop_queue(void* data);

	private:
		std::unordered_set<MessageQueueListener*> m_listeners;
		std::vector<MessageInfo*> m_queue;

		std::mutex m_queue_mutex;
		std::condition_variable m_queue_message_arrived;
		bool m_queue_message_arrived_notified;

		State m_state;
	};
}
