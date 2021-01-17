For parsing your own types, you have to specialise the `TryParser` class.
This can be done with convenience macros.
For example, if you want to parse `Chocolate`:

```C++
#include <polypropylene/serialisation/json/JsonParser.h>

// Put this into a header file.
PAX_DECLARE_JSONPARSER_FOR(Chocolate)

// Put this into a source file.
PAX_IMPLEMENT_JSONPARSER_FOR(Chocolate) {
    // access the json object under the name 'json'
    float cacao = json["cacao_percentage"];
    return Chocolate(cacao);
}
```
<details><summary>
<em>Click here to view expanded macros.</em></summary>
<p>

```C++
#include <polypropylene/serialisation/json/JsonParser.h>

// Expansion of PAX_DECLARE_JSONPARSER_FOR(Chocolate)
namespace PAX {
    template<>
    class TryParser<nlohmann::json, Chocolate> {
    public:
        PAX_NODISCARD static Chocolate tryParse(const nlohmann::json &);
    };
}

// Expansion of PAX_IMPLEMENT_JSONPARSER_FOR(Chocolate)
Chocolate PAX::TryParser<nlohmann::json, Chocolate>::tryParse(const nlohmann::json & json) {
    float cacao = j["cacaopercentage"];
    return Chocolate(cacao);
}
```
</p>
</details>

Make sure to use these macros outside of any namespaces.
You may want to declare the parser (`PAX_DECLARE_JSONPARSER_FOR`) in a header file and implement it (`PAX_IMPLEMENT_JSONPARSER_FOR`) in a source file.
The macros are defined in [JsonParser.h](https://github.com/pmbittner/Polypropylene/blob/master/include/polypropylene/serialisation/json/JsonParser.h).

Don't forget to include your parser wherever your type should be able to be parsed.
For example, you could include it together with `Polypropylene.h` at the begin of your program.

You can use your parser directly or trough the general interface:

```C++
// access your parser directly ...
Chocolate choco1 = PAX::TryParser<nlohmann::json, Chocolate>::tryParse(json);
// ... or use general interface
Chocolate choco2 = PAX::Json::tryParse<Chocolate>(json);
```

If your type (e.g., `Chocolate`) should be usable by the reflection system for creating prefabs from json files, you have to register a `IJsonFieldWriter` for your type in the `JsonFieldWriterRegister` you use for your prefabs ([see pizza example](https://github.com/PaulAtTUBS/Polypropylene/blob/master/examples/pizza/main.cpp)).
`IJsonFieldWriters` can read and write `nlohmann::json` objects from and to [`Fields`](https://github.com/PaulAtTUBS/Polypropylene/blob/master/include/polypropylene/reflection/Field.h), which are used for prefab loading and writing.

The default `JsonFieldWriter<T>` reuses the `TryParser` for parsing `nlohmann::json` objects.
So to enable `Chocolate` serialisation for properties do the following:

```C++
JsonFieldWriterRegister writerRegister;
JsonEntityPrefab<Pizza>::initialize(writerRegister);

JsonFieldWriter<Chocolate> chocoWriter;
writerRegister.registerWriter(paxtypeid(Chocolate), &chocoWriter);
```

For writing your own types back to json, `JsonFieldWriter<T>` uses the `<<` operator for writing to `ostreams` so far. If you have a better idea, please let me know.