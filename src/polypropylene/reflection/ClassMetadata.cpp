//
// Created by Bittner on 20.12.2019.
//

#include <polypropylene/reflection/ClassMetadata.h>
#include <polypropylene/log/Errors.h>

namespace PAX {
    ClassMetadata::ClassMetadata() = default;
    ClassMetadata::ClassMetadata(PAX::ClassMetadata &&other) noexcept = default;

    bool ClassMetadata::contains(const std::string &name) const {
        for (const Field & f : fields) {
            if (f.name == name) {
                return true;
            }
        }

        return false;
    }

    void ClassMetadata::add(const Field &field) {
        if (!this->contains(field.name)) {
            fields.push_back(field);
        } else {
            PAX_THROW_RUNTIME_ERROR("Duplicate registration of field " << field.name);
        }
    }

    Field ClassMetadata::get(const std::string &name) const {
        for (const Field & f : fields) {
            if (f.name == name) {
                return f;
            }
        }

        PAX_THROW_RUNTIME_ERROR("No field named \"" << name << "\" could be found.");
    }

    std::vector<Field> & ClassMetadata::getFields() {
        return fields;
    }
}