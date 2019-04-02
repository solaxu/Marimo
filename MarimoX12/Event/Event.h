#ifndef EVENT_H
#define EVENT_H 

#include <vector>
#include <functional>
#include <memory>
#include <map>

class Event;
class EventDelegate;
class IListener;
typedef std::shared_ptr<IListener> ListenerPtr;
class IDispatcher;
typedef std::shared_ptr<IDispatcher> DispatcherPtr;

class Event
{
public:
	Event(const std::wstring& id) :m_sEventID(id) {}
	virtual ~Event() {}
	const std::wstring& getEventID() { return m_sEventID; }
protected:
	std::wstring m_sEventID;
};

class EventDelegate
{
protected:
	typedef std::function<void(const Event& event)> DelegateHandler;

	struct FuntionBinding
	{
		std::wstring m_name;
		DelegateHandler m_handler;

		bool operator==(const FuntionBinding& binding)
		{
			return binding.m_name == m_name;
		}
	};

public:
	void append(const std::wstring& name, void(*fn)(const Event& event))
	{
		DelegateHandler func = fn;

		FuntionBinding binding;
		binding.m_handler = func;
		binding.m_name = name;
		
		m_handlers.push_back(binding);
	}

	template<typename ObjType>
	void append(const std::wstring& name, ObjType* caller, void(ObjType::*fn)(const Event& event))
	{
		DelegateHandler func = std::bind(fn, caller, std::placeholders::_1);

		FuntionBinding binding;
		binding.m_handler = func;
		binding.m_name = name;

		m_handlers.push_back(binding);
	}

	void remove(const std::wstring& name)
	{
		for (auto it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			if ((*it).m_name == name)
			{
				m_handlers.erase(it);
				break;
			}
		}
	}

	template<typename ObjType>
	void remove(const std::wstring& name)
	{
		for (auto it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			if ((*it).m_name == name)
			{
				m_handlers.erase(it);
				break;
			}
		}
	}

	void invoke(const Event& event)
	{
		for (FuntionBinding& binding : m_handlers)
		{
			binding.m_handler(event);
		}
	}

protected:
	std::vector<FuntionBinding> m_handlers;
};

class IEventDispatcher
{
public:
	template <typename T>
	void registerListener(const std::wstring& event_name, const std::wstring& func_name, T* listener, void(T::*fn)(const Event& event))
	{
		auto it = m_delegates.find(event_name);
		if (it != m_delegates.end())
		{
			(it->second).append(func_name, listener, fn);
		}
		else
		{
			EventDelegate& delegate = m_delegates[event_name];
			delegate.append(func_name, listener, fn);
		}
	}

	void registerListener(const std::wstring& event_name, const std::wstring& func_name, void(*fn)(const Event& event))
	{
		auto it = m_delegates.find(event_name);
		if (it != m_delegates.end())
		{
			(it->second).append(func_name, fn);
		}
		else
		{
			EventDelegate& delegate = m_delegates[event_name];
			delegate.append(func_name, fn);
		}
	}

	template <typename T>
	void unregisterListener(const std::wstring& event_name, const std::wstring& func_name)
	{
		auto it = m_delegates.find(event_name);
		if (it != m_delegates.end())
		{
			(it->second).remove(event_name);
		}
	}

	void unregisterListener(const std::wstring& event_name, const std::wstring& func_name)
	{
		auto it = m_delegates.find(event_name);
		if (it != m_delegates.end())
		{
			(it->second).remove(event_name);
		}
	}

	void unregisterListener(const std::wstring& event_name)
	{
		auto it = m_delegates.find(event_name);
		if (it != m_delegates.end())
		{
			m_delegates.erase(it);
		}
	}

	void fireEvent(const std::wstring& eventName, const Event& event)
	{
		auto it = m_delegates.find(eventName);
		if (it != m_delegates.end())
		{
			(it->second).invoke(event);
		}
	}

protected:
	std::map<std::wstring, EventDelegate> m_delegates;
};

#endif