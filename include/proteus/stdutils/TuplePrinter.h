//
// Created by Paul on 20.12.2017.
//

#ifndef POLYPROPYLENE_TUPLEPRINTER_H
#define POLYPROPYLENE_TUPLEPRINTER_H

#include <cstdlib>
#include <ostream>
#include <tuple>

namespace PAX {
    namespace Util {
        template<class Tuple, std::size_t N>
        struct TuplePrinter {
            static void print(const Tuple& t, std::ostream &stream)
            {
                TuplePrinter<Tuple, N-1>::print(t, stream);
                stream << ", " << std::get<N-1>(t);
            }
        };

        template<class Tuple>
        struct TuplePrinter<Tuple, 1> {
            static void print(const Tuple& t, std::ostream &stream)
            {
                stream << std::get<0>(t);
            }
        };
    }
}

#endif //POLYPROPYLENE_TUPLEPRINTER_H
