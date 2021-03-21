For converting your own types (from and to strings or json), you have to specialise the `TypeConverter` class.
The [TypeConverter][typeconverter] is an interface for converting two types to each other.
In Polypropylene it is used to parse and write [strings][stringutils] and [json objects][jsontypeconverter].

To parse from and read to json objects you can either
  - Implement the type converter for strings as it is the default fallback solution for reading and writing json objects
  - Implement the type converter for json objects, which we will describe in the following:

To implement a type converter, you can use the convenience macros for
[any type conversion][typeconverter],
[string conversion][stringutils],
or [json conversion][jsontypeconverter].
All macros are analogous so we show how to implement json conversion in the following.
For example, if you want to parse `Chocolate`:

```C++
#include <polypropylene/serialisation/json/JsonTypeConverter.h>

// Put this into a header file.
PAX_DECLARE_JSON_CONVERTER_FOR(Chocolate)

// Put this into a source file.
PAX_IMPLEMENT_JSON_CONVERT_TO(Chocolate) {
    // access the nlohmann::json object under the name 'x'
    float cacao = x["cacao_percentage"];
    return Chocolate(cacao);
}

PAX_IMPLEMENT_JSON_CONVERT_FROM(Chocolate) {
    // access the Chocolate object under the name 'x'
    nlohmann::json node;
    node["cacao_percentage"] = x.getCacao();
    return node;
}
```
<details><summary>
<em>Click here to view expanded macros.</em></summary>
<p>

```C++
#include <polypropylene/serialisation/json/JsonTypeConverter.h>

// Expansion of PAX_DECLARE_JSON_CONVERTER_FOR(Chocolate)
template<>
    class TypeConverter<nlohmann::json, Chocolate> {
    public:
        [[nodiscard]] static Chocolate convertTo(nlohmann::json const & j);
        [[nodiscard]] static nlohmann::json convertFrom(Chocolate const & x);
};

// Expansion of PAX_IMPLEMENT_JSON_CONVERT_TO(Chocolate)
Chocolate PAX::TypeConverter<nlohmann::json, Chocolate>::convertTo(nlohmann::json const & x) {
    float cacao = j["cacaopercentage"];
    return Chocolate(cacao);
}

// Expansion of PAX_IMPLEMENT_JSON_CONVERT_FROM(Chocolate)
nlohmann::json PAX::TypeConverter<nlohmann::json, Chocolate>::convertFrom(Chocolate const & x) {
    nlohmann::json node;
    node["cacao_percentage"] = x.getCacao();
    return node;
}
```
</p>
</details>

Make sure to use these macros outside of any namespaces.
You may want to declare the parser (`PAX_DECLARE_JSON_CONVERTER_FOR`) in a header file and implement it (`PAX_IMPLEMENT_JSON_CONVERT_TO` and `PAX_IMPLEMENT_JSON_CONVERT_FROM`) in a source file.
The macros are defined in [JsonTypeConverter.h][jsontypeconverter] and
refine macros from the more general `TypeConverter` type from [TypeConverter.h][typeconverter].

Don't forget to include your parser wherever your type should be able to be parsed.
For example, you could include it together with `Polypropylene.h` at the begin of your program.

You can use your parser directly or trough the general interface:

```C++
// access your parser directly ...
Chocolate choco1 = PAX::TypeConverter<nlohmann::json, Chocolate>::convertTo(json);
// ... or use general interface
Chocolate choco2 = PAX::Json::convertTo<Chocolate>(json);
```

If your type (e.g., `Chocolate`) should be usable by the reflection system for creating prefabs from json files, you have to register a `IJsonFieldWriter` for your type in the `JsonFieldWriterRegister` you use for your prefabs ([see pizza example](https://github.com/PaulAtTUBS/Polypropylene/blob/master/examples/pizza/main.cpp)).
`IJsonFieldWriters` can read and write `nlohmann::json` objects from and to [`Fields`](https://github.com/PaulAtTUBS/Polypropylene/blob/master/include/polypropylene/reflection/Field.h), which are used for prefab loading and writing.

The default `JsonFieldWriter<T>` reuses the `TypeConverter` for parsing `nlohmann::json` objects.
So to enable `Chocolate` serialisation for properties do the following:

```C++
JsonFieldWriterRegister writerRegister;
JsonEntityPrefab<Pizza>::initialize(writerRegister);

JsonFieldWriter<Chocolate> chocoWriter;
writerRegister.registerWriter(paxtypeid(Chocolate), &chocoWriter);
```

[typeconverter]: https://github.com/pmbittner/Polypropylene/blob/master/include/polypropylene/serialisation/TypeConverter.h
[stringutils]: https://github.com/pmbittner/Polypropylene/blob/master/include/polypropylene/stdutils/StringUtils.h
[jsontypeconverter]: https://github.com/pmbittner/Polypropylene/blob/master/include/polypropylene/serialisation/json/JsonTypeConverter.h