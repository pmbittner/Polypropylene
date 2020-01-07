//
// Created by Paul on 28.04.2017.
//

#ifndef POLYPROPYLENE_OSDEFINES_H
#define POLYPROPYLENE_OSDEFINES_H

#if defined (__linux__) || defined(linux) || defined(LINUX)
#define PAX_OS_LINUX 1

#elif defined (__unix__) || defined(UNIX)
#define PAX_OS_UNIX 1

#elif defined(WINDOWS) || defined(WIN32) ||  defined(_WIN32) || defined(__WIN32__)
#define PAX_OS_WIN 1

#elif defined(ANDROID)
#define PAX_OS_ANDROID 1
#endif

#if defined(PAX_OS_WIN) ^ defined(PAX_OS_LINUX) ^ defined(PAX_OS_ANDROID) ^ defined(PAX_OS_UNIX)
#else
#error "No or multiple operating systems declared!"
#endif

#endif //POLYPROPYLENE_OSDEFINES_H
