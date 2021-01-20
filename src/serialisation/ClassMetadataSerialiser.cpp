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
            Field::WriteResult result = fieldStorage->writeTo(field, variables);
            if (result.value != Field::WriteResult::Success) {
                if ((field.flags & Field::IsMandatory) && !(options & IgnoreMandatoryFlags)) {
                    PAX_THROW_RUNTIME_ERROR("Could not set mandatory field \"" << field.type.name() << " " << field.name << "\" from \"" << fieldStorage->toString() << "\"!\nReason is " << Field::WriteResult::ToString(result.value) << ": " << result.message);
                } else {
                    PAX_LOG(Log::Level::Warn, "Could not set field \"" << field.type.name() << " " << field.name << "\" from \"" << fieldStorage->toString() << "\"!\nReason is " << Field::WriteResult::ToString(result.value) << ": " << result.message);
                }
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
