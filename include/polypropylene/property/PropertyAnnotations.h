//
// Created by paul on 30.12.18.
//

#ifndef POLYPROPYLENE_PROPERTYANNOTATIONS_H
#define POLYPROPYLENE_PROPERTYANNOTATIONS_H

#include "PropertyDependencies.h"
#include "event/PropertyAttachedEvent.h"
#include "event/PropertyDetachedEvent.h"

/// Generators

//#define PAX_OVERWRITE_NEW_AND_DELETE_FOR_PROPERTIES

#ifdef PAX_OVERWRITE_NEW_AND_DELETE_FOR_PROPERTIES
    #define _PAX_GENERATE_PROPERTY_NEW_AND_DELETE_HEADER_ \
    public: \
        static void* operator new(std::size_t sz); \
        static void operator delete(void * object); \
    private:

    #define _PAX_GENERATE_PROPERTY_NEW_AND_DELETE_SOURCE_ \
    void* Type::operator new(std::size_t sz) { \
        return EntityType::GetPropertyAllocator().allocate<Type>(); \
    } \
    void Type::operator delete(void * object) { \
        EntityType::GetPropertyAllocator().free(paxtypeid(Type), object); \
    }

#else
    #define _PAX_GENERATE_PROPERTY_NEW_AND_DELETE_HEADER_
    #define _PAX_GENERATE_PROPERTY_NEW_AND_DELETE_SOURCE_
#endif

// TODO: Find a way to make add and remove methods in PropertyContainer private.
#define _PAX_GENERATE_PROPERTY_ADD_OR_REMOVE_SOURCE_(Type, methodName, asMultiple, asSingle, EventType) \
bool Type::methodName(EntityType & e) { \
    if (Super::methodName(e)) { \
        PAX_CONSTEXPR_IF (This::IsMultiple()) { \
            if (!e.asMultiple(paxtypeid(This), this)) return false; \
        } else { \
            if (!e.asSingle(paxtypeid(This), this)) return false; \
        } \
        EventType<EntityType, This> event(this, &e); \
        e.getEventService()(event); \
        return true; \
    } \
    return false; \
}

///// Annotations
#define PAX_PROPERTY_IS_ABSTRACT(...)
#define PAX_PROPERTY_IS_CONCRETE(...) __VA_ARGS__

/// Mandatory
#define PAX_PROPERTY(Typename, IfConcrete) \
public: \
    friend class PropertyFactory<Typename, EntityType>; \
    const ::PAX::TypeHandle& getClassType() const override; \
    static constexpr bool IsAbstract() { return IfConcrete(false &&) true; } \
protected: \
    using This = Typename; \
    bool PAX_INTERNAL(addTo)(EntityType & e) override; \
    bool PAX_INTERNAL(removeFrom)(EntityType & e) override; \
private: \
IfConcrete(_PAX_GENERATE_PROPERTY_NEW_AND_DELETE_HEADER_)

#define PAX_PROPERTY_DERIVES(Parent) \
public: \
    using Super = Parent; \
private:

#define PAX_PROPERTY_SETMULTIPLE(bool_val) \
public:\
    static constexpr bool IsMultiple() { return Super::IsMultiple() && (bool_val); } \
    bool isMultiple() const override { return IsMultiple(); } \
private:

#define PAX_PROPERTY_IS_MULTIPLE PAX_PROPERTY_SETMULTIPLE(true)
#define PAX_PROPERTY_IS_SINGLE PAX_PROPERTY_SETMULTIPLE(false)

/// Optional

#define PAX_PROPERTY_DEPENDS_ON(...) \
protected: \
    virtual bool areDependenciesMetFor(const EntityType & entity) const override { \
        static ::PAX::PropertyDependencies<EntityType, __VA_ARGS__> dependencies; \
        return Super::areDependenciesMetFor(entity) && dependencies.met(entity); \
    } \
private:

///// SOURCE

#define PAX_PROPERTY_SOURCE(Type, IfConcrete) \
    _PAX_GENERATE_PROPERTY_ADD_OR_REMOVE_SOURCE_(Type, PAX_INTERNAL(addTo), PAX_INTERNAL(addAsMultiple), PAX_INTERNAL(addAsSingle), ::PAX::PropertyAttachedEvent) \
    _PAX_GENERATE_PROPERTY_ADD_OR_REMOVE_SOURCE_(Type, PAX_INTERNAL(removeFrom), PAX_INTERNAL(removeAsMultiple), PAX_INTERNAL(removeAsSingle), ::PAX::PropertyDetachedEvent) \
    const ::PAX::TypeHandle& Type::getClassType() const { \
        static PAX::TypeHandle myType = typeid(This); \
        return myType; \
    } IfConcrete(_PAX_GENERATE_PROPERTY_NEW_AND_DELETE_SOURCE_)

#endif //POLYPROPYLENE_PROPERTYANNOTATIONS_H
