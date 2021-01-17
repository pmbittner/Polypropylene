Polypropylene is a pure CMake project.
It does not depend on any third-party software.
Building follows the straightforward CMake workflow.
When you are in the project's root directory, run the following commands to build:
```
mkdir build
cd build
cmake ..
make
```

## Options
Building Polypropylene can be customised.
The following CMake options configure Polypropylene's build.
By default, all options are activated, i.e., set to ON.

- `POLYPROPYLENE_WITH_JSON`: Includes the [nlohmann::json library][1] for loading and writing `EntityPrefabs` from and to json files.
- `POLYPROPYLENE_WITH_EXAMPLES`: Specifies if examples should be built or not.

## Linking
Polypropylene is built as a static library.
To link your project against Polyproplene, you need to:

- Add Polypropylene's `include/` directory to your includes.
  In CMake this can be done with `include_directories(path/to/Polypropylene/include)`.
- Link your library against `polypropylene.a` / `polypropylene.lib`, depending on your compiler.
  In CMake this can be done with `target_link_libraries(yourprogramtarget polypropylene)`.

We do not provide any find scripts so far.
Choose the way that works best for you to locate Polypropylene in your file system.
For example, Polypropylene can directly be included into your project's `CMakeLists.txt` (`add_subdirectory(Polypropylene)`) by using symlinks or git submodules.

[1]: https://github.com/nlohmann/json