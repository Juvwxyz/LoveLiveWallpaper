#pragma once

#include "Object.h"

#include <list>
#include <map>
#include <functional>

namespace LLWP
{
    template<class _Fty>
    class ObjectBinder
    {
    public:
        ObjectBinder(void* obj, std::function<_Fty> func):
            obj_ptr(obj),
            functor(func)
        {}

    private:
        void* obj_ptr;
        std::function<_Fty> functor;

        template <class TR, class... TA>
        friend class Delegate;
    };


    template<class OBJECT, template<class, class...> class Binder = ObjectBinder, class TR>
    ObjectBinder<TR()> operator,(OBJECT& o, TR(OBJECT::* func)())
    {
        return ObjectBinder<TR()>((void*)&o, std::bind(func, &o));
    }

    template<class OBJECT, template<class, class...> class Binder = ObjectBinder, class TR, class T1>
    ObjectBinder<TR(T1)> operator,(OBJECT& o, TR(OBJECT::* func)(T1))
    {
        return ObjectBinder<TR(T1)>((void*)&o, std::bind(func, &o, std::placeholders::_1));
    }

    template<class OBJECT, template<class, class...> class Binder = ObjectBinder, class TR, class T1, class T2>
    ObjectBinder<TR(T1, T2)> operator,(OBJECT& o, TR(OBJECT::* func)(T1, T2))
    {
        return ObjectBinder<TR(T1, T2)>((void*)&o, std::bind(func, &o, std::placeholders::_1, std::placeholders::_2));
    }


    template<class>
    constexpr bool is_delegate = false;


    template <class TR, class... TA>
    class Delegate
    {
    private:
        template<class>
        friend class Event;

        using BinderType = ObjectBinder<TR(TA...)>;

        std::list<ObjectBinder<TR(TA...)>> handler_list;
    public:
        void invoke(TA... args)
        {
            for (auto it = handler_list.begin(); it != handler_list.end(); it++)
            {
                (*it).functor(std::forward<TA>(args)...);
            }
        }

        void operator+=(ObjectBinder<TR(TA...)> func)
        {
            handler_list.push_back(func);
        }

        void operator+=(std::function<TR(TA...)> func)
        {
            handler_list.push_back(ObjectBinder<TR(TA...)>(nullptr, func));
        }

        void operator-=(ObjectBinder<TR(TA...)> func)
        {
            for (auto it = handler_list.begin(); it != handler_list.end();)
            {
                if (it->obj_ptr == func.obj_ptr)
                {
                    it = handler_list.erase(it);
                    continue;
                }
                it++;
            }
        }
    };

    template<template<class, class...> class Delegate, class TR, class... TA>
    constexpr bool is_delegate<Delegate<TR, TA...>> = true;

    template<class T>
    class Event;

    template<template<class, class...> class Delegate, class TR, class... TA>
    class Event<Delegate<TR, TA...>>
    {
        static_assert(is_delegate<Delegate<TR, TA...>>, "Bad Delegate Type");
    public:
        Event(Delegate<TR, TA...>& handler) : event_handler(handler) {}

        void operator+=(ObjectBinder<TR(TA...)> func)
        {
            event_handler += func;
        }

        void operator-=(ObjectBinder<TR(TA...)> func)
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
        Delegate<TR, TA...>& event_handler;
    };

    //template <class TR, class... TA>
    //class Event
    //{
    //public:
    //    Event(Delegate<TR, TA...>& handler) : event_handler(handler) {}

    //    void operator+=(ObjectBinder<TR(TA...)> func)
    //    {
    //        event_handler += func;
    //    }

    //    void operator-=(ObjectBinder<TR(TA...)> func)
    //    {
    //        event_handler -= func;
    //    }

    //    ~Event() {}

    //public:
    //    Event(const Event&) = delete;
    //    Event(Event&&) = delete;
    //    Event& operator=(const Event&) = delete;
    //    Event& operator=(Event&&) = delete;

    //private:
    //    Delegate<TR, TA...>& event_handler;
    //};


}