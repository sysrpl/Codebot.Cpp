unit ImageTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
	Classes,
	ShareTools,
	StreamTools;

type
	IImage = interface(IUnknown)
		['{CCD689A7-ABC1-4136-85F0-754DE0C07CCD}']
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
	end;

procedure ImageCreate(out Image: IImage); cdecl;

implementation

uses
	ImagingTypes, 
	Imaging;

{ TImage class }

type
	TImage = class(TInterfacedObject, IImage)
	private
		FImage: TImageData;
		FLoaded: Boolean;		
		function Convert: Boolean;
		procedure Reset;
	protected
		{ IImage }
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
	public
		destructor Destroy; override;		
	end;

const DefaultFormat = ifA8R8G8B8;

destructor TImage.Destroy;
begin
	Reset;
	inherited Destroy;
end;

function TImage.Convert: Boolean;
begin
	if FLoaded then
	begin	
		FLoaded := ConvertImage(FImage, DefaultFormat);
		if not FLoaded then 
			FreeImage(FImage);
	end;
	Result := FLoaded;
end;

procedure TImage.Reset;
begin
	if FLoaded then
		FreeImage(FImage);
	FLoaded := False;
end;

function  TImage.Generate(Width, Height: Cardinal): Boolean; 
begin
	Reset;
	FLoaded := NewImage(Width, Height, DefaultFormat, FImage);
	Result := FLoaded;
end;


function TImage.LoadFile(FileName: PAnsiChar): Boolean;
begin
	Reset;
	try
		FLoaded := LoadImageFromFile(FileName, FImage);
		Convert;
	except
		FLoaded := False;		
	end;
	Result := FLoaded;
end;

function TImage.LoadStream(Stream: INativeStream): Boolean;
var
	S: TStream;
begin
	Reset;
	if Stream = nil then
	begin
		Result := False;
		Exit;
	end;
	S := TNativeStream.Create(Stream);
	try
		FLoaded := LoadImageFromStream(S, FImage);
		Convert;
	except
		FLoaded := False;		
	end;
	S.Free;
	Result := FLoaded;
end;

function TImage.SaveFile(FileName: PAnsiChar): Boolean;
begin
	if FLoaded then
	try
		Result := SaveImageToFile(FileName, FImage);
	except
		Result := False;
	end
	else
		Result := False;
end;

function TImage.SaveStream(Extension: PAnsiChar; Stream: INativeStream): Boolean;
var
	S: TStream;
begin
	if FLoaded then
	begin
		if Stream = nil then
		begin
			Result := False;
			Exit;
		end;
		S := TNativeStream.Create(Stream);
		try
			Result := SaveImageToStream(Extension, S, FImage);
			Convert;
		except
			Result := False;
		end;
		S.Free;
	end
	else
		Result := False;
end;

function TImage.GetFileFormat(FileName: PAnsiChar): PAnsiChar;
begin
	Return(Result, DetermineFileFormat(FileName));
end;

function TImage.GetStreamFormat(Stream: INativeStream): PAnsiChar;
var
	S: TStream;
begin
	if Stream = nil then
	begin
		Return(Result, '');
		Exit;
	end;
	S := TNativeStream.Create(Stream);
	try
		Return(Result, DetermineStreamFormat(S));
	finally
		S.Free;
	end;
end;

function TImage.Flip: Boolean;
begin
	if FLoaded then
		Result := FlipImage(FImage)
	else
		Result := False;
end;

function TImage.Mirror: Boolean;
begin
	if FLoaded then
		Result := MirrorImage(FImage)
	else
		Result := False;
end;

function TImage.Resize(Width, Height: Cardinal): Boolean;
begin
	if FLoaded then
	try
		Result := ResizeImage(FImage, Width, Height, rfBicubic)
	except
		Result := False;
	end
	else
		Result := False;
end;

function TImage.Loaded: Boolean; 
begin
	Result := FLoaded;
end;

function TImage.Width: Cardinal;
begin
	if FLoaded then
		Result := FImage.Width
	else
		Result := 0;
end;

function TImage.Height: Cardinal;
begin
	if FLoaded then
		Result := FImage.Height
	else
		Result := 0;
end;

function TImage.Bits: Pointer;
begin
	if FLoaded then
		Result := FImage.Bits
	else
		Result := nil;
end;

procedure ImageCreate(out Image: IImage); 
begin
	Image := TImage.Create;
end;

end.
