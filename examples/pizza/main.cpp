//
// Created by Bittner on 05.12.2019.
//

#include "Pizza.h"
#include "toppings/Mozzarella.h"

#include "polypropylene/json/JsonLoader.h"
#include "polypropylene/property/construction/json/JsonEntityPrefabLoader.h"
#include "toppings/Champignon.h"
#include "toppings/TomatoSauce.h"

void registerPropertyTypes() {
    using namespace PAX;
    PAX_PROPERTY_REGISTER(Proteus::Examples::Mozzarella)
    PAX_PROPERTY_REGISTER(Proteus::Examples::Champignon)
    PAX_PROPERTY_REGISTER(Proteus::Examples::TomatoSauce)
}

int main(int argc, char** argv) {
    using namespace PAX;
    using namespace Proteus;
    using namespace Proteus::Examples;

    /// INITIAL SETUP
    registerPropertyTypes();

    // Todo: Find a way to prebuild this
    Resources resources;
    Json::JsonLoader jsonLoader;
    Json::JsonEntityPrefabLoader<Pizza> prefabLoader(resources);
    Json::JsonEntityPrefab<Pizza>::initialize(resources);
    resources.registerLoader(&jsonLoader);
    resources.registerLoader(&prefabLoader);


    /// EXAMPLE
    using PizzaPrefab = EntityPrefab<Pizza>;
    std::shared_ptr<PizzaPrefab> prefab = resources.loadOrGet<PizzaPrefab>(Path("res/pizza/funghi.json"));

    std::cout << "How spicy do you like you pizza (in scoville)?\n";
    std::string spicyness;
    std::cin >> spicyness;

    Pizza * pizzaFunghi = prefab->create({{"spicyness", spicyness}});
    pizzaFunghi->yummy();

    pizzaFunghi->bake();

    std::vector<Cheese*> cheeses = pizzaFunghi->get<Cheese>();
    Mozzarella * g = pizzaFunghi->get<Mozzarella>();

    return 0;
}


/*

struct Printable {
    char* (*print)(void) = NULL;
    void (*ficken)(int, int) = NULL;
};

struct MoesGeileKlasse {
    char geilheit = 9001;
};

/// Printable impl for Moe
MoesGeileKlasse * __Printable_Moe;

char * _MoesGeileKlasse_Printable_print() {
    return &__Printable_Moe->geilheit;
}

void _MoesGeileKlasse_Printable_ficken(int a, int b) {

}

Printable * getPrintableMoe() {
    Printable * p = new Printable;
    p->print = &_MoesGeileKlasse_Printable_print;
    p->ficken = &_MoesGeileKlasse_Printable_ficken;
    return p;
}

float mainsss() {
    MoesGeileKlasse moe;
    __Printable_Moe = &moe;
    Printable * moePrintImpl = getPrintableMoe();
    printf(moePrintImpl->print);
    moePrintImpl->ficken(1, 2);
    __Printable_Moe = NULL;
}
//*/