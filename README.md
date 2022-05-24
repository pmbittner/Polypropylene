# Polypropylene - Let Your Objects Become Anything You Wish

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Official documentation](https://img.shields.io/badge/Documentation-Read-green.svg)](https://github.com/PaulAtTUBS/Polypropylene/wiki)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/PaulAtTUBS/Polypropylene.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/PaulAtTUBS/Polypropylene/context:cpp)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/PaulAtTUBS/Polypropylene.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/PaulAtTUBS/Polypropylene/alerts/)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/b2c602e50b99464fbdd4caab644cd3d0)](https://www.codacy.com/manual/PaulAtTUBS/Polypropylene?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=PaulAtTUBS/Polypropylene&amp;utm_campaign=Badge_Grade)
<!--[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3539/badge)](https://bestpractices.coreinfrastructure.org/projects/3539)-->

Polypropylene is an embedded domain-specific language (EDSL) for C++17 to model separation of concerns at any granularity, from single objects to large frameworks.
It allows defining objects dynamically by composition of properties.
Properties add state and functionality to objects similar to mixins but are composed during runtime.

Polypropylene started as an [*Entity-Component System (ECS)*][gppcomp] but grew more general and flexible.
An ECS is a design pattern mostly used in video game engineering where objects are required to change behaviour and shape frequently and arbitrarily.
Thereby, an object (i.e., an *Entity*) is purely defined as a composition of individual components.
We refer to *Components* as *Properties* to emphasize that Polypropylene is not limited to but can be used as an ECS.
You may find a detailed description of ECS in the [ecs-faq](https://github.com/SanderMertens/ecs-faq).

During development several issues arose that were not addressed by existing ECS' so far.
Most prominently, I was missing an ECS which allows you to write the style you know: object-oriented and not data-oriented.
To not lose benefits of a data-oriented design, Polypropylene abstracts memory management in a backend, while allowing to program in an object-oriented fashion.

Below you find a small excerpt on how to define entities and properties in Polypropylene:

```C++
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
    
    virtual ~Cheese() = 0;
};

class Mozzarella : public Cheese {
    PAX_PROPERTY(Mozzarella, PAX_PROPERTY_IS_CONCRETE)
    PAX_PROPERTY_DERIVES(Cheese)
    PAX_PROPERTY_IS_SINGLE
};

void main() {
    Pizza pizza;
    pizza.add(new TomatoSauce()); // otherwise we cannot add cheese
    pizza.add(new Mozzarella());

    // no dynamic_casts here
    pizza.get<TomatoSauce>()->scoville = 3000; // put Tabasco in ;)
    Mozzarella * mozzarella = pizza.get<Mozzarella>();
    const std::vector<Cheese*>& cheeses = pizza.get<Cheese>(); // There might be multiple cheese types apart from mozzarella, so we get a vector.
}
```

Opposed to existing Entity-Component Systems and Dynamic Mixin Implementations, Polypropylene offers the following
features:

-   **Polymorphism Awareness**: Properties added to an entity are identified by each of their polymorphic types.
A property can be retrieved from an entity not only by its direct type but also by any of its super types.
In the above example, the `cheeses` vector also contains the `mozzarella` that was added to the `pizza` because class `Mozzarella` derives `Cheese`.
**No runtime type checks**, such as `dynamic_cast`, are necessary for property insertion, deletion, and retrieval.
Those operations have constant runtime (i.e. O(1)) as they depend on the length of the inheritance chain only.
As reflection is unavailable in C++, the parent property type has to be specified explicitly via `PAX_PROPERTY_DERIVES`.

-   **Property Multiplicity**: Entities may or may not contain several properties of the same type.
For each property type, its multiplicity (`PAX_PROPERTY_IS_SINGLE` or `PAX_PROPERTY_IS_MULTIPLE`) specifies whether the
same entity is allowed to contain multiple or only a single property of that type.
For example, arbitrarily different types of `Cheese` may be put onto your `Pizza` whereas any concrete `Cheese`, such as
`Mozzarella`, occurs only once.
Multiplicity is also considered upon retrieving objects as either the single instance or a list of contained properties
is returned.

-   **Property Dependencies**: Property types can specify dependencies on other properties with `PAX_PROPERTY_DEPENDS_ON`.
A property can only be added to an entity if all its dependencies are met.
Thus, properties can always assume that at least one property of each depending type is present in their entity.
In our example, `Cheese` can only be added to the `Pizza` if it contains `TomatoSauce`.

Apart from that, Polypropylene is:

-   **General**: Polypropylene is **general-purpose library**, useful and usable in any C++ project where objects are
required to alter arbitrarily during runtime or dynamic types are necessary.
It is neither restricted to nor designed for video games only.

-   **Fast**: The main operations for property composition `add`, `remove`, and `get` all have constant runtime
(i.e. O(1)) and do not require dynamic type checks.

-   **Object- but Data-Oriented**: Polypropylene is designed for object-oriented programming and thus properties are
deliberately allowed to exhibit custom behaviour.
Nevertheless, as properties are arbitrary classes, they can be implemented as plain old data (POD) for a strict ECS implementation.
Property allocation is centralised.
Dedicated allocators can be registered for each property type.
By default, pool allocators are used but may be replaced by any **custom allocator** by implementing the [`Allocator<size_t>`][allocator] interface.
Polypropylene comes with a default implementation for a pool and a malloc allocator.

-   **Standalone**: Polypropylene does not have any additional dependencies to other libraries except for the C++17
standard and [nlohmann::json][nlohmannjson] which is bundled with Polypropylene (and may be excluded from compilation).

-   **Platform-Independent**: Polypropylene is tested on Windows with MSVC and on Linux with GCC and Clang.

Polypropylene further contains the following features:

-   **Event Service**: Communication between properties is enabled by an event service per entity.
    Properties can emit and receive events similar to the publisher-subscriber design pattern.
    Entities will send `PropertyAdded-` and `PropertyRemovedEvents` to their event service upon adding or removing properties, respectively.
    Event services may optionally be linked such that events can be exchanged between entities or even throughout the whole program.
    ```C++
    class Pizza : public PAX::Entity<Pizza> {
    public:
        void bake() {
            getEventService().fire(BakedEvent());
        }
    };
    
    class Cheese : public PAX::Property<Pizza> {
        /** more code **/
    public:
        void attached(Pizza & p) override {
            p.getEventService().add<BakedEvent, Cheese, &Cheese::onPizzaBaked>(this);
        }
    
        void onPizzaBaked(BakedEvent & b) {
            std::cout << "Cheese turns brown ~~~" << std::endl;
        }
    };
    
    void main() {
        Pizza p;
        p.add(new Cheese());
        p.bake();
    }
    
    >>> Cheese turns brown ~~~
    ```
    Furthermore, a simplified specialised **EventHandler** inspired by C# events can be used to register and listen for specific event types.

-   **Serialisation**: Entities may be specified entirely in json files.
    (Other formats are not supported yet but can be integrated.)
    > margherita.json
    
    ```json
    {
      "Properties": [
        {
          "PAX::Examples::Mozzarella": {}
        },
        {
          "PAX::Examples::TomatoSauce": {
            "scoville": "9001"
          }
        }
      ]
    }
    ```
    
    > funghi.json
    
    ```json
    {
      "Inherits": [
        "margherita.json"
      ],
      "Properties": [
        {
          "PAX::Examples::Champignon": {}
        }
      ]
    }
    ```
    From such files so called [**prefabs**][prefab] are created.
    A prefab represents the kind of entity described in the json file.
    It allows instantiating arbitrary amounts of the corresponding entity:
    ```C++
    JsonEntityPrefab<Pizza> prefab = prefabLoader.load("res/pizza/funghi.json");
    Pizza * pizzaFunghi = prefab.create({});
    ```
    To customise the individual instances created by a prefab, custom parameters may be specified.
    For example, the scoville of the `TomatoSauce` can be made variable in the json file by introducing a variable name:
    ```json
    "scoville": "${hotness}"
    ```
    Upon pizza creation, this value can be specified arbitrarily.
    That way, customers are able to be delivered pizza as hot as they like :fire::
    ```C++
    std::cout << "How hot do you like your pizza (in scoville)?\n";
    std::string hotness;
    std::cin >> hotness;
    
    Pizza * pizzaFunghi = prefab->create({{"hotness", hotness}});
    ```
    As C++ does not provide reflection information, properties can provide metadata by their own for de-/serialisation:
    ```C++
    ClassMetadata TomatoSauce::getMetadata() {
        ClassMetadata m = Super::getMetadata();
        m.add(paxfieldof(scoville)).flags |= Field::IsMandatory;
        return m;
    }
    ```
    Prefabs can also be created from entities with `Entity::toPrefab()`.
    This will return a prefab that allows instantiating copies of the original entity at the point in time the prefab was created.
    Thus, any entity can be copied and the state of entities can be saved.
    Furthermore, **prefabs can be stored to json files**, as done in our [pizza example][example_pizza_main]:
    ```C++
    Path outPath = "res/pizza/out/funghiWith" + hotness + "scoville.json";
    PrototypeEntityPrefab<Pizza> prefabToSerialise = pizzaFunghi->toPrefab();
    JsonEntityPrefab<Pizza> asJson(prefabToSerialise);
    prefabLoader.write(asJson, outPath);
    ```

-   **EntityManagers and -Views**: Managing collections of entities can be done with `EntityManagers`.
    `EntityManagers` link the event services of all contained entities to their own central `EventService`.
    Thus, you can listen to all events broadcasted by the entities of an `EntityManager` at one place.

    `EntityManagerViews` provide a filtered view on all entities in a given `EntityManager`:
    ```C++
    EntityManager<Pizza> manager;
    manager.add(pizzaMargherita);
    manager.add(pizzaFunghi);
    manager.add(pizzaSalami);
    
    EntityManagerView<Pizza, Champignon> champignonView(manager);
    const std::vector<Pizza*> & pizzasWithChampignons = champignonView.getEntities();
    
    EntityManagerView<Pizza, Mozzarella, Salami> salamiView(manager);
    const std::vector<Pizza*> & pizzasThatAreAtLeastPizzaSalami = salamiView.getEntities();
    ```
<!--
-   **Systems**: Systems can be used for any kinds of (global) behaviour.
    In entity component systems, they are used to manage and execute behaviour on properties.
    For this reason, `PropertyOwningSystems` contain all properties of a given type by managing their allocation with a pool allocator.
    That allows for fast iteration over all properties of a certain type.
    ```C++
    // The system will register itself as the allocator to use for TomatoSauce.
    // Hence, it needs to be created before creating any TomatoSauce.
    PropertyOwningSystem<Pizza, TomatoSauce> tomatoValley;
    
    Pizza p;
    p.add(new pax_new(TomatoSauce)());
    
    for (TomatoSauce * t : tomatoValley) {
        // Here each active TomatoSauce that was allocated with the pizza's allocator can be found.
    }
    ```
-->

## Further Utilities
-   **Logging**: Polypropylene records errors and warnings with a custom logger.
               By default, logs are printed to `std::cout` and `std::cerr` but can be piped to any custom `ostream`.
               Logging also records timestamp, severity, and the name of the function containing the log statement.
               Several levels of severity for messages, such as `Info`, `Debug`, or `Error`, are available.

-   **Path**: Struct for platform agnostic path specification, simplification, and navigation.

-   Inclusion of the [nlohmann::json library][nlohmannjson] for loading json files.

## Customisations for Compile Time Minimisation
To obtain reflection information, we frequently use templates and macros.
We made certain features, such as loading from json files, optional such that you do not have to compile code that you do not need.
The following cmake options allow compile time customisation.
By default, all options are activated (set to ON):

-   `POLYPROPYLENE_WITH_JSON`: Includes the [nlohmann::json library][nlohmannjson] for loading and writing `EntityPrefabs` from and to json files.
-   `POLYPROPYLENE_WITH_EXAMPLES`: Specifies if examples should be built or not.
-   `POLYPROPYLENE_WITH_TESTS`: Specifies if tests should be built or not.

## Code Examples

The small pizza example from the top of this readme file can be found in [`examples/pizzasnippet/`][example_pizzasnippet].
It is exemplified in further detail in [`examples/pizza/`][example_pizza].
We suggest heading to the [`main.cpp`][example_pizza_main] for getting an entry point and quick start.

An extensive example for the usage of Polypropylene can be found in the game engine [PaxEngine3][pax3].
In PaxEngine3, both [Game Objects][pax3_gameobject] and [Game Worlds][pax3_gameworld] are implemented as Entities of Polypropylene.

For further instructions, please have a look at our [wiki].

## Future Plans and Open Issues
-   Type System: Identifying entities by the properties they implement.
For example, a `Pizza` with `TomatoSauce`, `Mozzarella`, and `Champignons` could be identified as `PizzaFunghi`.
A mechanism for manual or even automatic type specification could be implemented.

-   Enhancing Dependency Specification: Currently, properties are allowed to depend on other property types.
Entities however may only be allowed to contain certain combinations (configurations) of properties.
For instance, two property types could be exclusive to each other as they implement respectively contradicting data or behaviour.
Such additional constraints could be implemented by using techniques from software product-line engineering, especially feature models.

-   Dependencies are not considered upon property removal, yet.
 When removing a property, dependencies of other properties may break.

-   Make PoolAllocator dynamic: Currently, the pool allocator has a fixed size of 1024 elements.
Making it dynamically growable (e.g., with paging), saves memory and would allow for "arbitrarily" many allocated elements.

-   Ordering of properties with multiplicity `PAX_PROPERTY_IS_MULTIPLE` such that the vector returned by `Entity::get` is sorted.

-   Support for Older C++ Standards Than 17: Therefore, we need a custom implementation of `std::optional`.
We have to see if that would pay off.

## Feedback
I welcome any constructive critism, questions, and bug reports.
Feel free to open issues or pull-requests here on GitHub. :blush:

## Naming Conventions and "Where does the name come from?"
Polypropylene is a backronym for *POLYmorphism aware PROPertY Library for mutablE dyNamic objEcts*.
As this is a rather long name, we kept our short and memorisable inter-project namespace `PAX`.

## Related Work
If we could not get your interest in using our library, there are other great projects which you might want to check out.

Entity Component Systems:
- [entityx](https://github.com/alecthomas/entityx) - A fast, type-safe C++ Entity-Component system 
- [entt](https://github.com/skypjack/entt) - Gaming meets modern C++ - a fast and reliable entity component system (ECS) and much more 

Dynamic Mixins:
- [dynamix](https://github.com/iboB/dynamix) - A new take on polymorphism in C++ 
- [libciabatta](https://github.com/atomgalaxy/libciabatta) - The C++ Mixin Support Library: Sandwich Mixins all the way 
- [dyno](https://github.com/ldionne/dyno) - Runtime polymorphism done right

More interesting projects can be found in the [ecs-faq](https://github.com/SanderMertens/ecs-faq). Did I forget your library? Please open an issue or pull-request!

## Are You Hungry For Pizza :pizza: Now?
At least we are ... :yum:

[gppcomp]: http://gameprogrammingpatterns.com/component.html

[allocator]: include/polypropylene/memory/Allocator.h

[nlohmannjson]: https://github.com/nlohmann/json

[wiki]: https://github.com/pmbittner/Polypropylene/wiki

[prefab]: include/polypropylene/prefab/Prefab.h

[example_pizzasnippet]: examples/pizzasnippet
[example_pizza]: examples/pizza
[example_pizza_main]: examples/pizza/main.cpp

[pax3]: https://github.com/pmbittner/PaxEngine3
[pax3_gameobject]: https://github.com/pmbittner/PaxEngine3/blob/master/include/paxcore/gameentity/GameEntity.h
[pax3_gameworld]: https://github.com/pmbittner/PaxEngine3/blob/master/include/paxcore/world/World.h
