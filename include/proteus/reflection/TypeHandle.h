//
// Created by Paul on 14.01.2018.
//

#ifndef POLYPROPYLENE_TYPEINFO_H
#define POLYPROPYLENE_TYPEINFO_H

#include <typeindex>

namespace PAX {
    using TypeHandle = std::type_index;
}

#define paxtypeid(T) typeid(T)
#define paxtypeof(T) PAX::TypeHandle(paxtypeid(T))

#endif //POLYPROPYLENE_TYPEINFO_H
