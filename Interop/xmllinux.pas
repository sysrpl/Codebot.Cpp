uses
	Xml2;

type
  TInterfacedImpl = class(TObject, IInterface)
  protected
    FRefCount: Integer;
    function QueryInterface(const IID: TGUID; out Obj): HResult; stdcall;
    function _AddRef: Integer; stdcall;
    function _Release: Integer; stdcall;
  public
    procedure AfterConstruction; override;
    procedure BeforeDestruction; override;
    class function NewInstance: TObject; override;
    property RefCount: Integer read FRefCount;
  end;

procedure TInterfacedImpl.AfterConstruction;
begin
  InterlockedDecrement(XmlRefCount);
  InterlockedDecrement(FRefCount);
end;

procedure TInterfacedImpl.BeforeDestruction;
begin
  if RefCount <> 0 then
    Error(reInvalidPtr);
end;

class function TInterfacedImpl.NewInstance: TObject;
begin
  Result := inherited NewInstance;
  InterlockedIncrement(XmlCreated);
  InterlockedIncrement(XmlRefCount);
  TInterfacedImpl(Result).FRefCount := 1;
end;

function TInterfacedImpl.QueryInterface(const IID: TGUID; out Obj): HResult;
begin
  if GetInterface(IID, Obj) then
    Result := 0
  else
    Result := E_NOINTERFACE;
end;

function TInterfacedImpl._AddRef: Integer;
begin
  InterlockedIncrement(XmlRefCount);
  Result := InterlockedIncrement(FRefCount);
end;

function TInterfacedImpl._Release: Integer;
begin
  InterlockedDecrement(XmlRefCount);
  Result := InterlockedDecrement(FRefCount);
  if Result = 0 then
    Destroy;
end;

type
	TXPathResult = array of Pointer;

{ libxml2 helper functions }

{$region 'libxml2'}
function xmlGetElementIndex(Node: xmlNodePtr): Integer;
var
	N: xmlNodePtr;
begin
	Result := 1;
	if Node._type <> XML_ELEMENT_NODE then
		Exit;
	if Node.parent = nil then
		Exit;
	N := Node.parent.children;
	while N <> Node do
	begin
		if N._type = XML_ELEMENT_NODE then
			Inc(Result);
		N := N.next;
	end;
end;

function xmlRelativeQuery(Node: xmlNodePtr; const XPath: AnsiString): AnsiString;
var
	S: AnsiString;
	I: Integer;
begin
	Result := XPath;
	if Length(Result) = 0 then
		Exit;
	if Result[1] = '/' then
		Exit;
	if Result[1] = '.' then
		Exit;
	while Node <> nil do
	begin
		if Node._type <> XML_ELEMENT_NODE then
			Break;
		I := xmlGetElementIndex(Node);
		Str(I, S);
		Result := Node.name + '[' + S + ']/' + Result;
		Node := Node.parent;
	end;
	Result := '/' + Result;
end;

function xmlConvertXPath(PathObject: xmlXPathObjectPtr): TXPathResult;
var
	Nodes: xmlNodeSetPtr;
	List: xmlNodePtrPtr;
	Node: xmlNodePtr;
	I: Integer;
begin
	Result := nil;
	if (PathObject = nil) or (PathObject.nodesetval = nil) then
		Exit;
	Nodes := PathObject.nodesetval;
	if Nodes.nodeNr < 1 then
		Exit;
	SetLength(Result, Nodes.nodeNr);
	List := Nodes.nodeTab;
	for I := 0 to Nodes.nodeNr - 1 do
	begin
		Node := List^;
		Result[I] := Node;
		Inc(List);
	end;
end;

function xmlFirstElementChild(Node: xmlNodePtr) : xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Node := Node.children;
	while Node <> nil do
		if Node._type = XML_ELEMENT_NODE then
		begin
			Result := Node;
			Break;
		end
		else
			Node := Node.next;
end;

function xmlLastElementChild(Node: xmlNodePtr) : xmlNodePtr;
var
	Last: xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Node := Node.children;
	Last := nil;
	while Node <> nil do
		if Node._type = XML_ELEMENT_NODE then
		begin
			Last := Node;
			Node := Node.next;
		end
		else
			Node := Node.next;
	Result := Last;
end;

function xmlNextElementSibling(Node: xmlNodePtr) : xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Node := Node.next;
	while Node <> nil do
		if Node._type = XML_ELEMENT_NODE then
		begin
			Result := Node;
			Break;
		end
		else
			Node := Node.next;
end;

function xmlGetElementChild(Node: xmlNodePtr; Index: Integer): xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Node := xmlFirstElementChild(Node);
	if Node = nil then
		Exit;
	while Index > 0 do
	begin
		Dec(Index);
		Node := xmlNextElementSibling(Node);
		if Node = nil then
			Exit;
	end;
	Result := Node;
end;

function xmlGetElementChildCount(Node: xmlNodePtr): Integer;
begin
	Result := 0;
	if Node = nil then
		Exit;
	Node := xmlFirstElementChild(Node);
	if Node = nil then
		Exit;
	while Node <> nil do
	begin
		Inc(Result);
		Node := xmlNextElementSibling(Node);
	end;
end;

function xmlGetElementByName(Node: xmlNodePtr; const Name: AnsiString): xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Node := xmlFirstElementChild(Node);
	while Node <> nil do
		if PCharToStr(Node.name) = Name then
		begin
			Result := Node;
			Break;
		end
		else
			Node := xmlNextElementSibling(Node);
end;

function xmlFirstAttributeChild(Node: xmlNodePtr) : xmlAttrPtr;
var
	Attr: xmlAttrPtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Attr := Node.properties;
	while Attr <> nil do
		if Attr._type = XML_ATTRIBUTE_NODE then
		begin
			Result := Attr;
			Break;
		end
		else
			Attr := Attr.next;
end;

function xmlNextAttributeSibling(Attr: xmlAttrPtr) : xmlAttrPtr;
begin
	Result := nil;
	if Attr = nil then
		Exit;
	if Attr._type = XML_ATTRIBUTE_NODE then
		Attr := Attr.next
	else
		Exit;
	while Attr <> nil do
		if Attr._type = XML_ATTRIBUTE_NODE then
		begin
			Result := Attr;
			Break;
		end
		else
			Attr := Attr.next;
end;

function xmlGetAttributeChild(Node: xmlNodePtr; Index: Integer): xmlAttrPtr;
var
	Attr: xmlAttrPtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Attr := xmlFirstAttributeChild(Node);
	if Attr = nil then
		Exit;
	while Index > 0 do
	begin
		Dec(Index);
		Attr := xmlNextAttributeSibling(Attr);
		if Attr = nil then
			Exit;
	end;
	Result := Attr;
end;

function xmlGetAttributeChildCount(Node: xmlNodePtr): Integer;
var
	Attr: xmlAttrPtr;
begin
	Result := 0;
	if Node = nil then
		Exit;
	Attr := xmlFirstAttributeChild(Node);
	if Attr = nil then
		Exit;
	while Attr <> nil do
	begin
		Inc(Result);
		Attr := xmlNextAttributeSibling(Attr);
	end;
end;

function xmlGetAttributeByName(Node: xmlNodePtr; const Name: AnsiString): xmlAttrPtr;
var
	Attr: xmlAttrPtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Attr := xmlFirstAttributeChild(Node);
	while Attr <> nil do
		if PCharToStr(Attr.name) = Name then
		begin
			Result := Attr;
			Break;
		end
		else
			Attr := xmlNextAttributeSibling(Attr);
end;

function xmlGetAttributeByIndex(Node: xmlNodePtr; Index: Integer): xmlAttrPtr;
var
	Attr: xmlAttrPtr;
begin
	Result := nil;
	if (Node = nil) or (Index < 0) then
		Exit;
	Attr := xmlFirstAttributeChild(Node);
	while Attr <> nil do
	begin
		if Index = 0 then
		begin
			Result := Attr;
			Break;
		end;
		Dec(Index);
		Attr := xmlNextAttributeSibling(Attr);
	end;
end;

function xmlFirstTextChild(Node: xmlNodePtr) : xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	Node := Node.children;
	while Node <> nil do
		if Node._type = XML_TEXT_NODE then
		begin
			Result := Node;
			Break;
		end
		else
			Node := Node.next;
end;

function xmlNextTextSibling(Node: xmlNodePtr) : xmlNodePtr;
begin
	Result := nil;
	if Node = nil then
		Exit;
	if Node._type = XML_TEXT_NODE then
		Node := Node.next
	else
		Exit;
	while Node <> nil do
		if Node._type = XML_TEXT_NODE then
		begin
			Result := Node;
			Break;
		end
		else
			Node := Node.next;
end;

procedure xmlFormatDocument(Node: xmlNodePtr; Prefix: AnsiString);
var
	N, C: xmlNodePtr;
	S: AnsiString;
begin
	if Node = nil then
		Exit;
	S := Prefix + #9;
	N := xmlFirstElementChild(Node);
	while N <> nil do
	begin
		C := xmlNewText(PAnsiChar(S));
		xmlAddPrevSibling(N, C);
		xmlFormatDocument(N, S);
		N := xmlNextElementSibling(N);
	end;
	N := xmlLastElementChild(Node);
	if N <> nil then
	begin
		C := xmlNewText(PAnsiChar(Prefix));
		xmlAddChild(Node, C);
	end;
end;
{$endregion}

type
  TDocument = class;
  TNodeList = class;

{ TNode }

  TNode = class(TInterfacedImpl, INode)
  private
    FNode: xmlNodePtr;
		function Execute(XPath: PAnsiChar): TXPathResult;
  protected
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
  public
    constructor Create(Node: xmlNodePtr);
  end;

{ TNodeList }

  TNodeList = class(TInterfacedImpl, INodeList)
  private
    FNode: xmlNodePtr;
    FNodeType: xmlElementType;
  protected
    procedure Clear; stdcall;
    procedure Add(Node: INode); overload; stdcall;
    procedure Add(Name: PAnsiChar; out Node: INode); overload; stdcall;
    procedure Remove(Node: INode); overload; stdcall;
    procedure Remove(Name: PAnsiChar); overload; stdcall;
    procedure Item(Index: Integer; out Node: INode); overload; stdcall;
    function Item(Name: PAnsiChar; out Node: INode): Boolean; overload; stdcall;
    function Length: Integer; stdcall;
  public
    constructor Create(Node: xmlNodePtr; NodeType: xmlElementType);
  end;

{ TXPathList }

  TXPathList = class(TInterfacedImpl, INodeList)
  private
    FXPathResult: TXPathResult;
  protected
    procedure Clear; stdcall;
    procedure Add(Node: INode); overload; stdcall;
    procedure Add(Name: PAnsiChar; out Node: INode); overload; stdcall;
    procedure Remove(Node: INode); overload; stdcall;
    procedure Remove(Name: PAnsiChar); overload; stdcall;
    procedure Item(Index: Integer; out Node: INode); overload; stdcall;
    function Item(Name: PAnsiChar; out Node: INode): Boolean; overload; stdcall;
    function Length: Integer; stdcall;
  public
    constructor Create(const XPathResult: TXPathResult);
  end;

{ TDocument }

  TDocument = class(TNode, IDocument)
  private
    function GetDocument: xmlDocPtr;
		procedure SetDocument(Value: xmlDocPtr);
    property Document: xmlDocPtr read GetDocument write SetDocument;
  protected
    procedure CreateAttribute(const Name: PAnsiChar; out Node: INode); stdcall;
    procedure CreateElement(const Name: PAnsiChar; out Node: INode); stdcall;
    procedure SetRoot(Node: INode); stdcall;
    function GetRoot(out Node: INode): Boolean; stdcall;
    procedure Load(FileName: PAnsiChar); stdcall;
    procedure Save(FileName: PAnsiChar); stdcall;
  public
    constructor Create(Doc: xmlDocPtr);
    destructor Destroy; override;
  end;

{ TNode }

constructor TNode.Create(Node: xmlNodePtr);
begin
  inherited Create;
  FNode := Node;
end;

function TNode.Instance: Pointer;
begin
  Result := Self;
end;

function TNode.GetDocument(out Document: IDocument): Boolean;
var
  D: xmlDocPtr;
begin
	D := FNode.doc;
  Result := D <> nil;
  if Result then
    Document := TDocument.Create(D)
  else
    Document := nil;
end;

function TNode.GetParent(out Node: INode): Boolean;
var
  N: xmlNodePtr;
begin
  N := FNode.parent;
  Result := N <> nil;
  if Result then
    Node := TNode.Create(N)
  else
    Node := nil;
end;

function TNode.Execute(XPath: PAnsiChar): TXPathResult;
var
	Context: xmlXPathContextPtr;
	PathObject: xmlXPathObjectPtr;
begin
	Context := xmlXPathNewContext(FNode.doc);
	Context.node := FNode;
	PathObject := xmlXPathEval(XPath, Context);
	Result := xmlConvertXPath(PathObject);
	xmlXPathFreeObject(PathObject);
	xmlXPathFreeContext(Context);
end;

function TNode.SelectNode(XPath: PAnsiChar; out Node: INode): Boolean;
var
	R: TXPathResult;
begin
	R := Execute(XPath);
	Result := Length(R) > 0;
	if Result then
		Node := TNode.Create(R[0])
	else
		Node := nil;
end;

function TNode.SelectList(XPath: PAnsiChar; out List: INodeList): Boolean;
var
	R: TXPathResult;
begin
	R := Execute(XPath);
	Result := Length(R) > 0;
	if Result then
		List := TXPathList.Create(R)
	else
		List := nil;
end;

procedure TNode.Attributes(out List: INodeList);
begin
  List := TNodeList.Create(FNode, XML_ATTRIBUTE_NODE);
end;

procedure TNode.Nodes(out List: INodeList);
begin
  List := TNodeList.Create(FNode, XML_ELEMENT_NODE);
end;

function TNode.GetKind: Integer;
begin
  Result := Integer(FNode._type);
end;

function TNode.GetName: PAnsiChar;
begin
  case FNode._type of
    XML_ELEMENT_NODE:
      Return(Result, FNode.name);
    XML_ATTRIBUTE_NODE:
      Return(Result, FNode.name);
    XML_DOCUMENT_NODE:
      Return(Result, 'DOCUMENT');
  else
    Return(Result, FNode.name);
  end;
end;

function TNode.GetText: PAnsiChar;
var
	B: PAnsiChar;
	S: string;
begin
	B := xmlNodeGetContent(FNode);
	S := PCharToStr(B);
	xmlFree(B);
	Return(Result, S);
end;

procedure TNode.SetText(Value: PAnsiChar);
var
	B: PAnsiChar;
begin
  case FNode._type of
    XML_ELEMENT_NODE,
    XML_ATTRIBUTE_NODE:
      begin
				B := xmlEncodeSpecialChars(FNode.doc, Value);
				xmlNodeSetContent(FNode, B);
				xmlFree(B);
      end;
    XML_DOCUMENT_NODE:
      begin
				//  Do nothing
      end;
  end;
end;

function TNode.GetXml: PAnsiChar;
var
	B: xmlOutputBufferPtr;
	S: AnsiString;
begin
	B := xmlAllocOutputBuffer(nil);
	xmlNodeDump(B.buffer, FNode.doc, FNode, 0, 1);
	S := B.buffer.content;
	xmlOutputBufferFlush(B);
	xmlOutputBufferClose(B);
	Return(Result, S);
end;

procedure TNode.SetXml(Value: PAnsiChar);
begin
  case FNode._type of
    XML_ELEMENT_NODE,
    XML_ATTRIBUTE_NODE:
      begin
				// Do nothing
      end;			
    XML_DOCUMENT_NODE:
			begin
				FNode := xmlNodePtr(xmlParseDoc(Value));
				if FNode = nil then
					FNode := xmlNodePtr(xmlNewDoc('1.0'));
			end;
  end;
end;

{ TNodeList }

constructor TNodeList.Create(Node: xmlNodePtr; NodeType: xmlElementType);
begin
  inherited Create;
  FNode := Node;
  FNodeType := NodeType;
end;

procedure TNodeList.Clear;
var
	N: xmlNodePtr;
begin
	case FNodeType of
		XML_ELEMENT_NODE:
			while FNode.children <> nil do
			begin
				N := FNode.children;
				xmlUnlinkNode(N);
				xmlFreeNode(N);
			end;
		XML_ATTRIBUTE_NODE:
			while FNode.properties <> nil do
				xmlRemoveProp(FNode.properties);
	end;
end;

procedure TNodeList.Add(Node: INode);
var
	N: xmlNodePtr;
begin
	N := TNode(Node.Instance).FNode;
	xmlUnlinkNode(N);
	xmlAddChild(FNode, N);
end;

procedure TNodeList.Add(Name: PAnsiChar; out Node: INode);
var
	N: xmlNodePtr;
	A: xmlAttrPtr;
begin
	case FNodeType of
		XML_ELEMENT_NODE:
			begin
				N := xmlNewNode(nil, Name);
				N := xmlAddChild(FNode, N);
				Node := TNode.Create(N);
			end;
		XML_ATTRIBUTE_NODE:
			begin
				A := xmlGetAttributeByName(FNode, Name);
				if A = nil then
					A := xmlNewProp(FNode, PAnsiChar(Name), ' ');
				Node := TNode.Create(xmlNodePtr(A));
			end;
	else
		Node := nil;
	end;
end;

procedure TNodeList.Remove(Node: INode);
var
	N: xmlNodePtr;
begin
	N := TNode(Node.Instance).FNode;
	xmlUnlinkNode(N);
	xmlFreeNode(N);
end;

procedure TNodeList.Remove(Name: PAnsiChar);
var
	N: xmlNodePtr;
	A: xmlAttrPtr;
begin
	case FNodeType of
		XML_ELEMENT_NODE:
			begin
				N := xmlGetElementByName(FNode, Name);
				if N <> nil then
				begin
					xmlUnlinkNode(N);
					xmlFreeNode(N);
				end;
			end;
		XML_ATTRIBUTE_NODE:
			begin
				A := xmlGetAttributeByName(FNode, Name);
				if A <> nil then
					xmlRemoveProp(A);
			end;
	end;
end;

procedure TNodeList.Item(Index: Integer; out Node: INode);
var
	N: xmlNodePtr;
	A: xmlAttrPtr;
begin
	Node := nil;
	case FNodeType of
		XML_ELEMENT_NODE:
			begin
				N := xmlGetElementChild(FNode, Index);				
				if N <> nil then
					Node := TNode.Create(N);
			end;
		XML_ATTRIBUTE_NODE:
			begin
				A := xmlGetAttributeChild(FNode, Index);
				if (A <> nil) then
					Node := TNode.Create(N);
			end;
	end;
end;

function TNodeList.Item(Name: PAnsiChar; out Node: INode): Boolean;
var
	N: xmlNodePtr;
	A: xmlAttrPtr;
begin
	Result := False;
	Node := nil;
	case FNodeType of
		XML_ELEMENT_NODE:
			begin
				N := xmlGetElementByName(FNode, Name);
				Result :=  N <> nil;
				if Result then
					Node := TNode.Create(N);
			end;
		XML_ATTRIBUTE_NODE:
			begin
				A := xmlGetAttributeByName(FNode, Name);
				Result :=  A <> nil;
				if Result then
					Node := TNode.Create(xmlNodePtr(A));
			end;
	end;
end;

function TNodeList.Length: Integer;
begin
	Result := 0;
	case FNodeType of
		XML_ELEMENT_NODE:
			Result := xmlGetElementChildCount(FNode);
		XML_ATTRIBUTE_NODE:
			Result := xmlGetAttributeChildCount(FNode);
	end;
end;

{ TXPathList }

constructor TXPathList.Create(const XPathResult: TXPathResult);
begin
  inherited Create;
  FXPathResult := XPathResult;
end;

procedure TXPathList.Clear;
begin
end;

procedure TXPathList.Add(Node: INode);
begin
end;

procedure TXPathList.Add(Name: PAnsiChar; out Node: INode);
begin
	Node := nil;
end;

procedure TXPathList.Remove(Node: INode);
begin
end;

procedure TXPathList.Remove(Name: PAnsiChar);
begin
end;

procedure TXPathList.Item(Index: Integer; out Node: INode);
begin
	Node := nil;
	if (Index < 0) or (Index >= Length) then
		Exit;
	Node := TNode.Create(FXPathResult[Index]);
end;

function TXPathList.Item(Name: PAnsiChar; out Node: INode): Boolean;
var
	N: xmlNodePtr;
	S: AnsiString;
	I: Integer;
begin
	Result := False;
	Node := nil;
	S := PCharToStr(Name);
	for I := 0 to Length - 1 do
	begin
		N := xmlNodePtr(FXPathResult[I]);
		if PCharToStr(N.name) = S then
		begin
			Node := TNode.Create(N);
			Break;
		end;
	end;
end;

function TXPathList.Length: Integer;
begin
	Result := System.Length(FXPathResult);
end;

{ TDocument }

constructor TDocument.Create(Doc: xmlDocPtr);
begin
  inherited Create(xmlNodePtr(Doc));
  if FNode = nil then
		FNode := xmlNodePtr(xmlNewDoc('1.0'));
end;

destructor TDocument.Destroy;
begin
	xmlFreeDoc(xmlDocPtr(FNode));
	inherited Destroy;
end;

function TDocument.GetDocument: xmlDocPtr;
begin
	if FNode = nil then
		FNode := xmlNodePtr(xmlNewDoc('1.0'));
	Result := xmlDocPtr(FNode);
end;

procedure TDocument.SetDocument(Value: xmlDocPtr);
const
	Version = '1.0';
begin
	xmlFreeDoc(xmlDocPtr(FNode));
	FNode := xmlNodePtr(Value);
	if FNode = nil then
		FNode := xmlNodePtr(xmlNewDoc('1.0'));
end;

procedure TDocument.CreateAttribute(const Name: PAnsiChar; out Node: INode);
begin
	Node := TNode.Create(xmlNodePtr(xmlNewDocProp(Document, Name, ' ')));
end;

procedure TDocument.CreateElement(const Name: PAnsiChar; out Node: INode);
begin
	Node := TNode.Create(xmlNewNode(nil, Name));
end;

procedure TDocument.SetRoot(Node: INode);
begin
	if Node = nil then
		SetDocument(nil)
	else
		xmlDocSetRootElement(Document, TNode(Node.Instance).FNode);
end;

function TDocument.GetRoot(out Node: INode): Boolean;
var
	N: xmlNodePtr;
begin
	N := xmlDocGetRootElement(Document);
	Result := N <> nil; 
	if Result then
		Node := TNode.Create(N)
	else
		Node := nil;
end;

procedure TDocument.Load(FileName: PAnsiChar);
begin
	Document := xmlParseFile(FileName);
end;

procedure TDocument.Save(FileName: PAnsiChar);
begin
	xmlSaveFile(FileName, Document);
end;

procedure XmlDocumentCreate(out Document: IDocument);
begin
	Document := TDocument.Create(nil);
end;

