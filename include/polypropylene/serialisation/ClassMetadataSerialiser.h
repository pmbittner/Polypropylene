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

        /**
         * Sets the fieldstorage from and to which ClassMetadata should be read or written.
         * Does not take ownership of the given fieldstorage.
         * @param fieldStorage The fieldstorage to write and read classmetadata to/from.
         */
        void setStorage(FieldStorage * fieldStorage);

        /**
         * Writes all data in this object's field storage to the given class metadata.
         * Inverse operation is readFromMetadata.
         * @param metadata The metadata that should be written to.
         * @param options Options to configure writing process.
         *                For example, with IgnoreMandatoryFlags no mandatory fields in the given metadata will be written to.
         */
        void writeToMetadata(ClassMetadata & metadata, Options options = None) const;

        /**
         * Reads all fields from the given ClassMetadata and stores them in the internal FieldStorage.
         * @param metadata Metadata that should be serialized into the FieldStorage of this object.
         */
        void readFromMetadata(const ClassMetadata & metadata);
    };
}

#endif //POLYPROPYLENE_CLASSMETADATASERIALISER_H
