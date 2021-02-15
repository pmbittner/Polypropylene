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

#include "polypropylene/serialisation/TryParser.h"
#include "polypropylene/log/Errors.h"

namespace PAX {
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
        T tryParse(const std::string &str) {
            if (str.empty()) {
                PAX_THROW_RUNTIME_ERROR("String is empty!");
            }

            return TryParser<std::string, T>::tryParse(str);
        }

        template<typename T>
        std::string ToStringViaOstream(const T & t) {
            std::stringstream s;
            s << t;
            return s.str();
        }
    }

    /// Note: This macro cannot be used if 'type' is in a namespace (eg., MyApp::MyType) because it must not be
    ///       recognised as a preprocessor token.
#define PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(type) \
            template<> \
            class TryParser<std::string, type> { \
            public: \
                PAX_NODISCARD static type tryParse(const std::string & str); \
            };

    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(bool)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(char)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(short)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(unsigned short)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(int)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(unsigned int)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(long)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(unsigned long)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(long long)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(unsigned long long)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(float)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(double)

    template<>
    class TryParser<std::string, Log::Level> {
    public:
        PAX_NODISCARD static Log::Level tryParse(const std::string & str);
    };
}
#endif //POLYPROPYLENE_STRINGUTILS_H
