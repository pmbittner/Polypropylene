//
// Created by Paul on 08.05.2017.
//

#ifndef POLYPROPYLENE_EVENT_H
#define POLYPROPYLENE_EVENT_H

namespace PAX {
    struct Event {
    private:
        bool consumed = false;

    public:
        virtual ~Event();
        virtual void reuse();
        void consume();
        bool isConsumed();
    };
}

#endif //POLYPROPYLENE_EVENT_H
