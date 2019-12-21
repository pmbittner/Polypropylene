//
// Created by paul on 09.01.19.
//

#include <polypropylene/serialisation/ClassMetadataSerialiser.h>

namespace PAX
{
    ClassMetadataSerialiser::ClassMetadataSerialiser(PAX::Resources &resources, const PAX::VariableRegister &variables)
            : resources(resources), variables(variables) {}

    void ClassMetadataSerialiser::setStorage(FieldStorage * fieldStorage) {
        this->fieldStorage = fieldStorage;
    }

    void ClassMetadataSerialiser::writeToMetadata(ClassMetadata &metadata, Options options) const {
        for (Field & field : metadata.getFields()) {
            bool writeSuccessful = fieldStorage->writeTo(field, variables);
            if (!writeSuccessful && (field.flag & Field::IsMandatory) && !(options & IgnoreMandatoryFlags)) {
                PAX_THROW_RUNTIME_ERROR("No value for field \"" << field.name << "\" defined!");
            }
        }
    }

    void ClassMetadataSerialiser::readFromMetadata(ClassMetadata &metadata) {
        PAX_NOT_IMPLEMENTED();
    }
}
