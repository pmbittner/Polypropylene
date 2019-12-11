//
// Created by paul on 07.01.19.
//

#ifndef POLYPROPYLENE_GENERICFACTORY_H
#define POLYPROPYLENE_GENERICFACTORY_H

#include <memory>

namespace PAX {
    class IGenericFactory {
    public:
        virtual ~IGenericFactory() = default;
    };

    template<typename Resource>
    class GenericFactoryT : public IGenericFactory {};

    template<typename Resource, typename... Params>
    class GenericFactory : public GenericFactoryT<Resource> {
    public:
        virtual std::shared_ptr<Resource> create(Params...) = 0;
    };
}

#endif //POLYPROPYLENE_GENERICFACTORY_H
