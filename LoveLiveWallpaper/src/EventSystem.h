#pragma once

#include <list>
#include <functional>

namespace LLWP
{
    // 辅助逗号操作符
    // 将对象与其成员函数绑定
    // 以支持 Delegate += (obj, function) 语法
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

    // 委托类模板
    // 
    // C#风格的委托类，使用+=和-=操作符绑定和解绑委托函数，支持多播
    // 可以绑定普通函数、类的静态函数、到对象的成员函数
    // 
    // 模板参数：
    //     TR    - 返回值类型
    //     TA... - 参数包，可以为空
    // 
    template <class TR, class... TA>
    class Delegate<TR(TA...)>
    {
    private:
        template <class>
        friend class Event;

        // 存放事件处理函数的列表，以支持委托多播
        // 列表元素是 size_t 与 std::function 构成的 std::pair
        // size_t 用于区分不同的函数绑定(operator-=时用到)
        // 绑定普通函数或类静态函数时，size_t是函数的地址，std::function内包装的是该函数的指针
        // 绑定到对象的成员函数时，size_t是该对象的地址，std::function内包装的是对象与成员函数的绑定（使用std::bind）
        // 使用上方重载的逗号操作符绑定对象与成员函数
        std::list<std::pair<size_t, std::function<TR(TA...)>>> handler_list;

    public:
        TR operator()(TA... args)
        {
            return invoke(std::forward<TA>(args)...);
        }

        TR invoke(TA... args)
        {
            int s = (int)handler_list.size();
            if (s-- == 0) return TR();
            auto it = handler_list.begin();
            for (size_t i = 0; i < s; it++, i++)
            {
                it->second(std::forward<TA>(args)...);
            }
            return it->second(std::forward<TA>(args)...);
        }

        // 绑定另一个同类型委托中的处理函数
        void operator+=(Delegate<TR(TA...)> delegate)
        {
            for (auto it = delegate.handler_list.begin(); it != delegate.handler_list.end(); it++)
            {
                this->handler_list.push_back(*it);
            }
        }

        // 绑定到对象的成员函数
        void operator+=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            handler_list.push_back(func);
        }

        // 绑定函数指针
        void operator+=(TR(*func)(TA...))
        {
            auto a = std::make_pair<size_t, std::function<TR(TA...)>>(size_t(func), func);
            handler_list.push_back(a);
        }

        // 解绑另一个同类型委托中的处理函数
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

        // 解绑到对象的成员函数
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

        // 解绑函数指针
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
    // 事件类模板
    // 
    // C#风格的事件类，是对委托类的封装
    // 
    template <template <class, class...> class Delegate, class TR, class... TA>
    class Event<Delegate<TR(TA...)>>
    {
    public:

        // 构造事件时必须绑定委托类对象的实例
        // 通常的做法是先声明一个私有的委托对象
        // 再声明一个公开的事件对象，并将委托对象绑定到事件对象
        // 这样在类的外部就只能通过+=和-=来订阅和取消订阅事件
        Event(Delegate<TR(TA...)>& handler) : event_handler(handler) {}

        inline void operator+=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            event_handler += func;
        }

        inline void operator+=(TR(*func)(TA...))
        {
            event_handler += func;
        }

        inline void operator-=(std::pair<size_t, std::function<TR(TA...)>> func)
        {
            event_handler -= func;
        }

        inline void operator-=(TR(*func)(TA...))
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

    template<class... TA>
    using Action = Delegate<void(TA...)>;
}