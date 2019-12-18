//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
#define POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H


#include "polypropylene/reflection/TypeMap.h"

#include "Allocator.h"
#include "allocators/MallocAllocator.h"
#include "allocators/PoolAllocator.h"

namespace PAX {
    class AllocationService {
        TypeMap<IAllocator*> allocators;

    public:
        AllocationService() = default;

        void registerAllocator(const TypeHandle& type, IAllocator * provider) {
            allocators[type] = provider;
        }

        size_t unregisterAllocator(const TypeHandle & type) {
            return allocators.erase(type);
        }

        template<class T>
        PAX_NODISCARD void * allocate() {
            Allocator<sizeof(T)> * allocator = nullptr;

            const auto & allocIt = allocators.find(paxtypeid(T));
            if (allocIt != allocators.end()) {
                allocator = dynamic_cast<Allocator<sizeof(T)>*>(allocIt->second);
                if (!allocator) {
                    PAX_LOG(PAX::Log::Level::Error, "Registered Allocator for " << typeid(T).name() << " is not of type Allocator<" << typeid(T).name() << ">!");
                }
            }

            if (!allocator){
                // TODO: Avoid new: Allocator for allocator lul
                allocator = new PoolAllocator<sizeof(T)>();
                registerAllocator(paxtypeid(T), allocator);
            }

            return allocator->allocate();
        }

        bool free(const TypeHandle& type, void * object) {
            const auto& allocator = allocators.find(type);
            if (allocator != allocators.end()) {
                allocator->second->destroy(object);
                return true;
            }

            return false;
        }
    };
}
#endif //POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
