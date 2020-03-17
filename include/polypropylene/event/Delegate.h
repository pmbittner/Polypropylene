//
// Created by Paul on 31.05.2017.
//

#ifndef POLYPROPYLENE_DELEGATE_H
#define POLYPROPYLENE_DELEGATE_H

namespace PAX {
    template<typename... Args>
    class EventHandler;
    class EventService;

    template<typename... Args>
    struct Delegate {
    private:
        friend class EventHandler<Args...>;
        friend class EventService;

        void *callee = nullptr;
        void (*method)(void*, Args...);

    public:
        Delegate(void* callee, void (*method)(void*, Args...)) : callee(callee), method(method) {}

        friend bool operator==(Delegate const& lhs, Delegate const& rhs) {
            return (lhs.callee == rhs.callee) && (lhs.method == rhs.method);
        }
    };
}

#endif //POLYPROPYLENE_DELEGATE_H
