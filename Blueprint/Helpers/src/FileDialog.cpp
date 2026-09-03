module;
#include <Windows.h>
#include <ShObjIdl.h>
module Helpers.FileDialog;

import std;

namespace fs = std::filesystem;

namespace FileDialog
{
	std::optional<fs::path> OpenBlueprintFile()
	{
		IFileOpenDialog* Dialog = nullptr;
		HRESULT Hr = CoCreateInstance(
			CLSID_FileOpenDialog,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&Dialog)
		);
		
		if (FAILED(Hr))
			return {};

		std::array<COMDLG_FILTERSPEC, 2> Filters{
			{
				{ L"Satisfactory Blueprint", L"*.sbp" },
				{ L"All Files", L"*.*" }
			}
		};
		Dialog->SetFileTypes(Filters.size(), Filters.data());
		Dialog->SetTitle(L"Open Satisfactory Blueprint");

		Hr = Dialog->Show(nullptr);
		if (FAILED(Hr))
		{
			Dialog->Release();
			return {};
		}

		IShellItem* Item = nullptr;

		Hr = Dialog->GetResult(&Item);
		if (FAILED(Hr))
		{
			Dialog->Release();
			return {};
		}
		
		PWSTR Path = nullptr;
		Hr = Item->GetDisplayName(SIGDN_FILESYSPATH, &Path);
		fs::path Result;

		if (SUCCEEDED(Hr))
		{
			Result = Path;
			CoTaskMemFree(Path);
		}

		Item->Release();
		Dialog->Release();

		return Result;
	}
}