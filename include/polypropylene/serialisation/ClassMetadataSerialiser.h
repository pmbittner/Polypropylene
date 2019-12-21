//
// Created by Bittner on 10/01/2019.
//

#ifndef POLYPROPYLENE_CLASSMETADATASERIALISER_H
#define POLYPROPYLENE_CLASSMETADATASERIALISER_H

#include <string>
#include <polypropylene/resources/Resources.h>
#include <polypropylene/reflection/ClassMetadata.h>
#include "FieldStorage.h"

namespace PAX {
    /**
     * This class reads and writes ClassMetadata from / to FieldStorages.
     */
    class ClassMetadataSerialiser {
        FieldStorage * fieldStorage = nullptr;
        Resources & resources;
        const VariableRegister & variables;

    public:
        enum Options : uint32_t {
            None = 0,
            IgnoreMandatoryFlags = 1
        };

        explicit ClassMetadataSerialiser(Resources & resources, const VariableRegister & variables);
        virtual ~ClassMetadataSerialiser() = default;

        void setStorage(FieldStorage * fieldStorage);
        void writeToMetadata(ClassMetadata & metadata, Options options = None) const;
        void readFromMetadata(ClassMetadata & metadata);
    };
}

#endif //POLYPROPYLENE_CLASSMETADATASERIALISER_H
