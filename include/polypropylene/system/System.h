//
// Created by Paul on 24.03.2019.
//

#ifndef PAXENGINE3_PROPERTYCONTAINERSYSTEM_H
#define PAXENGINE3_PROPERTYCONTAINERSYSTEM_H


namespace PAX {
    template<typename T>
    class SystemManager;

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

#endif //PAXENGINE3_PROPERTYCONTAINERSYSTEM_H
