//
// Created by Paul on 24.03.2019.
//

#ifndef POLYPROPYLENE_PROPERTYCONTAINERSYSTEMMANAGER_H
#define POLYPROPYLENE_PROPERTYCONTAINERSYSTEMMANAGER_H

#include <cassert>

#include <set>
#include <memory>
#include <polypropylene/event/EventService.h>

#include "System.h"

namespace PAX {
    template<typename Derived>
    class SystemManager {
        std::set<std::unique_ptr<System<Derived>>> systems;
        bool initialized = false;

    protected:
        const std::set<std::unique_ptr<System<Derived>>> & getSystems() {
            return systems;
        }

    public:
        SystemManager() = default;
        virtual ~SystemManager() = 0;

        virtual void initialize() {
            assert(!initialized);

            for (auto & system : systems) {
                system->initialize(static_cast<Derived*>(this));
            }

            initialized = true;
        }

        virtual void terminate() {
            assert(initialized);

            for (auto & system : systems) {
                system->terminate(static_cast<Derived*>(this));
            }
        }

        /**
         * Takes ownership if the given system.
         * @param system
         */
        void addSystem(std::unique_ptr<System<Derived>> system) {
            if (initialized)
                system->initialize(static_cast<Derived*>(this));

            system->setManager(static_cast<Derived*>(this));

            systems.insert(std::move(system));
        }

        template<class T>
        T * getSystem() const {
            for (const std::unique_ptr<System<Derived>> & s : systems) {
                if (auto t = dynamic_cast<T*>(s.get())) {
                    return t;
                }
            }

            return nullptr;
        }
    };
}

#endif //POLYPROPYLENE_PROPERTYCONTAINERSYSTEMMANAGER_H
