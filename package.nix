{
  lib,
  stdenv,
  cmake,
  gtest,
  withTests ? true,
}:

stdenv.mkDerivation {
  pname = "polypropylene";
  version = "1.0.0";

  src = lib.fileset.toSource {
    root = ./.;
    fileset = lib.fileset.unions [
      ./CMakeLists.txt
      ./cmake
      ./include
      ./src
      ./examples
      ./test
    ];
  };

  nativeBuildInputs = [ cmake ];
  checkInputs = [ gtest ];

  cmakeFlags = [
    (lib.cmakeBool "POLYPROPYLENE_WITH_EXAMPLES" withTests)
    (lib.cmakeBool "POLYPROPYLENE_WITH_TESTS" withTests)
  ];

  doCheck = withTests;

  meta = {
    description = "Entity-property system for C++";
    license = lib.licenses.mit;
    platforms = lib.platforms.unix;
  };
}
