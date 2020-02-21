//
// Created by paul on 30.12.18.
//

#ifndef POLYPROPYLENE_PROPERTYANNOTATIONS_H
#define POLYPROPYLENE_PROPERTYANNOTATIONS_H

#include "PropertyDependencies.h"
#include "PropertyFactory.h"
#include "event/PropertyAttachedEvent.h"
#include "event/PropertyDetachedEvent.h"

/// Generators

// TODO: Find a way to make add and remove methods in PropertyContainer private.
#define _PAX_GENERATE_PROPERTY_ADD_OR_REMOVE_SOURCE_(Type, methodName, asMultiple, asSingle, EventType) \
bool Type::methodName(EntityType & e) { \
    if (Super::methodName(e)) { \
        PAX_CONSTEXPR_IF (Type::IsMultiple()) { \
            if (!e.asMultiple(paxtypeid(Type), this)) return false; \
        } else { \
            if (!e.asSingle(paxtypeid(Type), this)) return false; \
        } \
        EventType<EntityType, Type> event(this, &e); \
        e.getEventService()(event); \
        return true; \
    } \
    return false; \
}

///// Annotations
#define PAX_PROPERTY_IS_CONCRETE false
#define PAX_PROPERTY_IS_ABSTRACT true

/// Mandatory
#define PAX_PROPERTY(Typename, isAbstract) \
public: \
    friend ::PAX::PropertyFactory<Typename, EntityType>; \
    static constexpr bool IsAbstract() { return isAbstract; } \
    PAX_NODISCARD const ::PAX::TypeHandle& getClassType() const override; \
protected: \
    bool PAX_INTERNAL(addTo)(EntityType & e) override; \
    bool PAX_INTERNAL(removeFrom)(EntityType & e) override;

#define PAX_PROPERTY_DERIVES(Parent) \
public: \
    using Super = Parent; \
private:

#define PAX_PROPERTY_SETMULTIPLE(bool_val) \
public:\
    static constexpr bool IsMultiple() { return Super::IsMultiple() && (bool_val); } \
    PAX_NODISCARD bool isMultiple() const override; \
private:

#define PAX_PROPERTY_IS_MULTIPLE PAX_PROPERTY_SETMULTIPLE(true)
#define PAX_PROPERTY_IS_SINGLE PAX_PROPERTY_SETMULTIPLE(false)

#define PAX_ROOT_PROPERTY(Typename, EntityName, isAbstract) \
PAX_PROPERTY(Typename, isAbstract) \
PAX_PROPERTY_DERIVES(::PAX::Property<EntityName>) \
PAX_PROPERTY_IS_MULTIPLE


/// Optional

#define PAX_PROPERTY_DEPENDS_ON(...) \
protected: \
    PAX_NODISCARD bool areDependenciesMetFor(const EntityType & entity) const override { \
        static ::PAX::PropertyDependencies<EntityType, __VA_ARGS__> dependencies; \
        return Super::areDependenciesMetFor(entity) && dependencies.met(entity); \
    } \
private:

///// SOURCE

#define PAX_PROPERTY_IMPL(Type) \
    _PAX_GENERATE_PROPERTY_ADD_OR_REMOVE_SOURCE_(Type, PAX_INTERNAL(addTo), PAX_INTERNAL(addAsMultiple), PAX_INTERNAL(addAsSingle), ::PAX::PropertyAttachedEvent) \
    _PAX_GENERATE_PROPERTY_ADD_OR_REMOVE_SOURCE_(Type, PAX_INTERNAL(removeFrom), PAX_INTERNAL(removeAsMultiple), PAX_INTERNAL(removeAsSingle), ::PAX::PropertyDetachedEvent) \
    const ::PAX::TypeHandle& Type::getClassType() const { \
        static PAX::TypeHandle myType = paxtypeof(Type); \
        return myType; \
    } \
    bool Type::isMultiple() const { return IsMultiple(); }

#endif //POLYPROPYLENE_PROPERTYANNOTATIONS_H
