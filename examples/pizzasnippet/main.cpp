#include "polypropylene/Polypropylene.h"

class Pizza : public PAX::Entity<Pizza> {};

class TomatoSauce : public PAX::Property<Pizza> {
    PAX_PROPERTY(TomatoSauce, PAX_PROPERTY_IS_CONCRETE)
    PAX_PROPERTY_DERIVES(PAX::Property<Pizza>)
    PAX_PROPERTY_IS_SINGLE
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

PAX_PROPERTY_INIT(TomatoSauce) {}
PAX_PROPERTY_INIT(Cheese) {}
PAX_PROPERTY_INIT(Mozzarella) {}

int main() {
    Pizza pizza;
    pizza.add(new TomatoSauce()); // otherwise we cannot add cheese
    pizza.add(new Mozzarella());

    Mozzarella * mozzarella = pizza.get<Mozzarella>();
    const std::vector<Cheese*>& cheeses = pizza.get<Cheese>(); // contains our Mozzarella only
    const std::vector<PAX::Property<Pizza>*> & allProperties = pizza.getProperties();

    return 0;
}