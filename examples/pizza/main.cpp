//
// Created by Bittner on 05.12.2019.
//

#include <iostream>

#include "polypropylene/Polypropylene.h"

#ifdef PAX_WITH_JSON
#include "polypropylene/serialisation/json/JsonLoader.h"
#include "polypropylene/serialisation/json/property/JsonEntityPrefabLoader.h"
#include "polypropylene/serialisation/json/JsonParser.h"
#endif

#include "Pizza.h"
#include "toppings/Mozzarella.h"
#include "toppings/Champignon.h"
#include "toppings/TomatoSauce.h"

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

#ifdef PAX_WITH_JSON
    using namespace PAX::Json;

    JsonParserRegister jsonParserRegister; /// contains parsers from nlohmann::json to any custom type
    JsonEntityPrefabLoader<Pizza> prefabLoader; /// loads json files from disk to EntityPrefab<Pizza>
    JsonEntityPrefab<Pizza>::initialize(jsonParserRegister);
#endif

    /// EXAMPLE
    std::cout << "How spicy do you like you pizza (in scoville)?\n";
#ifdef PAX_WITH_JSON
    std::string spicyness;
    std::cin >> spicyness;

    JsonEntityPrefab<Pizza> prefab = prefabLoader.load("res/pizza/funghi.json");
    Pizza * pizzaFunghi = prefab.create({{"spicyness", spicyness}});
#else
    int spicyness;
    std::cin >> spicyness;

    Pizza * pizzaFunghi = new Pizza();

    /// Ideally, the allocation service is used for allocating memory of properties.
    /// Using the allocation service is optional.
    /// It can be used with placement new:
    ///   new (Pizza::GetPropertyAllocator().allocate<TomatoSauce>()) TomatoSauce(spicyness)
    /// Notice that using the allocation service has benefits though:
    ///   - PropertyOwningSystems only operate on properties allocated by the allocation service.
    ///     Manually allocated properties as done here won't be recognised.
    ///   - Entities take ownership of properties added to them. Deleting an entity also deletes
    ///     all contained properties but only those that were allocated with the allocation service.
    ///     Thus, beware of memory leaks as customly allocated properties have to be deleted manually!
    TomatoSauce tomatoSauce(spicyness);
    Mozzarella mozzarella;
    Champignon champignon;

    /// Add TomatoSauce first because Cheeses (e.g., Mozzarella) depend on it.
    pizzaFunghi->add(&tomatoSauce);
    pizzaFunghi->add(&mozzarella);
    pizzaFunghi->add(&champignon);
#endif

    pizzaFunghi->yummy();
    pizzaFunghi->bake();

    /// example for property access via templates
    {
        const std::vector<Cheese *> & cheeses = pizzaFunghi->get<Cheese>();
        Mozzarella * m = pizzaFunghi->get<Mozzarella>();
    }

    /// example for property access without templates
    {
        /// use the get function according to properties multiplicity if you know it ...
        const std::vector<Topping *> & cheeses = pizzaFunghi->getMultiple(paxtypeid(Cheese));
        Mozzarella * m = dynamic_cast<Mozzarella*>(pizzaFunghi->getSingle(paxtypeid(Mozzarella)));

        /// ... or if you do not:
        std::vector<Topping *> mozzarellas = pizzaFunghi->get(paxtypeid(Mozzarella));
    }

#ifdef PAX_WITH_JSON
    Path outPath = "res/pizza/out/funghiWith" + spicyness + "scoville.json";
    PrototypeEntityPrefab<Pizza> prefabToSerialise = pizzaFunghi->toPrefab();
    JsonEntityPrefab<Pizza> asJson(prefabToSerialise);
    prefabLoader.write(asJson, outPath);
#endif

    delete pizzaFunghi;

    return 0;
}
