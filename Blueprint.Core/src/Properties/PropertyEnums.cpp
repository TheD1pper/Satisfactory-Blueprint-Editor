#include <string_view>

#include "Properties/PropertyEnums.hpp"

namespace Core::Property
{
	namespace Array
	{
		const std::unordered_map<ElementType, std::string_view> ElementMap{
			{ElementType::LinearColor, "LinearColor"},
			{ElementType::Vector, "Vector"},
			{ElementType::SpawnData, "SpawnData"},
			{ElementType::BlueprintCategoryRecord, "BlueprintCategoryRecord"},
			{ElementType::BlueprintSubCategoryRecord, "BlueprintSubCategoryRecord"},
			{ElementType::DroneTripInformation, "DroneTripInformation"},
			{ElementType::FactoryCustomizationColorSlot, "FactoryCustomizationColorSlot"},
			{ElementType::FeetOffset, "FeetOffset"},
			{ElementType::FGCachedConnectedWire, "FGCachedConnectedWire"},
			{ElementType::FGDroneFuelRuntimeData, "FGDroneFuelRuntimeData"},
			{ElementType::GCheckmarkUnlockData, "GCheckmarkUnlockData"},
			{ElementType::GlobalColorPreset, "GlobalColorPreset"},
			{ElementType::HardDriveData, "HardDriveData"},
			{ElementType::HighlightedMarkerPair, "HighlightedMarkerPair"},
			{ElementType::Hotbar, "Hotbar"},
			{ElementType::InventoryStack, "InventoryStack"},
			{ElementType::ItemAmount, "ItemAmount"},
			{ElementType::MapMarker, "MapMarker"},
			{ElementType::MessageData, "MessageData"},
			{ElementType::MiniGameResult, "MiniGameResult"},
			{ElementType::PhaseCost, "PhaseCost"},
			{ElementType::PrefabIconElementSaveData, "PrefabIconElementSaveData"},
			{ElementType::PrefabTextElementSaveData, "PrefabTextElementSaveData"},
			{ElementType::ProjectAssemblyLaunchSequenceValue, "ProjectAssemblyLaunchSequenceValue"},
			{ElementType::ResearchData, "ResearchData"},
			{ElementType::ResearchTime, "ResearchTime"},
			{ElementType::ResourceSinkHistory, "ResourceSinkHistory"},
			{ElementType::ScannableObjectData, "ResourceObjectData"},
			{ElementType::ScannableResourcePair, "ScannableResourcePair"},
			{ElementType::SchematicCost, "SchematicCost"},
			{ElementType::ShoppingListBlueprintEntry, "ShoppingListBlueprintEntry"},
			{ElementType::ShoppingListClassEntry, "ShoppingListClassEntry"},
			{ElementType::ShoppingListRecipeEntry, "ShoppingListRecipeEntry"},
			{ElementType::SplinePointData, "SplinePointData"},
			{ElementType::SplitterSortRule, "SplitterSortRule"},
			{ElementType::SubCategoryMaterialDefault, "SubCategoryMaterialDefault"},
			{ElementType::TimeTableStop, "TimeTableStop"},
			{ElementType::WireInstance, "WireInstance"}
		};
	}

	namespace Map
	{
		const std::unordered_map<KeyType, std::string_view> KeyMap{
			{KeyType::Object, "ObjectProperty"},
			{KeyType::Int, "IntProperty"},
			{KeyType::Struct, "StructProperty"}
		};

		const std::unordered_map<ValueType, std::string_view> ValueMap{
			{ValueType::Byte, "ByteProperty"},
			{ValueType::Int, "IntProperty"},
			{ValueType::Int64, "Int64Property"},
			{ValueType::Struct, "StructProperty"}
		};
	}

	namespace Set
	{
		const std::unordered_map<ElementType, std::string_view> ElementMap{
			{ElementType::Uint32, "Uint32Property"},
			{ElementType::Struct, "StructProperty"},
			{ElementType::Object, "ObjectProperty"}
		};
	}

	namespace Struct
	{
		const std::unordered_map<ElementType, std::string_view> TypedMap{
			{ElementType::Vector, "Vector"},
			{ElementType::LinearColor, "LinearColor"},
			{ElementType::Quat, "Quat"},
			{ElementType::FluidBox, "FluidBox"},
			{ElementType::InventoryItem, "InventoryItem"},
			{ElementType::RailroadTrackPosition, "RailroadTrackPosition"},
			{ElementType::DataTime, "DataTime"},
			{ElementType::ClientIdentityInfo, "ClientIdentityInfo"}
		};
	}
}