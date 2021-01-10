////
//// Created by Paul Bittner on 10.01.2021.
////

#ifndef POLYPROPYLENE_LOGTESTS_H
#define POLYPROPYLENE_LOGTESTS_H

#include "PaxTest.h"

#include "polypropylene/log/Log.h"

namespace PAX {
    PAX_TEST(LogTests, DefaultStreamsPresent)
        std::vector<Log::Level> levels = {
                Log::Level::None,
                Log::Level::Error,
                Log::Level::Warn,
                Log::Level::Info,
                Log::Level::Debug,
                Log::Level::Verbose,
        };

        for (Log::Level level : levels) {
            EXPECT_NE(Log::instance.getStreamFor(level), nullptr) << "There is no default stream for Log::Level " << level;
        }
    }
}

#endif //POLYPROPYLENE_LOGTESTS_H
