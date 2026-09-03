module;
export module Helpers.FileDialog;
import std;

namespace fs = std::filesystem;

namespace FileDialog
{
	export std::optional<fs::path> OpenBlueprintFile();
}