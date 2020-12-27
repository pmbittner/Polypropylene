//
// Created by Paul Bittner on 27.12.2020.
//

#include "Prefab.h"

#ifndef POLYPROPYLENE_COMPOSITEPREFAB_H
#define POLYPROPYLENE_COMPOSITEPREFAB_H

namespace PAX {
    /**
     * A CompositePrefab implements the composition of multiple
     * prefabs of the same time.
     * It combines a set of prefabs such that the produced T
     * gets the contents of all these prefabs.
     */
    template<class T>
    class CompositePrefab : public Prefab<T> {
        const std::vector<Prefab<T>*> composedPrefabs;

    public:
        /**
         * Creates a prefab that is the composition of the given prefabs.
         * Takes ownership of the given prefabs.
         */
        explicit CompositePrefab(const std::vector<Prefab<T>*> & prefabs)
            : composedPrefabs(prefabs) {}

        ~CompositePrefab() override {
            for (Prefab<T> * p : composedPrefabs) {
                delete p;
            }
        }

        void addMyContentTo(T & t, const VariableRegister & variableRegister) override {
            for (Prefab<T> * p : composedPrefabs) {
                p->addMyContentTo(t, variableRegister);
            }
        }
    };
}

#endif //POLYPROPYLENE_COMPOSITEPREFAB_H
