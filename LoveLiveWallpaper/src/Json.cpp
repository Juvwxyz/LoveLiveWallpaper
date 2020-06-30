#include "Json.h"

#include <cstring>
#include <cmath>
#include <sstream>
#include <iostream>

void skip_white_space(std::istream& s)
{
    while (s.peek() == ' ' ||
        s.peek() == '\t' ||
        s.peek() == '\n' ||
        s.peek() == '\r')
    {
        s.get();
    }
}

namespace LLWP
{
    Json::Array::~Array()
    {
        for (size_t i = 0; i < size(); i++)
        {
            delete operator[](i);
        }
    }

    Json::Object::~Object()
    {
        for (auto it = begin(); it != end(); it++)
        {
            delete it->second;
        }
    }

    Json::Json(Type t) : type(t), data{ 0 }
    {
        switch (t)
        {
        case Type::object:
            data.o = new Object();
            break;
        case Type::array:
            data.a = new Array();
            break;
        case Type::string:
            data.s = new char[1]{ 0 };
            break;
        default:
            break;
        }
    }

    Json::Json() : type(Type::null), data{ 0 }
    {
    }

    Json::Json(Json&& js) noexcept : type(js.type), data(js.data)
    {
        js.data = { 0 };
    }

    Json Json::Deserialize(std::istream& s)
    {
        Json* json = read_element(s);
        if (json)
        {
            Json result(std::move(*json));
            delete json;
            return result;
        }
        else
        {
            return Json();
        }
    }

    Json::~Json()
    {
        clear();
    }

    void Json::clear()
    {
        switch (type)
        {
        case Type::string:
            if (data.s)
            {
                delete[] data.s;
            }
            break;
        case Type::array:
            if (data.a)
            {
                delete data.a;
            }
            break;
        case Type::object:
            if (data.o)
            {
                delete data.o;
            }
            break;
        default:
            break;
        }
        type = Type::null;
        data = { 0 };
    }

    Json* Json::read_element(std::istream& s)
    {
        Json* elem = nullptr;
        skip_white_space(s);
        switch (s.peek())
        {
        case '{':
            elem = read_object(s);
            break;
        case '[':
            elem = read_array(s);
            break;
        case '"':
            elem = read_string(s);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            elem = read_number(s);
            break;
        case 'n':
            elem = read_null(s);
            break;
        case 'f':
        case 't':
            elem = read_bool(s);
            break;
        default:
            break;
        }
        return elem;
    }

    Json* Json::read_object(std::istream& s)
    {
        Json* json = new Json(Type::object);

        s.get();
        skip_white_space(s);

        if (s.peek() == '}')
        {
            s.get();
            return json;
        }

        while (true)
        {
            skip_white_space(s);

            if (s.peek() != '"')
            {
                throw json_exception("Property keys must be doublequoted");
            }

            auto tmp = read_string(s);
            std::string str = std::string(tmp->data.s);
            delete tmp;

            skip_white_space(s);

            if (s.get() != ':')
            {
                throw json_exception("Colon expected");
            }

            skip_white_space(s);

            (*(json->data.o))[str] = read_element(s);

            skip_white_space(s);

            if (s.peek() == '}')
            {
                s.get();
                break;
            }
            else if (s.get() != ',')
            {
                throw json_exception("Comma expected");
            }
        }

        return json;
    }

    Json* Json::read_array(std::istream& s)
    {
        Json* json = new Json(Type::array);
        json->data.a = new Array();

        s.get();
        skip_white_space(s);

        if (s.peek() == ']')
        {
            s.get();
            return json;
        }

        while (true)
        {
            json->data.a->push_back(read_element(s));

            skip_white_space(s);

            if (s.peek() == ']')
            {
                s.get();
                break;
            }
            else if (s.get() != ',')
            {
                throw json_exception("Comma expected");
            }
        }

        return json;
    }

    Json* Json::read_number(std::istream& s)
    {
        Json* elem = nullptr;
        long long integer = 0;
        long double fraction = 0;
        bool isFloat = false;
        int exponent = 0;
        int sign = 1;
        if (s.peek() == '-')
        {
            sign = -1;
            s.get();
        }
        if (s.peek() == '0' && ((s.get(), s.peek()) != '.'))
        {
            elem = new Json(Type::integer);
            elem->data.i = 0;
            return elem;
        }
        else
        {
            integer = read_integer(s);
        }

        if (s.peek() == '.')
        {
            s.get();
            fraction = read_fraction(s);
            isFloat = true;
        }

        if (s.peek() == 'e' || s.peek() == 'E')
        {
            s.get();
            exponent = read_exponent(s);
            isFloat = true;
        }

        if (isFloat)
        {
            elem = new Json(Type::floating);
            elem->data.f = sign * (integer + fraction) * pow(10, exponent);
        }
        else
        {
            elem = new Json(Type::integer);
            elem->data.i = sign * integer;
        }
        return elem;
    }

    long long Json::read_integer(std::istream& s)
    {
        long long i = 0;
        while (s.peek() >= '0' && s.peek() <= '9')
        {
            i = (i * 10) + ((long long)s.get() - '0');
        }
        return i;
    }

    long double Json::read_fraction(std::istream& s)
    {
        long double fraction = 0;
        int exp = 10;

        while (s.peek() >= '0' && s.peek() <= '9')
        {
            fraction += ((long double)s.get() - '0') / exp;
            exp *= 10;
        }

        return fraction;
    }

    int Json::read_exponent(std::istream& s)
    {
        int sign = 1;
        if (s.peek() == '-')
        {
            sign = -1;
            s.get();
        }
        return sign * (int)read_integer(s);
    }

    Json* Json::read_null(std::istream& s)
    {
        Json* elem = new Json(Type::null);

        skip_white_space(s);

        if (s.get() == 'n' &&
            s.get() == 'u' &&
            s.get() == 'l' &&
            s.get() == 'l')
        {
            return elem;
        }

        throw json_exception("Value expected.");
    }

    Json* Json::read_bool(std::istream& s)
    {
        Json* elem = new Json(Type::boolean);

        skip_white_space(s);

        if (s.peek() == 't' &&
            s.get() == 't' &&
            s.get() == 'r' &&
            s.get() == 'u' &&
            s.get() == 'e')
        {
            elem->data.b = true;
            return elem;
        }
        else if (s.peek() == 'f' &&
            s.get() == 'f' &&
            s.get() == 'a' &&
            s.get() == 'l' &&
            s.get() == 's' &&
            s.get() == 'e')
        {
            elem->data.b = false;
            return elem;
        }

        throw json_exception("Value expected.");
    }

    Json* Json::read_string(std::istream& s)
    {
        std::string str;
        Json* json = new Json(Type::string);

        s.get();

        while (true)
        {
            if (s.peek() == '"')
            {
                s.get();
                break;
            }
            if ((unsigned char)s.peek() < ' ')
            {
                throw json_exception("Invalid characters in string. Control characters must be escaped.");
            }
            if (s.peek() == '\\')
            {
                s.get();
                switch (s.get())
                {
                case '\\':
                    str.push_back('\\');
                    break;
                case '\"':
                    str.push_back('\"');
                    break;
                case '/':
                    str.push_back('/');
                    break;
                case 'b':
                    str.push_back('\b');
                    break;
                case 'f':
                    str.push_back('\f');
                    break;
                case 'n':
                    str.push_back('\n');
                    break;
                case 'r':
                    str.push_back('\r');
                    break;
                case 't':
                    str.push_back('\t');
                    break;
                case 'u':
                {
                    uint32_t wch = 0;
                    unsigned char tmp;
                    for (size_t i = 0; i < 4; i++)
                    {
                        tmp = s.get();
                        if (tmp >= 'a' && tmp <= 'f')
                        {
                            tmp -= 'a' - 10;
                        }
                        else if (tmp >= 'A' && tmp <= 'F')
                        {
                            tmp -= 'A' - 10;
                        }
                        else if (tmp >= '0' && tmp <= '9')
                        {
                            tmp -= '0';
                        }
                        else
                        {
                            throw json_exception("Invalid unicode sequence in string.");
                        }
                        wch = (wch << 8) + tmp;
                    }
                    if (wch <= 0x0000007f)
                    {
                        tmp = (unsigned char)wch;
                        str.push_back(tmp);
                    }
                    else if (wch < 0x000007ff)
                    {
                        tmp = (unsigned char)(wch >> 6) | 0xc0;
                        str.push_back(tmp);
                        tmp = (unsigned char)(wch & 0x3f) | 0x80;
                        str.push_back(tmp);
                    }
                    else if (wch < 0x0000ffff)
                    {
                        tmp = (unsigned char)((wch >> 12)) | 0xe0;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 6) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)(wch & 0x3f) | 0x80;
                        str.push_back(tmp);
                    }
                    else if (wch < 0x001fffff)
                    {
                        tmp = (unsigned char)((wch >> 18)) | 0xf0;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 12) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 6) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)(wch & 0x3f) | 0x80;
                        str.push_back(tmp);
                    }
                    else if (wch < 0x03ffffff)
                    {
                        tmp = (unsigned char)(wch >> 24) | 0xf8;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 18) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 12) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 6) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)(wch & 0x3f) | 0x80;
                        str.push_back(tmp);
                    }
                    else if (wch < 0x7fffffff)
                    {
                        tmp = (unsigned char)(wch >> 30) | 0xfc;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 24) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 18) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 12) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)((wch >> 6) & 0x3f) | 0x80;
                        str.push_back(tmp);
                        tmp = (unsigned char)(wch & 0x3f) | 0x80;
                        str.push_back(tmp);
                    }
                    else
                    {
                        throw json_exception("Invalid unicode sequence in string.");
                    }
                }
                break;
                default:
                    throw json_exception("Invalid escape character in string.");
                }
            }
            else
            {
                str.push_back((char)s.get());
            }
        }

        json->data.s = new char[str.size() + 1];
        strcpy_s(json->data.s, str.size() + 1, str.c_str());
        return json;
    }

    std::string Json::Serialize()
    {
        std::string str;

        switch (type)
        {
        case Type::integer:
            str += std::to_string(data.i);
            break;
        case Type::floating:
        {
            char tmp[128];
            sprintf_s(tmp, "%Lg", data.f);
            str += tmp;
        }
        break;
        case Type::boolean:
            if (data.b)
            {
                str += "true";
            }
            else
            {
                str += "false";
            }
            break;
        case Type::string:
        {
            str += '"';
            int pos = 0;
            while (data.s[pos] != '\0')
            {
                switch (data.s[pos])
                {
                case '\"':
                    str += "\\\"";
                    break;
                case '\\':
                    str += "\\\\";
                    break;
                case '\b':
                    str += "\\b";
                    break;
                case '\f':
                    str += "\\f";
                    break;
                case '\n':
                    str += "\\n";
                    break;
                case '\r':
                    str += "\\r";
                    break;
                case '\t':
                    str += "\\t";
                    break;
                default:
                    str += data.s[pos];
                    break;
                }
                pos++;
            }
            str += '"';
        }
        break;
        case Type::array:
            str += data.a->Serialize();
            break;
        case Type::object:
            str += data.o->Serialize();
            break;
        case Type::null:
            str += "null";
            break;
        default:
            break;
        }

        return str;
    }

    void Json::format(size_t depth)
    {
        switch (type)
        {
        case Type::integer:
        {
            std::cout << data.i;
        }
        break;
        case Type::floating:
        {
            std::cout << data.f;
        }
        break;
        case Type::boolean:
        {
            if (data.b)
                std::cout << "true";
            else
                std::cout << "false";
        }
        break;
        case Type::null:
        {
            std::cout << "null";
        }
        break;
        case Type::string:
        {
            char* c = data.s;
            std::cout << '"';
            while (*c != '\0')
            {
                switch (*c)
                {
                case '\"':
                    std::cout << "\\\"";
                    break;
                case '\\':
                    std::cout << "\\\\";
                    break;
                case '\b':
                    std::cout << "\\b";
                    break;
                case '\f':
                    std::cout << "\\f";
                    break;
                case '\n':
                    std::cout << "\\n";
                    break;
                case '\r':
                    std::cout << "\\r";
                    break;
                case '\t':
                    std::cout << "\\t";
                    break;
                default:
                    std::cout << *c;
                    break;
                }
                c++;
            }
            std::cout << '"';
        }
        break;
        case Type::object:
        {
            std::cout << "{\n";
            int i = 0;
            for (auto it = data.o->begin(); it != data.o->end(); it++)
            {
                for (size_t i = 0; i < depth + 1; i++)
                {
                    std::cout << "    ";
                }
                std::cout << '"' << it->first << '"' << ": ";
                it->second->format(depth + 1);
                if (i < data.o->size() - 1)
                {
                    std::cout << ',';
                }
                i++;
                std::cout << "\n";
            }

            for (size_t i = 0; i < depth; i++)
            {
                std::cout << "    ";
            }
            std::cout.put('}');
        }
        break;
        case Type::array:
        {
            std::cout << '[' << '\n';
            for (size_t i = 0; i < data.a->size(); i++)
            {
                for (size_t j = 0; j < depth + 1; j++)
                {
                    std::cout << "    ";
                }
                (*(data.a))[i]->format(depth + 1);
                if (i < data.a->size() - 1)
                {
                    std::cout << ',';
                }
                std::cout << '\n';
            }

            for (size_t j = 0; j < depth; j++)
            {
                std::cout << "    ";
            }
            std::cout << ']';
        }
        break;
        default:
            break;
        }
    }

    Json& Json::operator[](const std::string& key)
    {
        if (type == Type::object)
        {
            if (data.o->count(key))
            {
                return *((*data.o)[key]);
            }
            throw json_exception("No such key.");
        }
        throw json_exception("Not an object type");
    }

    Json& Json::operator[](const char* key)
    {
        if (type == Type::object)
        {
            if (data.o->count(key))
            {
                return *((*data.o)[key]);
            }
            else
            {
                char* tmp = new char[strlen(key) + 1];
                strcpy(tmp, key);
                //member* m = new member{ tmp, new Json() };
                //data.o->insert(m);
                delete[] tmp;
                Json* j = ((*data.o)[key]);
                return *j;
            }
        }
        throw json_exception("Not an object type");
    }

    Json& Json::operator[](int index)
    {
        if (type == Type::array)
        {
            if (index < data.a->size())
            {
                return *(data.a->operator[](index));
            }
            throw json_exception("Index out of range.");
        }
        throw json_exception("Not an array type");
    }

    std::string Json::Object::Serialize()
    {
        std::string str;
        str += '{';

        for (auto it = begin(); it != end(); it++)
        {
            str += '"';
            str += it->first;
            str += '"';

            str += ':';

            str += it->second->Serialize();

            str += ',';
        }

        str.back() = '}';

        return str;
    }

    std::string Json::Array::Serialize()
    {
        std::string str;
        str += '[';

        for (size_t i = 0; i < size(); i++)
        {
            str += operator[](i)->Serialize();

            str += ',';
        }

        str.back() = ']';

        return str;
    }

    std::ostream& operator<<(std::ostream& o, Json::Array* ary)
    {
        o << '[';
        for (size_t i = 0; i < ary->size() - 1; i++)
        {
            o << *(*ary)[i] << ',';
        }
        if (ary->size() > 0)
        {
            o << *ary->back();
        }
        o << ']';
        return o;
    }

    std::ostream& operator<<(std::ostream& o, Json::Object* obj)
    {
        o << '{';
        size_t count = 0;
        for (auto it = obj->begin(); it != obj->end(); it++)
        {
            o << '"' << it->first << '"';
            o << ':';
            o << *(it->second);
            count++;
            if (count != obj->size())
            {
                o << ',';
            }
        }
        o << '}';
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const Json& json)
    {
        switch (json.type)
        {
        case Json::Type::integer:
            return o << json.data.i;
        case Json::Type::floating:
            return o << json.data.f;
        case Json::Type::boolean:
            if (json.data.b)
                return o << "true";
            else
                return o << "false";
        case Json::Type::string:
        {
            char* c = json.data.s;
            o.put('"');
            while (*c != '\0')
            {
                switch (*c)
                {
                case '\r':
                    o << "\\r";
                    break;
                case '\n':
                    o << "\\n";
                    break;
                case '\t':
                    o << "\\t";
                    break;
                case '\f':
                    o << "\\f";
                    break;
                case '\\':
                    o << "\\\\";
                    break;
                case '"':
                    o << "\\\"";
                    break;

                default:
                    o.put(*c);
                    break;
                }
                c++;
            }
            o.put('"');
            return o;
        }
        case Json::Type::array:
            return o << json.data.a;
        case Json::Type::object:
            return o << json.data.o;
        default:
            return o;
        }
    }

} // namespace LLWP