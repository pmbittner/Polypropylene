//
// Created by Paul on 06.07.2017.
//

#ifndef PAXENGINE3_STRINGUTILS_H
#define PAXENGINE3_STRINGUTILS_H

#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <locale>

#include "TryParser.h"

namespace PAX {
    namespace String {
        void toLower(std::string & str);
        void toUpper(std::string & str);

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

        std::string getExtension(const std::string &path);

        void replace(std::string &string, const char &from, const char &to);

        template<typename T>
        T tryParse(const std::string &str) {
            return TryParser<std::string, T>::tryParse(str);
        }
    }

#define PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(type) \
            template<> \
            class TryParser<std::string, type> { \
            public: \
                PAX_NODISCARD static type tryParse(const std::string & str); \
            };

    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(bool)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(int)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(unsigned int)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(unsigned long)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(float)
    PAX_SPECIALIZE_STRINGTRYPARSE_HEADER(double)
}
#endif //PAXENGINE3_STRINGUTILS_H
