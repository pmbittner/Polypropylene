//
// Created by Paul on 02.06.2017.
//

#ifndef POLYPROPYLENE_ERRORS_H
#define POLYPROPYLENE_ERRORS_H

#include <sstream>
#include <stdexcept>
#include "Log.h"

#define PAX_ASSERT_NOT_NULL(value, ...) do { \
if (value == nullptr) { \
    std::stringstream _pax_stringstream_; \
    _pax_stringstream_ << __VA_ARGS__; \
    PAX_LOG(::PAX::Log::Level::Error, _pax_stringstream_.str()); \
    throw std::invalid_argument(_pax_stringstream_.str()); \
}} while(0)

#define PAX_THROW_RUNTIME_ERROR(...) do { \
    std::stringstream _pax_stringstream_; \
    _pax_stringstream_ << __VA_ARGS__; \
    PAX_LOG(::PAX::Log::Level::Error, _pax_stringstream_.str()); \
    throw std::runtime_error(_pax_stringstream_.str()); \
} while(0)

#define PAX_NOT_IMPLEMENTED() PAX_LOG(PAX::Log::Level::Warn, "not implemented")
#define PAX_NOT_IMPLEMENTED_EXCEPTION() PAX_THROW_RUNTIME_ERROR("not implemented")

#endif //POLYPROPYLENE_ERRORS_H
