//
// Created by Bittner on 03.04.2019.
//

#include <polypropylene/definitions/OSDetection.h>
#include <polypropylene/log/Log.h>
#include <iostream>
#include <sstream>
#include <ctime>

namespace PAX {
    namespace Time {
        // Thanks to kjellkod: https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/
        static std::tm threadsafe_localtime(const std::time_t &time) {
            std::tm tm_snapshot{};
#ifdef PAX_OS_WIN
            localtime_s(&tm_snapshot, &time);
#else
            localtime_r(&time, &tm_snapshot); // POSIX
#endif
            return tm_snapshot;
        }
    }

    Log Log::instance = Log();

    std::ostream * Log::getStreamFor(PAX::Log::Level level) {
        if (level <= currentLevel) {
            return outstreams.at(level);
        } else {
            return &nullStream;
        }
    }

    Log::Log() : nullStream(&nullBuffer) {
        outstreams[Level::None] = &nullStream;

        outstreams[Level::Error] = &std::cerr;
        outstreams[Level::Warn] = &std::cerr;

        outstreams[Level::Info] = &std::cout;
        outstreams[Level::Debug] = &std::cout;
        outstreams[Level::Verbose] = &std::cout;
    }

    std::string Log::timestamp() {
        std::time_t t = std::time(nullptr);
        std::tm now = Time::threadsafe_localtime(t);
        std::stringstream s;
        s << 1900 + now.tm_year << "/" << 1 + now.tm_mon << "/" << now.tm_mday << " " << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec;
        return s.str();
    }

    std::ostream & Log::stream(Level level, const char * functionName, const char * fileName, int line) {
        std::ostream & stream = *getStreamFor(level);
        stream << timestamp() << " " << level << " [" << (functionName ? functionName : "unknown") << "] ";
        if (level == Level::Warn || level == Level::Error || level == Level::Debug) {
            stream << fileName << "(" << line << "): ";
        }
        return stream;
    }

    std::ostream & Log::stream_raw(Level level) {
        return *getStreamFor(level);
    }

    Log::Level TryParser<std::string, Log::Level>::tryParse(const std::string & str) {
        std::string lvl = str;
        String::toLower(lvl);

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

std::ostream& operator<<(std::ostream& os, const ::PAX::Log::Level & level) {
    using namespace ::PAX;
    switch (level) {
        case Log::Level::None: {
            return os;
        }
        case Log::Level::Error: {
            return os << "ERROR";
        }
        case Log::Level::Warn: {
            return os << "WARNING";
        }
        case Log::Level::Info: {
            return os << "INFO";
        }
        case Log::Level::Debug: {
            return os << "DEBUG";
        }
        case Log::Level::Verbose: {
            return os << "VERBOSE";
        }
        default: {
            return os << "<UNKNOWN LOG LEVEL>";
        }
    }
}