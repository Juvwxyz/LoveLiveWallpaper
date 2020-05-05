#pragma once

#include <list>
#include <functional>

namespace LLWP
{

    template <class OBJECT, class TR>
    std::pair<size_t, std::function<TR()>> operator,(OBJECT& o, TR(OBJECT::* func)())
    {
        return std::make_pair<size_t, std::function<TR()>>((size_t)&o, std::bind(func, &o));
    }

    template <class OBJECT, class TR, class T1>
    std::pair<size_t, std::function<TR(T1)>> operator,(OBJECT& o, TR(OBJECT::* func)(T1))
    {
        return std::make_pair<size_t, std::function<TR(T1)>>((size_t)&o, std::bind(func, &o, std::placeholders::_1));
    }

    template <class OBJECT, class TR, class T1, class T2>
    std::pair<size_t, std::function<TR(T1, T2)>> operator,(OBJECT& o, TR(OBJECT::* func)(T1, T2))
    {
        return std::make_pair<size_t, std::function<TR(T1, T2)>>((size_t)&o, std::bind(func, &o, std::placeholders::_1, std::placeholders::_2));
    }

    template <class OBJECT, class TR, class T1, class T2, class T3>
    std::pair<size_t, std::function<TR(T1, T2, T3)>> operator,(OBJECT& o, TR(OBJECT::* func)(T1, T2, T3))
    {
        return std::make_pair<size_t, std::function<TR(T1, T2, T3)>>((size_t)&o, std::bind(func, &o, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    template <class>
    class Delegate;

    template <class TR, class... TA>
    class Delegate<TR(TA...)>
    {
    private:
        template <class>
        friend class Event;

        std::list<std::pair<size_t, std::function<TR(TA...)>>> handler_list;

    public:
        TR operator()(TA... args)
        {
            return invoke(std::forward<TA>(args)...);
        }

        TR invoke(TA... args)
        {
            int s = handler_list.size();
            if (s-- == 0) return TR();
            auto it = handler_list.begin();
            for (size_t i = 0; i < s; it++, i++)
            {
                it->second(std::forward<TA>(args)...);
            }
            return it->second(std::forward<TA>(args)...);
        }

        void operator+=(Delegate<TR(TA...)> delegate)
        {
            for (auto it = delegate.handler_list.begin(); it != delegate.handler_list.end(); it++)
            {
                this->handler_list.push_back(*it);
            }
        }

        void operator+=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            handler_list.push_back(func);
        }

        void operator+=(TR(*func)(TA...))
        {
            auto a = std::make_pair<size_t, std::function<TR(TA...)>>(size_t(func), func);
            handler_list.push_back(a);
        }

        void operator-=(Delegate<TR(TA...)> delegate)
        {
            for (auto it = delegate.handler_list.begin(); it != delegate.handler_list.end(); it++)
            {
                for (auto i = this->handler_list.rbegin(); i != this->handler_list.rend(); i++)
                {
                    if (it->first == i->first)
                    {
                        this->handler_list.erase(i);
                        break;
                    }
                }
            }
        }

        void operator-=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            for (auto i = this->handler_list.rbegin(); i != this->handler_list.rend(); i++)
            {
                if (func.first == i->first)
                {
                    this->handler_list.erase((++i).base());
                    break;
                }
            }
        }

        void operator-=(TR(*func)(TA...))
        {
            for (auto i = this->handler_list.rbegin(); i != this->handler_list.rend(); i++)
            {
                if ((size_t)func == i->first)
                {
                    this->handler_list.erase((++i).base());
                    break;
                }
            }
        }
    };

    template <class T>
    class Event;

    template <template <class, class...> class Delegate, class TR, class... TA>
    class Event<Delegate<TR(TA...)>>
    {
    public:
        Event(Delegate<TR(TA...)>& handler) : event_handler(handler) {}

        void operator+=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            event_handler += func;
        }

        void operator+=(TR(*func)(TA...))
        {
            event_handler += func;
        }

        void operator-=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            event_handler -= func;
        }

        void operator-=(TR(*func)(TA...))
        {
            event_handler -= func;
        }

        ~Event() {}

    public:
        Event(const Event&) = delete;
        Event(Event&&) = delete;
        Event& operator=(const Event&) = delete;
        Event& operator=(Event&&) = delete;

    private:
        Delegate<TR(TA...)>& event_handler;
    };

}