#pragma once
#include "Codebot/Interop/IInterface.h"

namespace Codebot
{
namespace Interop
{

// IFileFind interface

struct IFileFind : public IInterface
{
	static const Guid Id;
	virtual void stdcall Search(const PChar path, const PChar wildcard, Cardinal attributes) = 0;
	virtual Boolean stdcall Next() = 0;
	virtual void stdcall Close() = 0;
	virtual const PChar stdcall Name() = 0;
	virtual Cardinal stdcall Size() = 0;
	virtual Cardinal stdcall Attributes() = 0;
	virtual Double stdcall Date() = 0;
};

extern "C"
{
	// File routines
	import Boolean FileExists(const PChar fileName);
	import LargeWord FileSize(const PChar fileName);
	import Boolean FileDelete(const PChar fileName);
	import Boolean FileRename(const PChar oldName, const PChar newName);
	// Directory routines
	import const PChar DirectoryCurrent();
	import Boolean DirectoryChange(const PChar dir);
	import Boolean DirectoryCreate(const PChar dir);
	import Boolean DirectoryDelete(const PChar dir);
	import Boolean DirectoryExists(const PChar dir);
	import Boolean DirectoryRename(const PChar oldName, const PChar newName);
	// File and directory search interface
	import void FileFindCreate(IFileFind*& find);
};

}
}
