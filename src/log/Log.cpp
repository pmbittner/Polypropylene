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

    Log & Log::Instance() {
        static Log log;
        return log;
    }

    std::ostream * Log::getStreamFor(PAX::Log::Level level) {
        if (level <= currentLevel) {
            return outstreams.at(level);
        } else {
            return &nullStream;
        }
    }

    Log::Log() : nullStream(&nullBuffer) {
        setStreamForLevel(Level::None, nullStream);

        setStreamForLevel(Level::Error, std::cerr);
        setStreamForLevel(Level::Warn, std::cerr);

        setStreamForLevel(Level::Info, std::cout);
        setStreamForLevel(Level::Debug, std::cout);
        setStreamForLevel(Level::Verbose, std::cout);
    }

    void Log::setStreamForLevel(Level level, std::ostream & stream) {
        outstreams[level] = &stream;
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
}

std::ostream& ::PAX::operator<<(std::ostream& os, const ::PAX::Log::Level & level) {
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