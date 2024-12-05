//
// Created by Bittner on 03.04.2019.
//

#ifndef POLYPROPYLENE_LOG_H
#define POLYPROPYLENE_LOG_H

#include "../definitions/OSDetection.h"
#include "../definitions/Definitions.h"
#include <ostream>
#include <map>

namespace PAX {
    class Log {
    public:
        enum class Level {
            None = 0,
            Error,
            Warn,
            Info,
            Debug,
            Verbose
        };

    private:
        class NullBuffer : public std::streambuf {
        public:
            int overflow(int c) override { return c; }
        } nullBuffer;
        std::ostream nullStream;

        std::map<Level, std::ostream*> outstreams;

        Log();

    public:
        static Log& Instance();
        static std::string timestamp();

        Level currentLevel =
#ifdef PAX_BUILD_TYPE_DEBUG
            Level::Debug;
#else
            Level::Error;
#endif

        /**
         * Sets the stream to write logs to for a given log level.
         * All logs made on the given level will be made to the given stream afterwards.
         * @param level The level whose stream should be changed.
         * @param stream The stream to write logs to.
         */
        void setStreamForLevel(Level level, std::ostream & stream);

        /**
         * @param level The level whose stream should be returned.
         * @return The ostream to which log messages for the given level are streamed to.
         */
        std::ostream * getStreamFor(Level level);

        PAX_NODISCARD std::ostream & stream(Level level, const char * functionName = nullptr, const char * fileName = nullptr, int line = 0);
        PAX_NODISCARD std::ostream & stream_raw(Level level);
    };

std::ostream& operator<<(std::ostream& os, const Log::Level & level);

// What about __func__ ?
#ifdef PAX_OS_WIN
#define PAX_FUNCTION_NAME __FUNCTION__
#else
#define PAX_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

/**
 * Main macro for logging.
 * For the given level (first argument) the given message (second argument) will be logged.
 * Use the ostream operator << to concatenate arguments in the message.
 * For example,
 *     int x = 3;
 *     PAX_LOG(Log::Level::Info, "Test Log Message " << x);
 * will write "Test Log Message 3" with a timestamp and caller information to the stream for level Info in Log::instance.
 */
#define PAX_LOG(level, ... /* message */) do {::PAX::Log::Instance().stream(level, PAX_FUNCTION_NAME, __FILE__, __LINE__) << __VA_ARGS__ << std::endl;} while(0)

/**
 * The same as PAX_LOG but does not write any metadata.
 */
#define PAX_LOG_RAW(level, ... /* message */) do {::PAX::Log::Instance().stream_raw(level) << __VA_ARGS__ << std::endl;} while(0)

/**
 * PAX_LOG_DEBUG will only log if the program is built in debug mode.
 */
#ifdef PAX_BUILD_TYPE_DEBUG
    #define PAX_LOG_DEBUG(level, ... /* message */) PAX_LOG(level, __VA_ARGS__)
#else
    #define PAX_LOG_DEBUG(level, ... /* message */) do {} while(0)
#endif
}

#endif //POLYPROPYLENE_LOG_H
