//
// Created by Paul on 24.03.2019.
//

#ifndef POLYPROPYLENE_PROPERTYCONTAINERSYSTEM_H
#define POLYPROPYLENE_PROPERTYCONTAINERSYSTEM_H


namespace PAX {
    template<typename T>
    class SystemManager;

    /**
     * Generic interface for any system that can be attached to SystemManagers of the given type.
     * Systems are used to administrate and execute behaviour on properties.
     * @tparam ManagerType The manager type to which this system can be added to.
     */
    template<typename ManagerType>
    class System {
        friend class SystemManager<ManagerType>;
        ManagerType * manager = nullptr;

        void setManager(ManagerType * manager) {
            this->manager = manager;
        }

    public:
        System() = default;
        virtual ~System() = 0;

        virtual void initialize(ManagerType*) {}
        virtual void terminate(ManagerType*) {}

        ManagerType * getManager() {
            return manager;
        }
    };
}

#endif //POLYPROPYLENE_PROPERTYCONTAINERSYSTEM_H
