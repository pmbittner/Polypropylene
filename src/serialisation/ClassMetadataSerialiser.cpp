//
// Created by paul on 09.01.19.
//

#include "polypropylene/log/Errors.h"
#include <polypropylene/serialisation/ClassMetadataSerialiser.h>

namespace PAX
{
    ClassMetadataSerialiser::ClassMetadataSerialiser(const PAX::VariableRegister &variables)
            : variables(variables) {}

    void ClassMetadataSerialiser::setStorage(FieldStorage * fieldStorage) {
        this->fieldStorage = fieldStorage;
    }

    void ClassMetadataSerialiser::writeToMetadata(ClassMetadata &metadata, Options options) const {
        for (Field & field : metadata.getFields()) {
            bool writeSuccessful = fieldStorage->writeTo(field, variables);
            if (!writeSuccessful && (field.flags & Field::IsMandatory) && !(options & IgnoreMandatoryFlags)) {
                PAX_THROW_RUNTIME_ERROR("No value for field \"" << field.name << "\" defined!");
            }
        }
    }

    void ClassMetadataSerialiser::readFromMetadata(const ClassMetadata &metadata) {
        for (const Field & field : metadata.getFields()) {
            bool readSuccessful = fieldStorage->readFrom(field);
            if (!readSuccessful) {
                PAX_LOG(Log::Level::Warn, "Field \"" << field.name << "\" of type \"" << field.type.name() << "\" could not be read!");
            }
        }
    }
}
