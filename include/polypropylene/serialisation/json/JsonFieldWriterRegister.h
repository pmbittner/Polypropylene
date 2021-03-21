//
// Created by Paul Bittner on 17.03.2020.
//

#ifndef PAXENGINE3_JSONFIELDWRITERREGISTER_H
#define PAXENGINE3_JSONFIELDWRITERREGISTER_H

#ifdef PAX_WITH_JSON

#include "JsonFieldWriter.h"
#include "polypropylene/reflection/TypeMap.h"

namespace PAX {
    namespace Json {
        /**
         * Register for managing IJsonFieldWriters by types.
         * Each registered writer is assumed to be able to parse the type of data it is registered for.
         */
        class JsonFieldWriterRegister {
            TypeMap<const IJsonFieldWriter *> writers;
            JsonFieldWriterRegister();

        public:
            static JsonFieldWriterRegister& Instance();

            PAX_NODISCARD bool hasWriterForType(const PAX::TypeId &type) const;
            bool registerWriter(const PAX::TypeId &type, const IJsonFieldWriter * parser);
            PAX_NODISCARD const IJsonFieldWriter * getWriterFor(const PAX::TypeId &type) const;
        };
    }
}

#endif // PAX_WITH_JSON

#endif //PAXENGINE3_JSONFIELDWRITERREGISTER_H
