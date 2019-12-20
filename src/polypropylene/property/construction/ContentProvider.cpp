//
// Created by paul on 09.01.19.
//

#include <polypropylene/property/construction/ContentProvider.h>

namespace PAX
{
    ContentProvider::ContentProvider(PAX::Resources &resources, const PAX::VariableRegister &variables)
            : resources(resources), variables(variables) {}

    void ContentProvider::setContent(PropertyContent *content) {
        this->content = content;
    }

    void ContentProvider::writeToMetadata(ClassMetadata &metadata, Options options) const {
        for (Field & field : metadata.getFields()) {
            bool writeSuccessful = content->writeTo(field, variables);
            if (!writeSuccessful && (field.flag & Field::IsMandatory) && !(options & IgnoreMandatoryFlags)) {
                PAX_THROW_RUNTIME_ERROR("No value for field \"" << field.name << "\" defined!");
            }
        }
    }

    void ContentProvider::readFromMetadata(ClassMetadata &metadata) {
        PAX_NOT_IMPLEMENTED();
    }
}
