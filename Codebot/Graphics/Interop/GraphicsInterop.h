#pragma once
#include <Codebot/Interop/IInterface.h>
#include <Codebot/Interop/IStream.h>

namespace Codebot
{
namespace Graphics
{
namespace Interop
{

/*
function Generate(Width, Height: Cardinal): Boolean; stdcall;
function LoadFile(FileName: PAnsiChar): Boolean; stdcall;
function LoadStream(Stream: INativeStream): Boolean; stdcall;
function SaveFile(FileName: PAnsiChar): Boolean; stdcall;
function SaveStream(Extension: PAnsiChar; Stream: INativeStream): Boolean; stdcall;
function GetFileFormat(FileName: PAnsiChar): PAnsiChar; stdcall;
function GetStreamFormat(Stream: INativeStream): PAnsiChar; stdcall;
function Flip: Boolean; stdcall;
function Mirror: Boolean; stdcall;
function Resize(Width, Height: Cardinal): Boolean; stdcall;
function Loaded: Boolean; stdcall;
function Width: Cardinal; stdcall;
function Height: Cardinal; stdcall;
function Bits: Pointer; stdcall;
*/


struct IImage : public Codebot::Interop::IInterface
{
	typedef Codebot::Interop::IStream IStream;
	static const Codebot::Interop::Guid Id;
	virtual Boolean stdcall Generate(Cardinal width, Cardinal height) = 0;
	virtual Boolean stdcall LoadFile(const PChar fileName) = 0;
	virtual Boolean stdcall LoadStream(IStream* stream) = 0;
	virtual Boolean stdcall SaveFile(const PChar fileName) const = 0;
	virtual Boolean stdcall SaveStream(const PChar extension, IStream* stream) const = 0;
	virtual const PChar stdcall GetFileFormat(const PChar fileName) const  = 0;
	virtual const PChar stdcall GetStreamFormat(IStream* stream) const  = 0;
	virtual Boolean stdcall Flip() const = 0;
	virtual Boolean stdcall Mirror() const = 0;
	virtual Boolean stdcall Resize(Cardinal width, Cardinal height) const = 0;
	virtual Boolean stdcall Loaded() const = 0;
	virtual Cardinal stdcall Width() const  = 0;
	virtual Cardinal stdcall Height() const  = 0;
	virtual Byte* stdcall Bits() const = 0;
};

extern "C"
{
	import void ImageCreate(IImage*& image);
};

}
}
}
