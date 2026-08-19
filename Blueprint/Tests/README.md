# Blueprint.Tests

Unit test project for the Blueprint editor, built on [Catch2](https://github.com/catchorg/Catch2) (amalgamated, single-header build vendored under `Vendor/Catch2`).

## How it's wired up

- `Tests.lua` — premake5 project definition. Builds a `ConsoleApp` named `Tests`, linked against `Core`, `Parser`, `Services`, `Helpers`, and `Catch2`. It globs every `*.cpp`/`*.hpp` in this folder and its subfolders, so **any new `.cpp` file placed anywhere under `Blueprint/Tests/` is picked up automatically** — no need to edit `Tests.lua` when adding test files.
- `EntryPoint.cpp` — the test runner's `main()`. Uses a custom Catch2 session (`CATCH_AMALGAMATED_CUSTOM_MAIN`, see `Vendor/Catch2/Catch2.lua`) with `showSuccessfulTests = true` so passing assertions are printed too, not just failures.
- Included from the root `premake5.lua` via `include "Blueprint/Tests/Tests.lua"` inside the `Blueprint` group.

## Directory layout

```
Blueprint.Tests/
│
├── EntryPoint.cpp             Catch2 custom main()
├── Tests.lua                  premake5 project (auto-globs all .cpp/.hpp below)
│
├── Fixtures/                  reusable Catch2 test fixtures
│   └── BlueprintFixture.hpp   base fixture that loads/parses a blueprint for use across test cases
│
├── Helpers/                   shared test utilities (distinct from Blueprint/Helpers, the source module)
│   ├── TestFile.hpp           resolves paths into Data/ and reads fixture files
│   └── TestAssertions.hpp     custom Catch2 matchers / assertion helpers
│
├── Data/                      sample .sbp blueprint files used as test input
│   ├── ValidBlueprint.sbp
│   └── InvalidBlueprint.sbp
│
├── Parser/                    tests for Blueprint/Parser
│   ├── HeaderTests.cpp        → BlueprintHeader parsing
│   ├── BodyTests.cpp          → Parser.ixx / Parser.cpp
│   └── PropertyTests.cpp      → Property.cpp
│
└── Services/                  tests for Blueprint/Services
    └── BlueprintManagerTests.cpp → BlueprintManager.ixx/.cpp
```

`Parser/` and `Services/` mirror the `Blueprint/<Module>/{modules,src}` layout of the module they cover — follow that pattern if coverage is added for another module (e.g. `Core/`).

> **Note:** as of writing, only the scaffolding (`EntryPoint.cpp`, `Tests.lua`) exists. The tree above is the target layout — create the folders/files as you add coverage.

## Fixtures, Helpers & Data

- **`Fixtures/`** — Catch2 fixture classes (types used as the first argument to `TEST_CASE_METHOD`) that set up shared state, e.g. `BlueprintFixture` parsing a blueprint once so multiple `SECTION`s can exercise it.
- **`Helpers/`** — free functions/macros used across test files: `TestFile.hpp` for locating and reading files out of `Data/`, `TestAssertions.hpp` for assertion helpers that don't belong in any one test file.
- **`Data/`** — raw `.sbp` sample files fed into the parser during tests. Not compiled; referenced at runtime via `Helpers/TestFile.hpp`. Name files after the scenario they represent (`ValidBlueprint.sbp`, `InvalidBlueprint.sbp`, ...) so the intent is clear from the test output alone.

## Naming convention

- One `.cpp` file per component under test, e.g. `HeaderTests.cpp`, `BlueprintManagerTests.cpp`.
- Group related assertions with Catch2 `TEST_CASE`/`SECTION`, or `TEST_CASE_METHOD` when using a fixture from `Fixtures/`:

  ```cpp
  #include "catch_amalgamated.hpp"
  #include "Fixtures/BlueprintFixture.hpp"
  #include "Helpers/TestFile.hpp"

  TEST_CASE_METHOD(BlueprintFixture, "Parser reads a valid blueprint header", "[Parser]")
  {
      SECTION("header fields are populated")
      {
          // ...
      }
  }
  ```

- Tag tests by module (`[Parser]`, `[Services]`, ...) so they can be filtered at the command line, e.g. `Tests.exe [Parser]`.

## Adding a new test file

1. Drop a new `.cpp` in the matching module subfolder (create it if it doesn't exist yet).
2. `#include "catch_amalgamated.hpp"`, any needed `Fixtures/`/`Helpers/` headers, and `import` whatever Blueprint module(s) you're testing (already available via the `includedirs`/`links` set in `Tests.lua`).
3. Re-run premake (`buildvs22.bat`) so the new file is added to the generated project — required once per new file since premake globs at generation time, not at compile time.

## Building & running

```
buildvs22.bat         # regenerates the Visual Studio 2022 solution via premake5
```

Then build the `Tests` project (in the `Blueprint` group) from the generated solution. The binary lands at `bin/<config>/Blueprint/Tests/Tests.exe`.

Run all tests:

```
Tests.exe
```

Run a subset by tag or name:

```
Tests.exe [Parser]
Tests.exe "Parser reads a valid blueprint header"
Tests.exe --list-tests
```
