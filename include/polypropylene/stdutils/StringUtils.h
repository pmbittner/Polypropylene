//
// Created by Paul on 06.07.2017.
//

#ifndef POLYPROPYLENE_STRINGUTILS_H
#define POLYPROPYLENE_STRINGUTILS_H

#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <locale>

#include "polypropylene/serialisation/TypeConverter.h"
#include "polypropylene/log/Errors.h"

#define PAX_DECLARE_STRING_CONVERTER_FOR(type) PAX_DECLARE_TYPE_CONVERTER_FOR(::std::string, type)
#define PAX_IMPLEMENT_STRING_CONVERT_TO(type) PAX_IMPLEMENT_CONVERT_TO(::std::string, type)
#define PAX_IMPLEMENT_STRING_CONVERT_FROM(type) PAX_IMPLEMENT_CONVERT_FROM(::std::string, type)
#define PAX_IMPLEMENT_STRING_CONVERT_FROM_WITH_OSTREAM(type) \
    PAX_IMPLEMENT_CONVERT_FROM(::std::string, type) { \
        std::stringstream s; \
        s << x; \
        return s.str(); \
    }

#define PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(type, ...) \
    PAX_IMPLEMENT_STRING_CONVERT_TO(type) { return __VA_ARGS__; }

#define PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(type, ...) \
    PAX_IMPLEMENT_STRING_CONVERT_FROM(type) { return __VA_ARGS__; }

namespace PAX {
    template<class T>
    class TypeConverter<std::string, T> {
    public:
        PAX_NODISCARD static T convertTo(std::string const & s) {
            return T(s);
        }

        PAX_NODISCARD static std::string convertFrom(T const & t) {
            std::stringstream s;
            s << t;
            return s.str();
        }
    };

    namespace String {
        void LowerCased(std::string & str);
        void UpperCased(std::string & str);
        std::string LowerCase(const std::string & str);
        std::string UpperCase(const std::string & str);

        /// trim from start (in place)
        void ltrim(std::string &s);

        /// trim from end (in place)
        void rtrim(std::string &s);

        /// trim from both ends (in place)
        void trim(std::string &s);

        /// trim from both ends (copying)
        std::string trimmed(std::string s);

        bool startsWith(const std::string &s, const std::string &prefix);
        bool endsWith(const std::string &s, const std::string &suffix);

        /// Split the given *subject* string at occurences of *separator*.
        std::vector<std::string> split(char separator, const std::string &subject, bool keepEmptySubstrings = true);

        // TODO: Move to Path.h
        std::string getExtension(const std::string &path);

        void replace(std::string &string, const char &from, const char &to);

        template<typename T>
        T convertTo(std::string const &str) {
            if (str.empty()) {
                PAX_THROW_RUNTIME_ERROR("String is empty!");
            }

            return TypeConverter<std::string, T>::convertTo(str);
        }

        template<typename F>
        std::string convertFrom(F const & f) {
            return TypeConverter<std::string, F>::convertFrom(f);
        }

        template<typename T>
        std::string ToStringViaOstream(T const & t) {
            std::stringstream s;
            s << t;
            return s.str();
        }
    }

    /// Note: This macro cannot be used if 'type' is in a namespace (eg., MyApp::MyType) because it must not be
    ///       recognised as a preprocessor token.
#define PAX_SPECIALIZE_STRINGCONVERTER_HEADER(type) \
            template<> \
            class TypeConverter<std::string, type> { \
            public: \
                PAX_NODISCARD static type convertTo(const std::string & str); \
                PAX_NODISCARD static std::string convertFrom(const type & str); \
            };

    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(bool)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(char)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(short)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(unsigned short)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(int)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(unsigned int)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(long)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(unsigned long)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(long long)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(unsigned long long)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(float)
    PAX_SPECIALIZE_STRINGCONVERTER_HEADER(double)

    template<>
    class TypeConverter<std::string, Log::Level> {
    public:
        PAX_NODISCARD static Log::Level convertTo(const std::string & str);
        PAX_NODISCARD static std::string convertFrom(const Log::Level & str);
    };
}
#endif //POLYPROPYLENE_STRINGUTILS_H
