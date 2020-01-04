//
// Created by Bittner on 04.01.2020.
//

#ifndef POLYPROPYLENE_FORWARDDECLARATIONS_H
#define POLYPROPYLENE_FORWARDDECLARATIONS_H

namespace PAX {
    template<class TEntityType>
    class Property;

    template<class TEntityType>
    class PrototypeEntityPrefab;

    template<class TDerived, class TPropertyType = Property<TDerived>>
    class Entity;
}

#endif //POLYPROPYLENE_FORWARDDECLARATIONS_H
