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

    bool TryParser<std::string, bool>::tryParse(const std::string & str) {
        std::string strcopy = str;
        String::LowerCased(strcopy);

        return
                strcopy == "true"
                || strcopy == "1"
                || strcopy == "on"
                || strcopy == "yes"
                || strcopy == "y";
    }

    char TryParser<std::string, char>::tryParse(const std::string &str) {
        return str[0];
    }

    short TryParser<std::string, short>::tryParse(const std::string &str) {
        return static_cast<short>(std::stoi(str));
    }

    unsigned short TryParser<std::string, unsigned short>::tryParse(const std::string &str) {
        return static_cast<unsigned short>(std::stoul(str));
    }

    int TryParser<std::string, int>::tryParse(const std::string &str) {
        return std::stoi(str);
    }

    unsigned int TryParser<std::string, unsigned int>::tryParse(const std::string &str) {
        return static_cast<unsigned int>(std::stoul(str));
    }

    long TryParser<std::string, long>::tryParse(const std::string &str) {
        return std::stol(str);
    }

    unsigned long TryParser<std::string, unsigned long>::tryParse(const std::string &str) {
        return std::stoul(str);
    }

    long long TryParser<std::string, long long>::tryParse(const std::string &str) {
        return std::stoll(str);
    }

    unsigned long long TryParser<std::string, unsigned long long>::tryParse(const std::string &str) {
        return std::stoull(str);
    }

    float TryParser<std::string, float>::tryParse(const std::string &str) {
        return std::stof(str);
    }

    double TryParser<std::string, double>::tryParse(const std::string &str) {
        return std::stod(str);
    }

    Log::Level TryParser<std::string, Log::Level>::tryParse(const std::string & str) {
        std::string lvl = String::LowerCase(str);

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
}