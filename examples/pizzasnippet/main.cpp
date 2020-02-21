#include "polypropylene/Polypropylene.h"

class Pizza : public PAX::Entity<Pizza> {};

class TomatoSauce : public PAX::Property<Pizza> {
    PAX_PROPERTY(TomatoSauce, PAX_PROPERTY_IS_CONCRETE)
    PAX_PROPERTY_DERIVES(PAX::Property<Pizza>)
    PAX_PROPERTY_IS_SINGLE

public:
    unsigned int scoville = 0;
};

class Cheese : public PAX::Property<Pizza> {
    PAX_PROPERTY(Cheese, PAX_PROPERTY_IS_ABSTRACT)
    PAX_PROPERTY_DERIVES(PAX::Property<Pizza>)
    PAX_PROPERTY_IS_MULTIPLE

    PAX_PROPERTY_DEPENDS_ON(TomatoSauce)
};

class Mozzarella : public Cheese {
    PAX_PROPERTY(Mozzarella, PAX_PROPERTY_IS_CONCRETE)
    PAX_PROPERTY_DERIVES(Cheese)
    PAX_PROPERTY_IS_SINGLE
};

PAX_PROPERTY_IMPL(TomatoSauce)
PAX_PROPERTY_IMPL(Cheese)
PAX_PROPERTY_IMPL(Mozzarella)

int main() {
    Pizza pizza;
    pizza.add(new TomatoSauce()); // otherwise we cannot add cheese
    pizza.add(new Mozzarella());

    // no dynamic_casts here
    pizza.get<TomatoSauce>()->scoville = 3000; // put Tabasco in ;)
    Mozzarella * mozzarella = pizza.get<Mozzarella>();
    const std::vector<Cheese*>& cheeses = pizza.get<Cheese>(); // contains our Mozzarella only
    const std::vector<PAX::Property<Pizza>*> & allProperties = pizza.getProperties();

    return 0;
}