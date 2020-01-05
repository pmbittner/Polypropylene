# Polypropylene - Let Your Objects Become Anything You Wish

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/PaulAtTUBS/Polypropylene.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/PaulAtTUBS/Polypropylene/context:cpp)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/PaulAtTUBS/Polypropylene.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/PaulAtTUBS/Polypropylene/alerts/)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3539/badge)](https://bestpractices.coreinfrastructure.org/projects/3539)

Polypropylene is a C++17 library for dynamic object definition by composition of properties.
Properties add state and functionality to objects similar to mixins but are composed dynamically during runtime.
Polypropylene started as an _Entity-Component System (ECS)_ but grew more general and flexible soon.
An ECS is a design pattern mostly used in video game engineering where objects are required to change behaviour and shape frequently and arbitrarily.
Thereby, an object (i.e., an _Entity_) is purely defined as a composition of individual components.
We refer to _Components_ as _Properties_ because Polypropylene is not a pure ECS anymore but a general-purpose library.
During development several issues arose that were not addressed by existing ECS so far.

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
    
    
    Pizza pizza;
    pizza.add(new TomatoSauce()); // otherwise we cannot add cheese
    pizza.add(new Mozzarella());
    
    // no dynamic_casts here
    Mozzarella * mozzarella = pizza.get<Mozzarella>();
    const std::vector<Cheese*>& cheeses = pizza.get<Cheese>(); // contains our Mozzarella only
    

**Opposed to existing Entity-Component Systems and Dynamic Mixin Implementations**, Polypropylene offers the following features:

- **Polymorphism Awareness**: Properties added to an entity are identified by each of their polymorphic types.
A property can be retrieved from an entity not only by its direct type but also by any of its super types.
**No runtime type checks**, such as `dynamic_cast`, are necessary for property insertion, deletion, and retrieval.
Those operations have constant runtime (i.e. O(1)) as they depend on the length of the inheritance chain only.
As reflection is unavailable in C++, the parent property type has to be specified explicitly via `PAX_PROPERTY_DERIVES`.

- **Property Multiplicity**: Entities may or may not contain several properties of the same type.
For each property type, its multiplicity (`PAX_PROPERTY_IS_SINGLE` or `PAX_PROPERTY_IS_MULTIPLE`) specifies whether the same entity is allowed to contain multiple or only a single property of that type.
For example, arbitrarily different kinds of `Cheese` may be put onto your `Pizza` whereas any concrete `Cheese`, such as `Mozzarella`, occurs only once.
Multiplicity is also considered upon retrieving objects as either the single instance or a list of contained properties is returned.

- **Property Dependencies**: Property types can specify dependencies on other properties with `PAX_PROPERTY_DEPENDS_ON`.
A property can only be added to an entity if all its dependencies are met.
Thus, properties can always assume that at least one property of each depending type is present in their entity.
In our example, `Cheese` can only be added to the `Pizza` if it contains `TomatoSauce`.

... and thereby is:

- **General**: Polypropylene is **general-purpose library**, useful and usable in any C++ project where objects are required to alter arbitrarily during runtime or dynamic types are necessary.
It is neither restricted to nor designed for video games only.

- **Fast**: The main operations for property composition `add`, `remove`, and `get` all have constant runtime (i.e. O(1)) and do not require dynamic type checks.

- **Object- but Data-Oriented**: Polypropylene is designed for object-oriented programming and thus properties are deliberately allowed to exhibit custom behaviour.
Nevertheless, as properties are arbitrary classes, they can be implemented as plain old data (POD).
Property allocation is centralised.
Dedicated allocators can be registered for each property type.
By default, pool allocators are used but may be replaced by any **custom allocator** on program start.
Polyproylene contains a pool and a malloc allocator implementation so far.

[//]: # The `Property<T>` base class as well as the macros for property specification do not contain / generate any fields but only methods.

- **Standalone**: Polypropylene does not have any additional dependencies to other libraries except for the C++17 standard.
Support for older C++ versions is planned.

- **Platform-Independent**: Polypropylene is tested on Windows with MSVC and on Linux with GCC and Clang.

Polypropylene further contains the following features:

- **Event Service**: Communication between properties is enabled by an event service per entity.
Properties can emit and receive events similar to the publisher-subscriber design pattern.
The event service is also used for `PropertyAdded-` and `PropertyRemovedEvents`.
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

- **Serialisation**: Entities may be specified entirely in json files.
    (Other formats are not supported yet but can be integrated easily.)
    > margherita.json
    ```json
    {
      "Properties": {
        "PAX::Examples::Mozzarella": {},
        "PAX::Examples::TomatoSauce": {
          "scoville": "9001"
        }
      }
    }
    ```
    > funghi.json
    ```json
    {
      "Inherits": [
        "margherita.json"
      ],
      "Properties": {
        "PAX::Examples::Champignon": {}
      }
    }
    ```
    From such files so called **prefabs** are created.
    A prefab represents the kind of entity described in the json file.
    It allows instantiating arbitrary amounts of the corresponding entity:
    ```
    JsonEntityPrefab<Pizza> prefab = prefabLoader.load("res/pizza/funghi.json");
    Pizza * pizzaFunghi = prefab.create({});
    ```
    To customise the individual instances created by a prefab, custom parameters may be specified.
    For example, the scoville of the TomatoSauce can be made variable by replacing it in the json file by:
    ```
    "scoville": "${spicyness}"
    ```
    Upon pizza creation, this value can be specified arbitrarily.
    That way, customers are able to be delivered pizza as hot as they like:
    ```
    std::cout << "How spicy do you like you pizza (in scoville)?\n";
    std::string spicyness;
    std::cin >> spicyness;
  
    Pizza * pizzaFunghi = prefab->create({{"spicyness", spicyness}});
    ```
    As C++ does not provide reflection information, properties can provide metadata by their own for de-/serialisation:
    ```
    ClassMetadata TomatoSauce::getMetadata() {
        ClassMetadata m = Super::getMetadata();
        m.add(paxfieldof(scoville), Field::IsMandatory);
        return m;
    }
     ```
    Prefabs can also be created from entities with `Enity::toPrefab()`.
    This will return a prefab that allows instantiating copies of the original entity at the point in time the prefab was created.
    Thus, any entity can be copied and the state of entities can be saved.
    Furthermore, **prefabs can be stored to json files**.
    
- **Views**: Managing collections of entities can be done with `EntityManagers`.
  `EntityManagers` link the event services of all contained entities such that a central `EventService` receives and broadcasts the events of all entities.
  `EntityManagerViews` provide a filtered view on all entities in a given `EntityManager`:
  ```
  EntityManager<Pizza> manager;
  manager.add(pizzaMargherita);
  manager.add(pizzaFunghi);
  manager.add(pizzaSalami);
  
  EntityManagerView<Pizza, Champignon> champignonView(manager);
  const std::vector<Pizza*> & pizzasWithChampignons = champignonView.getEntities();
  
  EntityManagerView<Pizza, Mozzarella, Salami> salamiView(manager);
  const std::vector<Pizza*> & pizzasThatAreAtLeastPizzaSalami = salamiView.getEntities();
  ```
  
- **Systems**: Systems can be used for any kinds of (global) behaviour.
  In entity component systems, they are used to manage and execute behaviour on properties.
  For this reason, `PropertyOwningSystems` contain all properties of a given type by managing their allocation with a pool allocator.
  That allows for fast iteration over all properties of a certain type.
  ```
  // The system will register itself as the allocator to use for TomatoSauce.
  // Hence, it needs to be created before creating any TomatoSauce.
  PropertyOwningSystem<Pizza, TomatoSauce> tomatoValley;
  
  Pizza p;
  p.add(new (Pizza::GetPropertyAllocator().allocate<TomatoSauce>()) TomatoSauce());
  
  for (TomatoSauce * t : tomatoValley) {
    // Here each active TomatoSauce that was allocated with the pizza's allocator can be found.
  }
  ```


#### Further Utilities:
- **Logging**: Polypropylene records errors and warnings with a custom logger.
               By default, logs are printed to `std::cout` and `std::cerr` but can be piped to any custom `ostream`.
               Logging also records timestamp, severity, and the name of the function containing the log statement.
               Several levels of severity for messages, such as `Info`, `Debug`, or `Error`, are available.
- **Path**: Struct for platform agnostic path specification, simplification, and navigation.
- Inclusion of the [nlohmann::json library][1] for loading json files.


#### Customisations for Compile Time Minimisation
To obtain reflection information, we frequently use templates and macros.
We made certain features, such as loading from json files, optional such that you do not have to compile code that you do not need.
The following cmake options allow compile time customisation.
By default, all options are activated (set to ON):

- `POLYPROPYLENE_WITH_JSON`: Includes the [nlohmann::json library][1] for loading and writing `EntityPrefabs` from and to json files.
- `POLYPROPYLENE_WITH_EXAMPLES`: Specifies if examples should be built or not.

## Code Examples

The small pizza example from the top of this readme file can be found in `examples/pizzasnippet/`.
It is exemplified in further detail in `examples/pizza/`.
We suggest heading to the `main.cpp` for getting an entry point and quick start.

We plan to publish examples of usage of Polypropylene in video game development but this requires more code polishing, yet.


## Future Plans and Open Issues
- Type System: Identifying entities by the properties they implement.
For example, a `Pizza` with `TomatoSauce`, `Mozzarella`, and `Champignons` could be identified as `PizzaFunghi`.
A mechanism for manual or even automatic type specification has to be implemented.

- Enhancing Dependency Specification: Currently, properties are allowed to depend on other property types.
Entities however may only be allowed to contain certain combinations (configurations) of properties.
For instance, two property types could be exclusive to each other as they implement respectively contradicting data or behaviour.
Such additional constraints could be implemented by using techniques from software product line engineering, especially feature models.

- Dependencies are not considered upon property removal, yet.
 When removing a property, dependencies of other properties may break.

- Support for Older C++ Standards Than 17: Therefore, we need a custom implementation of `std::optional`.
We have to see if that would pay off.


## Feedback
I welcome any constructive critism or questions.
Feel free to open issues or pull-requests here on GitHub.


## Naming Conventions and "Where does the name come from?"
Polypropylene is a backronym for _POLYmorphism aware PROPertY Library for mutablE dyNamic objEcts_.
As this is a rather long name, we kept our short and memorisable inter-project namespace `PAX`.


## Are You Hungry For Pizza Now?
At least we are ...

[1]: https://github.com/nlohmann/json
