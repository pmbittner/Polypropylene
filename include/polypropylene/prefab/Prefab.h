//
// Created by paul on 06.01.19.
//

#ifndef POLYPROPYLENE_PREFAB_H
#define POLYPROPYLENE_PREFAB_H

#include "polypropylene/reflection/VariableRegister.h"

namespace PAX {
    class IPrefab {
    public:
        /**
         * Allows specifying global variables for all prefabs.
         * For example, the working directory of the current program could be made accessible to all prefabs this way.
         */
        static VariableRegister PreDefinedVariables;
    };

    /**
     * Prefabs create new entities with a pre-defined set of properties.
     * Each entity created by an prefab can be considered equal.
     * Prefabs may inherit from each other when supported by the concrete prefab implementation.
     *
     * @tparam T The concrete type to instantiate (i.e., the derived class)
     */
    template<class T>
    class Prefab : public virtual IPrefab {
    public:
        explicit Prefab() = default;
        virtual ~Prefab() = default;

        /**
         * Creates a new entity from this prefab.
         * @param variableRegister Custom variables that can be considered upon creation.
         * @return returns a new entity of this prefab.
         */
        PAX_NODISCARD virtual T * create(const VariableRegister & variableRegister) {
            // TODO: Agree on global Allocator for T!
            T * e = new T();
            addMyContentTo(*e, variableRegister);
            return e;
        }

        /**
         * Adds all properties of this prefab to the given entity.
         * This can be used to combine multiple prefabs.
         * @param entity The entity to which the properties of this prefab should be added to.
         * @param variableRegister Custom variables that can be considered upon creation.
         */
        virtual void addMyContentTo(T & t, const VariableRegister & variableRegister) = 0;
    };
}

#endif //POLYPROPYLENE_PREFAB_H
