//
// Created by Bittner on 20.12.2019.
//

#include <polypropylene/reflection/ClassMetadata.h>
#include <polypropylene/log/Errors.h>

namespace PAX {
    ClassMetadata::ClassMetadata() = default;
    ClassMetadata::ClassMetadata(const PAX::ClassMetadata &other) = default;
    ClassMetadata::ClassMetadata(PAX::ClassMetadata &&other) noexcept = default;

    bool ClassMetadata::contains(const std::string &name) const {
        for (const Field & f : fields) {
            if (f.name == name) {
                return true;
            }
        }

        return false;
    }

    Field& ClassMetadata::add(Field && field) {
        if (!this->contains(field.name)) {
            fields.emplace_back(field);
            return fields.back();
        } else {
            PAX_THROW_RUNTIME_ERROR("Duplicate registration of field " << field.name);
        }
    }

    Field & ClassMetadata::get(const std::string &name) {
        return const_cast<Field&>(const_cast<const ClassMetadata*>(this)->get(name));
    }

    const Field & ClassMetadata::get(const std::string &name) const {
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

    const std::vector<Field> & ClassMetadata::getFields() const {
        return fields;
    }

    void ClassMetadata::writeTo(ClassMetadata & other) const {
        for (const Field & myField : fields) {
            Field & otherField = other.get(myField.name);
            if (myField.isEqualTo(otherField)) {
                otherField.setFrom(myField);
            }
        }
    }
}