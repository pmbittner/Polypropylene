//
// Created by Bittner on 05.12.2019.
//

#include "Pizza.h"
#include "toppings/Mozzarella.h"
#include "toppings/Champignon.h"
#include "toppings/TomatoSauce.h"

#include "polypropylene/property/construction/EntityPrefab.h"

#ifdef PAX_WITH_JSON
#include "polypropylene/json/JsonLoader.h"
#include "polypropylene/property/construction/json/JsonEntityPrefabLoader.h"
#include "polypropylene/json/JsonParser.h"
#endif

/**
 * Each type of property has to be registered before it can be used in prefabs.
 * A property is registered by exactly that name given to PAX_PROPERTY_REGISTER.
 * For instance,
 * \code{.cpp}
 *     PAX_PROPERTY_REGISTER(PAX::Examples::Mozzarella)
 * \endcode
 * registers Mozzarella as "PAX::Examples::Mozzarella",
 * whereas
 * \code{.cpp}
 *     using namespace PAX::Examples;
 *     PAX_PROPERTY_REGISTER(Mozzarella)
 * \endcode
 * registers Mozzarella as "Mozzarella".
 * The registered name is used for identifiying property types upon deserialisation, e.g., in json files.
 * Custom names can be specified with PAX_PROPERTY_REGISTER_AS(propertyType, strName)
 */
void registerPropertyTypes() {
    PAX_PROPERTY_REGISTER(PAX::Examples::Mozzarella);
    PAX_PROPERTY_REGISTER(PAX::Examples::Champignon);
    PAX_PROPERTY_REGISTER(PAX::Examples::TomatoSauce);
}

int main(int argc, char** argv) {
    using namespace PAX;
    using namespace PAX::Examples;

    /// INITIAL SETUP
    registerPropertyTypes();

    // Todo: Find a way to prebuild this
#ifdef PAX_WITH_JSON
    Resources resources;
    JsonParserRegister::Instance()->registerParser(paxtypeof(int), new JsonParser<int>());
    Json::JsonLoader jsonLoader;
    Json::JsonEntityPrefabLoader<Pizza> prefabLoader(resources);
    Json::JsonEntityPrefab<Pizza>::initialize(resources);
    resources.registerLoader(&jsonLoader);
    resources.registerLoader(&prefabLoader);
#endif

    /// EXAMPLE
    std::cout << "How spicy do you like you pizza (in scoville)?\n";
#ifdef PAX_WITH_JSON
    std::string spicyness;
    std::cin >> spicyness;

    using PizzaPrefab = EntityPrefab<Pizza>;
    std::shared_ptr<PizzaPrefab> prefab = resources.loadOrGet<PizzaPrefab>(Path("res/pizza/funghi.json"));

    Pizza * pizzaFunghi = prefab->create({{"spicyness", spicyness}});
#else
    float spicyness;
    std::cin >> spicyness;

    Pizza * pizzaFunghi = new Pizza();
    pizzaFunghi->add(new TomatoSauce());
    pizzaFunghi->add(new Mozzarella());
    pizzaFunghi->add(new Champignon());

    pizzaFunghi->get<TomatoSauce>()->setSpicyness(spicyness);
#endif

    pizzaFunghi->yummy();
    pizzaFunghi->bake();

    std::vector<Cheese*> cheeses = pizzaFunghi->get<Cheese>();
    Mozzarella * g = pizzaFunghi->get<Mozzarella>();

    return 0;
}
