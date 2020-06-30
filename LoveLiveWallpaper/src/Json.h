#pragma once
#include "JsonException.h"

#include <istream>
#include <string>
#include <exception>
#include <vector>
#include <map>

namespace LLWP
{
    class Json
    {
    public:
        enum class Type : char
        {
            object = 'o',
            array = 'a',
            string = 's',
            floating = 'f',
            integer = 'i',
            boolean = 'b',
            null = 'n',
        };

        Json();
        Json(Type t);
        Json(Json&& js) noexcept;

        static Json Deserialize(std::istream& s);
        std::string Serialize();

        void format(size_t depth);

        Json& operator[](const std::string& key);
        Json& operator[](const char* key);
        Json& operator[](int index);

        template<class T>
        T as() const
        {
            switch (type)
            {
            case LLWP::Json::Type::floating:
                if (std::is_convertible_v<T, long double>)
                {
                    return (T)data.f;
                }
                break;
            case LLWP::Json::Type::integer:
                if (std::is_convertible_v<T, long long>)
                {
                    return (T)data.i;
                }
                break;
            case LLWP::Json::Type::boolean:
                if (std::is_convertible_v<T, bool>)
                {
                    return (T)data.b;
                }
                break;
            default:
                break;
            }
            throw json_exception("Bad Conversion.");
        }

        template<>
        std::string as() const
        {
            if (type == Type::string)
            {
                return std::string(data.s);
            }
            throw json_exception("Bad Conversion.");
        }

        template<class T>
        operator T() const
        {
            return as<T>();
        }

        template<class T>
        Json& operator=(T right)
        {
            throw json_exception("Bad Conversion.");
        }

        template<class T, class SFINAE = typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, int>>
        Json& operator=(T right)
        {
            if (std::is_same_v<T, bool>)
            {
                clear();
                type = Type::boolean;
                data.b = right;
            }
            else if (std::is_integral_v<T>)
            {
                clear();
                type = Type::integer;
                data.i = right;
            }
            else if (std::is_floating_point_v<T>)
            {
                clear();
                type = Type::floating;
                data.f = right;
            }
            else
            {
                throw json_exception("Bad Conversion.");
            }
            return *this;
        }

        Json& operator=(Json&& right) noexcept
        {
            clear();
            type = right.type;
            data = right.data;
            right.data = { 0 };
            right.clear();
            return *this;
        }

        Json& operator=(const ::std::string& str)
        {
            clear();
            type = Type::string;
            data.s = new char[str.size() + 1];
            strcpy_s(data.s, str.size() + 1, str.c_str());
            return *this;
        }

        ~Json();
    private:
        struct Array : public std::vector<Json*>
        {
            std::string Serialize();
            ~Array();
        };

        struct Object : public std::map<std::string, Json*>
        {
            std::string Serialize();
            ~Object();
        };

        union Value
        {
            long long i;
            long double f;
            bool b;
            char* s;
            Array* a;
            Object* o;
        };


        Type type;
        Value data;

        void clear();

        static Json* read_element(std::istream& s);
        static Json* read_object(std::istream& s);
        static Json* read_array(std::istream& s);
        static Json* read_number(std::istream& s);
        static Json* read_null(std::istream& s);
        static Json* read_bool(std::istream& s);
        static Json* read_string(std::istream& s);
        static long long read_integer(std::istream& s);
        static long double read_fraction(std::istream& s);
        static int read_exponent(std::istream& s);

        friend std::ostream& operator<<(std::ostream& o, const Json& json);
        friend std::ostream& operator<<(std::ostream& o, Array* ary);
        friend std::ostream& operator<<(std::ostream& o, Object* obj);
    };

} // namespace LLWP
