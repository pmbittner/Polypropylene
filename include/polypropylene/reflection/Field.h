//
// Created by Bittner on 20.12.2019.
//

#ifndef POLYPROPYLENE_FIELD_H
#define POLYPROPYLENE_FIELD_H

#include <string>
#include <polypropylene/reflection/TypeHandle.h>

namespace PAX {
    struct Field {
        enum Flag : uint32_t {
            None = 0,
            IsMandatory = 1,
            //IsResource = 2 // For future use in engine
        };

        TypeHandle type;
        std::string name;
        Flag flag;

        // having this instance independent would be very nice
        void * data;

        Field(const std::string & name, const TypeHandle & type, void * data, Flag flags = None);
    };
}

#endif //POLYPROPYLENE_FIELD_H
