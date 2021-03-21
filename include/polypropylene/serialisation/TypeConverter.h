//
// Created by Bittner on 08.08.2019.
//

#ifndef POLYPROPYLENE_TYPECONVERTER_H
#define POLYPROPYLENE_TYPECONVERTER_H

#include "polypropylene/definitions/Definitions.h"

#define PAX_DECLARE_TYPE_CONVERTER_FOR(FromType, ToType) \
namespace PAX { \
    template<> \
    class TypeConverter<FromType, ToType> { \
    public: \
        PAX_NODISCARD static ToType convertTo(FromType const &); \
        PAX_NODISCARD static FromType convertFrom(ToType const &); \
    }; \
}

#define PAX_IMPLEMENT_CONVERT_TO(FromType, ToType) \
    ToType TypeConverter<FromType, ToType>::convertTo(FromType const & x)

#define PAX_IMPLEMENT_CONVERT_FROM(FromType, ToType) \
    FromType TypeConverter<FromType, ToType>::convertFrom(ToType const & x)

namespace PAX {
    template<class From, class To>
    class TypeConverter {
    public:
        PAX_NODISCARD static To convertTo(From const & f) {
            return f;
        }

        PAX_NODISCARD static From convertFrom(To const & t) {
            return t;
        }
    };
}

#endif //POLYPROPYLENE_TYPECONVERTER_H
