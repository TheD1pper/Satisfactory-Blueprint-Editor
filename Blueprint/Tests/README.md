# Blueprint.Tests

Unit test project for the Blueprint editor, built on [Catch2](https://github.com/catchorg/Catch2) (amalgamated, single-header build vendored under `Vendor/Catch2`).

## Directory layout

Tests mirror the source tree they cover, one subfolder per Blueprint module:

```
Blueprint.Tests/
│
├── Tests.lua              premake5 project (auto-globs all .cpp/.hpp below)
├── Entry.cpp               Catch2 custom main()
│
├── Core/                   tests for Blueprint/Core
│   ├── Types/              → Core.Data / Core.Complex (BasicDataTypes, enums, ...)
│   ├── Properties/         → Core/modules/Properties/*.ixx (Array/Map/Set/Struct property types)
│   └── ...
│
├── Parser/                 tests for Blueprint/Parser
│   ├── HeaderParserTests.cpp    → BlueprintHeader parsing
│   ├── BodyParserTests.cpp      → Parser.ixx / Parser.cpp
│   ├── PropertyParserTests.cpp  → Property.cpp
│   └── ...
│
└── Services/                tests for Blueprint/Services
    ├── BlueprintManagerTests.cpp  → BlueprintManager.ixx/.cpp
    └── ...
```

This is a convention, not something premake enforces — subfolders exist only to keep tests organized next to the module they exercise. Follow the existing `Blueprint/<Module>/{modules,src}` layout when deciding where a new test belongs.

> **Note:** as of writing, only the scaffolding (`Entry.cpp`, `Tests.lua`) exists. The tree above is the target layout — create the module subfolders as you add coverage for each area.

## Naming convention

- One `.cpp` file per class/component under test, suffixed `Tests`, e.g. `BlueprintManagerTests.cpp`, `ArrayPropertyTests.cpp`.
- Inside each file, group related assertions with Catch2 `TEST_CASE` + `SECTION`, e.g.:

  ```cpp
  #include "catch_amalgamated.hpp"
  import Core.Property;

  TEST_CASE("ArrayProperty round-trips values", "[Core][Properties]")
  {
      SECTION("empty array")
      {
          // ...
      }
  }
  ```

- Use Catch2 tags (`[Core]`, `[Parser]`, `[Services]`, `[Properties]`, ...) matching the folder/module so tests can be filtered at the command line, e.g. `Tests.exe [Parser]`.

## Adding a new test file

1. Drop a new `.cpp` in the matching module subfolder (create it if it doesn't exist yet).
2. `#include "catch_amalgamated.hpp"` and `import` whatever Blueprint module(s) you're testing (they're already available via the `includedirs`/`links` set in `Tests.lua`).
3. Re-run premake (`build.bat`) so the new file is added to the generated project — required once per new file since premake globs at generation time, not at compile time.

## Building & running

```
buildvs22.bat            # regenerates the Visual Studio 2022 solution via premake5
```

Then build the `Tests` project (in the `Blueprint` group) from the generated solution. The binary lands at `bin/<config>/Blueprint/Tests/Tests.exe`.

Run all tests:

```
Tests.exe
```

Run a subset by tag or name:

```
Tests.exe [Parser]
Tests.exe "ArrayProperty round-trips values"
Tests.exe --list-tests
```
