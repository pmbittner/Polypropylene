//
// Created by Paul on 04.10.2017.
//

#include <polypropylene/stdutils/StringUtils.h>
#include <sstream>
#include <iostream>

namespace PAX {
    namespace String {
        void LowerCased(std::string & str) {
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        }

        void UpperCased(std::string & str) {
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        }

        std::string LowerCase(const std::string & str) {
            std::string copy = str;
            LowerCased(copy);
            return copy;
        }

        std::string UpperCase(const std::string & str) {
            std::string copy = str;
            UpperCased(copy);
            return copy;
        }

        void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
        }

        void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }

        void trim(std::string &s) {
            ltrim(s);
            rtrim(s);
        }

        std::string trimmed(std::string s) {
            trim(s);
            return s;
        }

        bool startsWith(const std::string &s, const std::string &prefix) {
            return s.find(prefix) == 0;
        }

        bool endsWith(const std::string &s, const std::string &suffix) {
            return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
        }

        std::vector<std::string> split(const char separator, const std::string & subject, bool keepEmptySubstrings) {
            std::vector<std::string> result;
            std::istringstream ss(subject);
            while (!ss.eof())
            {
                std::string field;
                std::getline(ss, field, separator);
                if (!keepEmptySubstrings && field.empty()) continue;
                result.push_back(field);
            }
            return result;
        }

        std::string getExtension(const std::string &path) {
            auto dotIndex = path.find_last_of('.');

            if (dotIndex == std::string::npos)
                return "";

            return path.substr(dotIndex + 1);
        }

        void replace(std::string &string, const char &from, const char &to) {
            std::replace(string.begin(), string.end(), from, to);
        }
    }

    PAX_IMPLEMENT_STRING_CONVERT_TO(bool) {
        std::string strcopy = x;
        String::LowerCased(strcopy);

        return
                strcopy == "true"
                || strcopy == "1"
                || strcopy == "on"
                || strcopy == "yes"
                || strcopy == "y";
    }

    PAX_IMPLEMENT_STRING_CONVERT_FROM(bool) {
        if (x) {
            return "true";
        }
        return "false";
    }

    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(char, x[0])
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(short, static_cast<short>(std::stoi(x)))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(unsigned short, static_cast<unsigned short>(std::stoul(x)))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(int, std::stoi(x))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(unsigned int, static_cast<unsigned int>(std::stoul(x)))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(long, std::stol(x))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(unsigned long, std::stoul(x))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(long long, std::stoll(x))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(unsigned long long, std::stoull(x))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(float, std::stof(x))
    PAX_IMPLEMENT_STRING_CONVERT_TO_LAMBDA(double, std::stod(x))

    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(char, std::string(1, x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(short, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(unsigned short, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(int, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(unsigned int, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(long, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(unsigned long, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(long long, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(unsigned long long, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(float, std::to_string(x))
    PAX_IMPLEMENT_STRING_CONVERT_FROM_LAMBDA(double, std::to_string(x))

    PAX_IMPLEMENT_STRING_CONVERT_TO(Log::Level) {
        std::string lvl = String::LowerCase(x);

        if (lvl == "err" || lvl == "error" || lvl == "1") {
            return Log::Level::Error;
        } else if (lvl == "warn" || lvl == "warning" || lvl == "2") {
            return Log::Level::Warn;
        } else if (lvl == "dbg" || lvl == "debug" || lvl == "3") {
            return Log::Level::Debug;
        } else if (lvl == "verbose" || lvl == "4") {
            return Log::Level::Verbose;
        }

        return Log::Level::None;
    }

    PAX_IMPLEMENT_STRING_CONVERT_FROM_WITH_OSTREAM(Log::Level)
}