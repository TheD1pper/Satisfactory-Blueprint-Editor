# Satisfactory Blueprint File Format

Struct layout as modeled in `Blueprint/Core/modules/Core.ixx` (module `Core.Types`). A blueprint (`.sbp`) is a `Blueprint`, made of a `BlueprintHeader` followed by a `BlueprintBody`. Fields are listed in serialization order (top to bottom, matching declaration order in the source).

## Overview

A blueprint file is a `Blueprint`, which consists of a header followed by a body, in this order:

| # | Field | Type |
|---|---|---|
| 1 | `Header` | `BlueprintHeader` |
| 2 | `Body` | `BlueprintBody` |

The blueprint header consists of the following fields, in order, fully expanded down to primitive `int`/`uint`/`float`/`bool`/`string` leaves (composite fields are shown as their dotted path; `[]` marks a variable-length list, whose entries each repeat the given sub-layout):

| # | Field | Type |
|---|---|---|
| 1 | `HeaderVersion` | `int32` |
| 2 | `SaveVersion` | `int32` |
| 3 | `BuildVersion` | `int32` |
| 4 | `Dimensions` | `int32[3]` |
| 5 | `Cost` | `ObjectReference[]` — each entry: |
| 5.1 | `Cost[].Level.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 5.2 | `Cost[].Level.Data` | `string` |
| 5.3 | `Cost[].Path.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 5.4 | `Cost[].Path.Data` | `string` |
| 6 | `Recipes` | `ObjectReference[]` — same per-entry layout as `Cost` above |
| 6.1 | `Recipes[].Level.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 6.2 | `Recipes[].Level.Data` | `string` |
| 6.3 | `Recipes[].Path.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 6.4 | `Recipes[].Path.Data` | `string` |
| 7 | `SaveObjectData.Version` | `uint32` |
| 8 | `SaveObjectData.Package.UE4Version` | `int32` |
| 9 | `SaveObjectData.Package.UE5Version` | `int32` |
| 10 | `SaveObjectData.Licensee` | `int32` |
| 11 | `SaveObjectData.Engine.Major` | `uint16` |
| 12 | `SaveObjectData.Engine.Minor` | `uint16` |
| 13 | `SaveObjectData.Engine.Path` | `uint16` |
| 14 | `SaveObjectData.Engine.ChangeList` | `uint32` |
| 15 | `SaveObjectData.Engine.Branch.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 16 | `SaveObjectData.Engine.Branch.Data` | `string` |
| 17 | `SaveObjectData.Custom` | `CustomVersion[]` — each entry: |
| 17.1 | `Custom[].A` | `uint32` |
| 17.2 | `Custom[].B` | `uint32` |
| 17.3 | `Custom[].C` | `uint32` |
| 17.4 | `Custom[].D` | `uint32` |
| 17.5 | `Custom[].Version` | `int32` |

The blueprint body consists of a single field: a flat, ordered list of every object placed in the blueprint.

| # | Field | Type |
|---|---|---|
| 1 | `Objects` | `Object[]` |

Each entry in `Objects` is read as one of two shapes, chosen by its `Type` tag — first the tag, then the shape's fields, in order:

| # | Field | Type |
|---|---|---|
| 1 | `Type` | `ObjectType` (`Actor` or `Object`) |
| 2 | `Header` | `ActorHeader` (if `Type == Actor`) or `ObjectHeader` (if `Type == Object`) |

`ActorHeader` fields, in order, fully expanded:

| # | Field | Type |
|---|---|---|
| 1 | `ClassName.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 2 | `ClassName.Data` | `string` |
| 3 | `Reference.Level.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 4 | `Reference.Level.Data` | `string` |
| 5 | `Reference.Path.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 6 | `Reference.Path.Data` | `string` |
| 7 | `ObjectFlags` | `uint32` |
| 8 | `NeedTransform` | `bool` |
| 9 | `Transform.Rotation` | `float32[4]` |
| 10 | `Transform.Position` | `float32[3]` |
| 11 | `Transform.Scale` | `float32[3]` |
| 12 | `PlacedInLevel` | `bool` |

`ObjectHeader` fields, in order, fully expanded (identical to `ActorHeader` above, with `OuterPathName` inserted after `ObjectFlags`):

| # | Field | Type |
|---|---|---|
| 1 | `ClassName.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 2 | `ClassName.Data` | `string` |
| 3 | `Reference.Level.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 4 | `Reference.Level.Data` | `string` |
| 5 | `Reference.Path.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 6 | `Reference.Path.Data` | `string` |
| 7 | `ObjectFlags` | `uint32` |
| 8 | `OuterPathName.Encoding` | `StrEncoding` (`uint8`-backed enum) |
| 9 | `OuterPathName.Data` | `string` |
| 10 | `NeedTransform` | `bool` |
| 11 | `Transform.Rotation` | `float32[4]` |
| 12 | `Transform.Position` | `float32[3]` |
| 13 | `Transform.Scale` | `float32[3]` |
| 14 | `PlacedInLevel` | `bool` |

The remaining sections below detail each of these types field-by-field, including their own nested types (`SaveObjectVersionData`, `ObjectReference`, `String`, `Transform3f`, etc.).

## Header: `BlueprintHeader`

| Field | Type | Notes |
|---|---|---|
| `HeaderVersion` | `int32` | Version of the header layout itself. |
| `SaveVersion` | `int32` | Engine save-system version the blueprint was written with. |
| `BuildVersion` | `int32` | Game build number. |
| `Dimensions` | `int32[3]` | Blueprint bounding box size (X, Y, Z). |
| `Cost` | `ObjectReference[]` | Items/quantities required to build (added in a later header version). |
| `Recipes` | `ObjectReference[]` | Recipes used by buildings in the blueprint (added in a later header version). |
| `SaveObjectData` | `SaveObjectVersionData` | Engine/package/custom version block written ahead of object data in newer versions. |

### `SaveObjectVersionData`

| Field | Type | Notes |
|---|---|---|
| `Version` | `uint32` | Save object version. |
| `Package` | `PackageFileVersion` | UE4/UE5 package version pair. |
| `Licensee` | `int32` | Licensee version. |
| `Engine` | `EngineVersion` | Engine version that produced the file. |
| `Custom` | `CustomVersion[]` | Custom version container (per-plugin/system versions, e.g. `Dev-Blueprints`). |

#### `PackageFileVersion`

| Field | Type |
|---|---|
| `UE4Version` | `int32` |
| `UE5Version` | `int32` |

#### `EngineVersion`

| Field | Type |
|---|---|
| `Major` | `uint16` |
| `Minor` | `uint16` |
| `Path` (patch) | `uint16` |
| `ChangeList` | `uint32` |
| `Branch` | `String` |

#### `CustomVersion`

A GUID (`A`/`B`/`C`/`D`) identifying the versioned system, plus its version number.

| Field | Type |
|---|---|
| `A` | `uint32` |
| `B` | `uint32` |
| `C` | `uint32` |
| `D` | `uint32` |
| `Version` | `int32` |

### `ObjectReference`

Used for both `Cost` and `Recipes` entries (and inside object headers, below).

| Field | Type |
|---|---|
| `Level` | `String` |
| `Path` | `String` |

### `String`

Not a raw string — a tagged encoding.

| Field | Type | Notes |
|---|---|---|
| `Encoding` | `StrEncoding` | `Utf8` or `Utf16`. |
| `Data` | `std::string` | Raw bytes in the declared encoding. |

## Body: `BlueprintBody`

| Field | Type |
|---|---|
| `Objects` | `Object[]` |

The body is the flat list of every actor/object placed in the blueprint (this corresponds to the TOC section of the underlying save format — see `docs/Satisfactory Blueprint Editor Resources.md`).

### `Object`

A tagged union: every entry is either an actor or a component/object, discriminated by `Type`.

| Field | Type | Notes |
|---|---|---|
| `Type` | `ObjectType` | `Actor` or `Object`. |
| `Header` | `variant<ActorHeader, ObjectHeader>` | Which alternative is active is determined by `Type`. |

#### `ActorHeader` (when `Type == ObjectType::Actor`)

| Field | Type | Notes |
|---|---|---|
| `ClassName` | `String` | UE class of the actor (e.g. `Build_ConveyorBeltMk1_C`). |
| `Reference` | `ObjectReference` | This actor's own level/path identity. |
| `ObjectFlags` | `uint32` | UE object flags. |
| `NeedTransform` | `bool` | Whether `Transform` below is meaningful/present. |
| `Transform` | `Transform3f` | World transform (rotation, position, scale). |
| `PlacedInLevel` | `bool` | Whether the actor was placed directly in the level (vs. spawned). |

#### `ObjectHeader` (when `Type == ObjectType::Object`)

Same as `ActorHeader`, plus an owning object path since non-actor objects are subobjects of something else.

| Field | Type | Notes |
|---|---|---|
| `ClassName` | `String` | UE class of the object. |
| `Reference` | `ObjectReference` | This object's own level/path identity. |
| `ObjectFlags` | `uint32` | UE object flags. |
| `OuterPathName` | `String` | Path of the outer/owning object. |
| `NeedTransform` | `bool` | Whether `Transform` below is meaningful/present. |
| `Transform` | `Transform3f` | World transform (rotation, position, scale). |
| `PlacedInLevel` | `bool` | Whether placed directly in the level. |

### `Transform3f`

| Field | Type | Notes |
|---|---|---|
| `Rotation` | `float32[4]` | Quaternion (X, Y, Z, W). |
| `Position` | `float32[3]` | World position. |
| `Scale` | `float32[3]` | Scale. |

## Supporting types

### `Guid`

Generic 4×`uint32` GUID, same shape as the `A`/`B`/`C`/`D` fields used inline in `CustomVersion`.

| Field | Type |
|---|---|
| `A` | `uint32` |
| `B` | `uint32` |
| `C` | `uint32` |
| `D` | `uint32` |

### `StrEncoding`

| Value | Meaning |
|---|---|
| `Utf8` | `Data` holds UTF-8 bytes. |
| `Utf16` | `Data` holds UTF-16 bytes. |

### `ObjectType`

| Value | Meaning |
|---|---|
| `Actor` | Entry's header is an `ActorHeader`. |
| `Object` | Entry's header is an `ObjectHeader`. |
