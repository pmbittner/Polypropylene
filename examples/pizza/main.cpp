//
// Created by Bittner on 05.12.2019.
//

#include <iostream>

#include "polypropylene/Polypropylene.h"
#include "polypropylene/memory/PropertyPool.h"

#ifdef PAX_WITH_JSON
#include "polypropylene/serialisation/json/JsonLoader.h"
#include "polypropylene/serialisation/json/property/JsonEntityPrefabLoader.h"
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

    PAX_LOG(PAX::Log::Level::Info, "Running Pizza Example");

    /// INITIAL SETUP
    registerPropertyTypes();

#ifdef PAX_WITH_JSON
    using namespace PAX::Json;

    JsonFieldWriterRegister writerRegister; /// contains parsers from nlohmann::json to fields
    JsonEntityPrefabLoader<Pizza> prefabLoader; /// loads json files from disk to EntityPrefab<Pizza>
    JsonEntityPrefab<Pizza>::initialize(writerRegister);
#endif

    /// EXAMPLE

    /// Create a property pool for champignons.
    /// The pool will hold all allocated instances of champignons.
    /// It will do so by communicating with the AllocationService of Pizza.
    /// It's usage is optional.
    PropertyPool<Champignon> champignonPool;

    std::cout << "How hot do you like your pizza (in scoville)?\n";
#ifdef PAX_WITH_JSON
    std::string hotness;
    std::cin >> hotness;

    JsonEntityPrefab<Pizza> prefab = prefabLoader.load("res/pizza/funghi.json");
    Pizza * pizzaFunghi = prefab.create({{"hotness", hotness}});
#else
    int hotness;
    std::cin >> hotness;

    Pizza * pizzaFunghi = new Pizza();

    /// Either allocate properties where and how you like ...
    TomatoSauce tomatoSauce(hotness);
    Mozzarella * mozzarella = new Mozzarella();
    /// ... or (optionally) use the allocation service.
    /// Using the allocation service has the following benefits:
    /// - PropertyPools only know the properties allocated by the allocation service.
    ///   Manually allocated properties as done here won't be recognised.
    /// - Entities take ownership of properties added to them. Deleting an entity also deletes
    ///   all contained properties but only those that were allocated with the allocation service.
    ///   Thus, beware of memory leaks as customly allocated properties have to be deleted manually!
    /// PropertyFactories and Prefabs always use the allocation service.
    Champignon * champignon = pax_new(Champignon)();

    /// Add TomatoSauce first because Cheeses (e.g., Mozzarella) depend on it.
    pizzaFunghi->add(&tomatoSauce);
    pizzaFunghi->add(mozzarella);
    pizzaFunghi->add(champignon);
#endif

    pizzaFunghi->yummy();
    pizzaFunghi->bake();

    /// Let's take a look at all allocated champignons.
    PAX_LOG(Log::Level::Info, "List of allocated champignons:");
    for (Champignon * c : champignonPool) {
        PAX_LOG(Log::Level::Info, "    " << c);
        assert(c->getOwner() == pizzaFunghi);
    }

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
    /// Export pizzaFunghi as json.
    { /// Open a scope here to remove the temporary prefabs we create.
        Path outPath = "res/pizza/out/funghiWith" + hotness + "scoville.json";
        PrototypeEntityPrefab<Pizza> prefabToSerialise = pizzaFunghi->toPrefab();
        JsonEntityPrefab<Pizza> asJson(prefabToSerialise);
        prefabLoader.write(asJson, outPath);

        PAX_LOG(Log::Level::Info, "Your pizza was delivered to '" << outPath << "'.");
    }
#endif

#ifdef PAX_WITH_JSON
    /// Entities created with prefabs (or with the allocation service) have to be deleted via the allocation service.
    Pizza::GetAllocationService().deleteAndFree<Pizza>(pizzaFunghi);
#else
    delete pizzaFunghi;
#endif

    return 0;
}
