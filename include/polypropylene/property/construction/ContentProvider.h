//
// Created by Bittner on 10/01/2019.
//

#ifndef POLYPROPYLENE_CONTENTPROVIDER_H
#define POLYPROPYLENE_CONTENTPROVIDER_H

#include <string>
#include <polypropylene/resources/Resources.h>
#include <polypropylene/reflection/ClassMetadata.h>

// TODO: This has nothing to do with properties anymore. So move it or bla

namespace PAX {
    class PropertyContent {
    public:
        PropertyContent() = default;
        virtual ~PropertyContent() = default;

        PAX_NODISCARD virtual bool has(const std::string & name) const = 0;
        PAX_NODISCARD virtual VariableHierarchy getResourceParametersFor(const std::string & name) const = 0;
        PAX_NODISCARD virtual bool writeTo(Field & field, const VariableRegister &variables) const = 0;
        virtual void readFrom(Field & field) = 0;
    };

    class ContentProvider {
        PropertyContent * content = nullptr;
        Resources & resources;
        const VariableRegister & variables;

    public:
        enum Options : uint32_t {
            None = 0,
            IgnoreMandatoryFlags = 1
        };

        explicit ContentProvider(Resources & resources, const VariableRegister & variables);
        virtual ~ContentProvider() = default;

        void setContent(PropertyContent * content);
        void writeToMetadata(ClassMetadata & metadata, Options options = None) const;
        void readFromMetadata(ClassMetadata & metadata);
    };
}

#endif //POLYPROPYLENE_CONTENTPROVIDER_H
