//
// Created by Bittner on 10/01/2019.
//

#ifndef POLYPROPYLENE_CLASSMETADATASERIALISER_H
#define POLYPROPYLENE_CLASSMETADATASERIALISER_H

#include <string>
#include <polypropylene/reflection/ClassMetadata.h>
#include "FieldStorage.h"

namespace PAX {
    /**
     * This class reads and writes ClassMetadata from / to FieldStorages.
     */
    class ClassMetadataSerialiser {
        FieldStorage * fieldStorage = nullptr;
        const VariableRegister & variables;

    public:
        enum Options : uint32_t {
            None = 0,
            IgnoreMandatoryFlags = 1
        };

        explicit ClassMetadataSerialiser(const VariableRegister & variables);
        virtual ~ClassMetadataSerialiser() = default;

        void setStorage(FieldStorage * fieldStorage);
        void writeToMetadata(ClassMetadata & metadata, Options options = None) const;
        void readFromMetadata(const ClassMetadata & metadata);
    };
}

#endif //POLYPROPYLENE_CLASSMETADATASERIALISER_H
