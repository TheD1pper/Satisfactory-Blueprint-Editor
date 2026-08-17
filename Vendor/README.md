# Vendor

Third-party libraries built from source live here, one subfolder per library.

To add a library:

1. Drop its source under `Vendor/<LibraryName>/`.
2. Add a `<LibraryName>.lua` premake script in that folder defining `project "<LibraryName>"` (see any `Blueprint.*` project for the pattern: `kind`, `language`, `targetdir`/`objdir`, `files`, `includedirs`).
3. Uncomment/add its `include "Vendor/<LibraryName>/<LibraryName>.lua"` line inside the `group "Vendor"` block in the root `premake5.lua`.
4. In whichever project needs it, add the library to `includedirs` and `links`.
