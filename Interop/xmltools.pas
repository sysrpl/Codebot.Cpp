unit XmlTools; 

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
	ShareTools;

type
	INodeList = interface;
	IDocument = interface;

	INode = interface
		['{BC90FD97-E83D-41BB-B4D8-3E25AA5EB2C6}']
		function Instance: Pointer; stdcall;
		function GetDocument(out Document: IDocument): Boolean; stdcall;
		function GetParent(out Node: INode): Boolean; stdcall;
		function SelectNode(XPath: PAnsiChar; out Node: INode): Boolean; stdcall;
		function SelectList(XPath: PAnsiChar; out List: INodeList): Boolean; stdcall;
		procedure Attributes(out List: INodeList); stdcall;
		procedure Nodes(out List: INodeList); stdcall;
		function GetKind: Integer; stdcall;
		function GetName: PAnsiChar; stdcall;
		function GetText: PAnsiChar; stdcall;
		procedure SetText(Value: PAnsiChar); stdcall;
		function GetXml: PAnsiChar; stdcall;
		procedure SetXml(Value: PAnsiChar); stdcall;
	end;

	INodeList = interface
		['{D36A2B84-D31D-4134-B878-35E8D33FD067}']
		procedure Clear; stdcall;
		procedure Add(Node: INode); overload; stdcall;
		procedure Add(Name: PAnsiChar; out Node: INode); overload; stdcall;
		procedure Remove(Node: INode); overload; stdcall;
		procedure Remove(Name: PAnsiChar); overload; stdcall;
		procedure Item(Index: Integer; out Node: INode); overload; stdcall;
		function Item(Name: PAnsiChar; out Node: INode): Boolean; overload; stdcall;
		function Length: Integer; stdcall;
	end;

	IDocument = interface(INode)
		['{B713CB91-C809-440A-83D1-C42BDF806C4A}']
		procedure Beautify; stdcall;
		procedure CreateAttribute(const Name: PAnsiChar; out Node: INode); stdcall;
		procedure CreateElement(const Name: PAnsiChar; out Node: INode); stdcall;
		procedure SetRoot(Node: INode); stdcall;
		function GetRoot(out Node: INode): Boolean; stdcall;
		procedure Load(FileName: PAnsiChar); stdcall;
		procedure Save(FileName: PAnsiChar); stdcall;
	end;

var
	XmlCreated: Integer;
	XmlRefCount: Integer;

procedure XmlDocumentCreate(out Document: IDocument); cdecl;
function XmlGetCreated: Integer; cdecl;
function XmlGetRefCount: Integer; cdecl;

implementation

{$ifdef unix} 
{$i xmllinux.pas}
{$else}
{$i xmlwindows.pas}
{$endif}

function XmlGetCreated: Integer; 
begin
	Result := XmlCreated;
end;

function XmlGetRefCount: Integer; 
begin
	Result := XmlRefCount;
end;

end.
