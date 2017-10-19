#include "MessageQueue.h"

namespace argeo
{
	MessageQueue::MessageQueue()
		: m_state(State::Stopped)
		, m_queue_message_arrived_notified(false)
	{ }

	MessageQueue::~MessageQueue()
	{
		terminate();
	}

	void MessageQueue::start_in_current_thread()
	{
		{ // Lock Queue Mutex
			std::unique_lock<std::mutex> lock(m_queue_mutex);

			if (m_state != State::Stopped)
			{
				throw std::runtime_error("The MessageQueue is already running.");
			}

			m_state = State::Running;

		} // UnLock Queue Mutex

		run();
	}

	void MessageQueue::start_in_another_thread()
	{
		{ // Lock Queue Mutex
			std::unique_lock<std::mutex> lock(m_queue_mutex);

			if (m_state != State::Stopped)
			{
				throw std::runtime_error("The MessageQueue is already running.");
			}

			m_state = State::Running;
		} // UnLock Queue Mutex

		std::thread thread(&MessageQueue::run, this);
		thread.detach();
	}

	void MessageQueue::post(Callback callback, void* data)
	{
		{ // Lock Queue Mutex
			std::unique_lock<std::mutex> lock(m_queue_mutex);

			m_queue.push_back(new MessageInfo(callback, data));

			m_queue_message_arrived_notified = true;
			m_queue_message_arrived.notify_one();
		} // UnLock Queue Mutex
	}

	void MessageQueue::post(void* data)
	{
		{
			std::unique_lock<std::mutex> lock(m_queue_mutex);
			m_queue.push_back(new MessageInfo(nullptr, data));
			m_queue_message_arrived_notified = true;
			m_queue_message_arrived.notify_one();
		}
	}

	void MessageQueue::send(Callback callback, void* data)
	{
		MessageInfo* message_info = new MessageInfo(callback, data);

		std::unique_lock<std::mutex> queue_lock(m_queue_mutex);
		m_queue.push_back(message_info);
		m_queue_message_arrived_notified = true;
		m_queue_message_arrived.notify_one();

		std::unique_lock<std::mutex> message_lock(message_info->done);
		while (!message_info->notified)
		{
			message_info->condition.wait(message_lock);
		}
		message_info->notified = false;
	}

	void MessageQueue::send(void* data)
	{
		MessageInfo* message_info = new MessageInfo(nullptr, data);

		std::unique_lock<std::mutex> queue_lock(m_queue_mutex);

		m_queue.push_back(message_info);
		m_queue_message_arrived_notified = true;
		m_queue_message_arrived.notify_one();

		std::unique_lock<std::mutex> message_lock(message_info->done);
		while (!message_info->notified)
		{
			message_info->condition.wait(message_lock);
		}
		message_info->notified = false;
	}

	void MessageQueue::wait_for_message()
	{
		std::unique_lock<std::mutex> queue_lock(m_queue_mutex);

		while (m_queue.size() == 0)
		{
			while (!m_queue_message_arrived_notified)
			{
				m_queue_message_arrived.wait(queue_lock);
			}
			m_queue_message_arrived_notified = false;
		}
	}

	void MessageQueue::run()
	{
		try
		{
			std::vector<MessageInfo*> current;
			do
			{
				{ // Lock Queue Mutex
					std::unique_lock<std::mutex> lock(m_queue_mutex);

					if (m_queue.size() > 0)
					{
						current.insert(current.end(), m_queue.begin(), m_queue.end());
						m_queue.clear();
					}
					else
					{
						while (!m_queue_message_arrived_notified)
						{
							m_queue_message_arrived.wait(lock);
						}
						m_queue_message_arrived_notified = false;
						current.insert(current.end(), m_queue.begin(), m_queue.end());
						m_queue.clear();
					}
				} // UnLock Queue Mutex

				process_current_queue(current);
				current.clear();
			} while (m_state == State::Running);
		}
		catch (std::exception e)
		{
		}

		{ // Lock Queue Mutex
			std::unique_lock<std::mutex> lock(m_queue_mutex);
			m_state = State::Stopped;
		} // UnLock Queue Mutex
	}

	void MessageQueue::process_queue()
	{
		std::vector<MessageInfo*> current;

		{ // Lock Queue Mutex
			std::unique_lock<std::mutex> lock(m_queue_mutex);

			if (m_state != State::Stopped)
				throw std::domain_error("The MessageQueue is already running.");

			if (m_queue.size() > 0)
			{
				m_state = State::Running;
				current.insert(current.end(), m_queue.begin(), m_queue.end());
				m_queue.clear();
			}
		} // UnLock Queue Mutex

		if (current.size() > 0)
		{
			try
			{
				process_current_queue(current);
			}
			catch (std::exception ex)
			{
			}

			{ // Lock Queue Mutex
				std::unique_lock<std::mutex> lock(m_queue_mutex);
				m_state = State::Stopped;
			} // UnLock Queue Mutex
		}
	}

	void MessageQueue::process_current_queue(std::vector<MessageInfo*> current)
	{
		for (int i = 0; i < current.size(); ++i)
		{
			if (m_state == State::Stopping)
			{
				// Push the remainder of 'current' back into 'm_queue'.

				{ // Lock Queue Mutex
					std::unique_lock<std::mutex> lock(m_queue_mutex);
					current.erase(current.begin(), current.begin() + i);
					m_queue.insert(m_queue.begin(), current.begin(), current.end());
				} // UnLock Queue Mutex

				break;
			}

			process_message(current[i]);
		}
	}

	void MessageQueue::process_message(MessageInfo* message)
	{
		if (message->callback != nullptr)
		{
			message->callback(message->message);
		}
		else
		{
			for (auto listener : m_listeners)
			{
				listener->on_received(MessageQueueEventArgs(message->message));
			}
		}

		std::unique_lock<std::mutex> lock(message->done);
		message->notified = true;
		message->condition.notify_one();
	}


	void MessageQueue::terminate()
	{
		post(static_cast<Callback>(&MessageQueue::stop_queue), this);
	}

	void MessageQueue::terminate_and_wait()
	{
		send(static_cast<Callback>(&MessageQueue::stop_queue), this);
	}

	void MessageQueue::stop_queue(void* data)
	{
		MessageQueue* queue = static_cast<MessageQueue*>(data);
		queue->m_state = State::Stopping;
	}

	bool MessageQueue::add_listener(MessageQueueListener* args)
	{
		return m_listeners.insert(args).second;
	}

	bool MessageQueue::remove_listener(MessageQueueListener* args)
	{
		return m_listeners.erase(args) > 0;
	}
}
