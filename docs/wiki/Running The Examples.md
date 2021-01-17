## Pizza
If you built Polypropylene with json support (`POLYPROPYLENE_WITH_JSON`), the pizza example accesses the project's resource directory.
In order that the resource directory (`/res`) can be found, the pizza example has to be executed from the repository's root directory, i.e., **the working directory has to be the repository's root directory**:
```
mkdir build
cd build
cmake ..
make

cd ..
./build/pizza
```