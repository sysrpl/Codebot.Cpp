uses
  MSXMLParser;

type
  TDocument = class;
  TNodeList = class;

{ TNode }

  TNode = class(TInterfacedObject, INode)
  private
    FNode: IXMLDOMNode;
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
  public
    constructor Create(Node: IXMLDOMNode);
  end;

{ TNodeList }

  TNodeList = class(TInterfacedObject, INodeList)
  private
    FNode: IXMLDOMNode;
    FList: IInterface;
  protected
    procedure Clear(); stdcall;
    procedure Add(Node: INode); overload; stdcall;
    procedure Add(Name: PAnsiChar; out Node: INode); overload; stdcall;
    procedure Remove(Node: INode); overload; stdcall;
    procedure Remove(Name: PAnsiChar); overload; stdcall;
    procedure Item(Index: Integer; out Node: INode); overload; stdcall;
    function Item(Name: PAnsiChar; out Node: INode): Boolean; overload; stdcall;
    function Length: Integer; stdcall;
  public
    constructor Create(Node: IXMLDOMNode; List: IInterface);
  end;

{ TDocument }

  TDocument = class(TNode, IDocument)
  private
    FDocument: IXMLDOMDocument;
  protected
    procedure CreateAttribute(const Name: PAnsiChar; out Node: INode); stdcall;
    procedure CreateElement(const Name: PAnsiChar; out Node: INode); stdcall;
    procedure SetRoot(Node: INode); stdcall;
    function GetRoot(out Node: INode): Boolean; stdcall;
    procedure Load(FileName: PAnsiChar); stdcall;
    procedure Save(FileName: PAnsiChar); stdcall;
  public
    constructor Create(Document: IXMLDOMDocument);
  end;

{ TNode }

constructor TNode.Create(Node: IXMLDOMNode);
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
  D: IXMLDOMDocument;
begin
  D := FNode.ownerDocument;
  Result := D <> nil;
  if Result then
    Document := TDocument.Create(D)
  else
    Document := nil;
end;

function TNode.GetParent(out Node: INode): Boolean;
var
  N: IXMLDOMNode;
begin
  N := FNode.parentNode;
  Result := N <> nil;
  if Result then
    Node := TNode.Create(N)
  else
    Node := nil;
end;

function TNode.SelectNode(XPath: PAnsiChar; out Node: INode): Boolean;
var
  N: IXMLDOMNode;
begin
  Node := nil;
  try
    N := FNode.selectSingleNode(StrPas(XPath));
    Result := N <> nil;
    if Result then
      Node := TNode.Create(N);
  except
    Result := False;
  end;
end;

function TNode.SelectList(XPath: PAnsiChar; out List: INodeList): Boolean;
var
  L: IXMLDOMNodeList;
begin
  List := nil;
  try
    L := FNode.selectNodes(StrPas(XPath));
    Result := L <> nil;
    if Result then
      List := TNodeList.Create(nil, L) ;
  except
    Result := False;
  end;
end;

procedure TNode.Attributes(out List: INodeList);
begin
  List := TNodeList.Create(FNode, FNode.attributes);
end;

procedure TNode.Nodes(out List: INodeList);
begin
  List := TNodeList.Create(FNode, FNode.childNodes);
end;

function TNode.GetKind: Integer;
begin
  Result := FNode.nodeType;
end;

function TNode.GetName: PAnsiChar;
begin
  case FNode.nodeType of
    NODE_ELEMENT:
      Return(Result, (FNode as IXMLDOMElement).tagName);
    NODE_ATTRIBUTE:
      Return(Result, (FNode as IXMLDOMAttribute).name);
    NODE_DOCUMENT:
      Return(Result, 'DOCUMENT');
  else
    Return(Result, FNode.nodeName);
  end;
end;

function TNode.GetText: PAnsiChar;
begin
  case FNode.nodeType of
    NODE_ELEMENT:
      Return(Result, (FNode as IXMLDOMElement).text);
    NODE_ATTRIBUTE:
      Return(Result, (FNode as IXMLDOMAttribute).value);
    NODE_TEXT:
      Return(Result, (FNode as IXMLDOMText).data);
    NODE_DOCUMENT:
      Return(Result, (FNode as IXMLDOMDocument).xml);
  else
    Return(Result, FNode.nodeValue);
  end;
end;

procedure TNode.SetText(Value: PAnsiChar);
var
  S: AnsiStrng;
begin
  S := StrPas(Value);
  case FNode.nodeType of
    NODE_ELEMENT:
      (FNode as IXMLDOMElement).text := S;
    NODE_ATTRIBUTE:
      (FNode as IXMLDOMAttribute).value := S;
    NODE_TEXT:
      (FNode as IXMLDOMText).data := S;
    NODE_DOCUMENT:
      (FNode as IXMLDOMDocument).loadXML(S);
  else
    FNode.nodeValue := S;
  end;
end;

{ TNodeList }

constructor TNodeList.Create(Node: IXMLDOMNode; List: IInterface);
begin
  inherited Create;
  FNode := Node;
  FList := List;
end;

procedure TNodeList.Clear;
var
  L: IXMLDOMNamedNodeMap;
  I: Integer;
begin
  if FNode = nil then
    Exit;
  L := FList as IXMLDOMNamedNodeMap;
  if L <> nil then
    for I := L.length - 1 downto 0 do
      L.removeNamedItem(L.item[I].nodeName)
  else
    while FNode.firstChild <> nil do
      FNode.removeChild(FNode.firstChild);
end;

procedure TNodeList.Add(Node: INode);
var
  N: IXMLDOMNode;
begin
  if FNode = nil then
    Exit;
  N := TNode(Node.Instance).FNode;
  if Supports(N, IXMLDOMElement) then
    FNode.appendChild(N)
  else if Supports(N, IXMLDOMAttribute) then
    FNode.attributes.setNamedItem(N);
end;

procedure TNodeList.Add(Name: PAnsiChar; out Node: INode);
var
  D: IXMLDOMDocument;
  N: IXMLDOMNode;
  S: AnsiStrng;
begin
  Node := nil;
  if FNode = nil then
    Exit;
  D := FNode.ownerDocument;
  if D = nil then
    Exit;
  S := StrPas(Name);
  if Supports(FList, IXMLDOMNodeList) then
  begin
    N := D.createElement(S);
    FNode.appendChild(N);
  end
  else
  begin
    N := D.createAttribute(S);
    (FList as IXMLDOMNamedNodeMap).setNamedItem(N);
  end;
  Node := TNode.Create(N);
end;

procedure TNodeList.Remove(Node: INode);
begin
  if FNode <> nil then
    FNode.removeChild(TNode(Node.Instance).FNode);
end;

procedure TNodeList.Remove(Name: PAnsiChar);
var
  Node: INode;
begin
  if FNode = nil then
    Exit;
  Item(Name, Node);
  if Node <> nil then
    FNode.removeChild(TNode(Node.Instance).FNode);
end;

procedure TNodeList.Item(Index: Integer; out Node: INode);
begin
  if Supports(FList, IXMLDOMNamedNodeMap) then
    Node := TNode.Create((FList as IXMLDOMNamedNodeMap).item[Index])
  else
    Node := TNode.Create((FList as IXMLDOMNodeList).item[Index]);
end;

function TNodeList.Item(Name: PAnsiChar; out Node: INode): Boolean;
var
  N: IXMLDOMNode;
  C: IXMLDOMElement;
  L: IXMLDOMNodeList;
  I: Integer;
  S: AnsiStrng;
begin
  Node := nil;
  N := nil;
  S := StrPas(Name);
  if Supports(FList, IXMLDOMNamedNodeMap) then
    (FList as IXMLDOMNamedNodeMap).getNamedItem(S)
  else
  begin
    L := FList as IXMLDOMNodeList;
    for I := 0 to L.length - 1 do
    begin
      C := L.item[I] as IXMLDOMElement;
      if C = nil then
        Continue;
      if C.tagName = S then
      begin
        N := C;
        Break;
      end;
    end;
  end;
  Result := N <> nil;
  if Result then
    Node := TNode.Create(N)
  else
    Node:= nil;    
end;

function TNodeList.Length: Integer;
begin
  if Supports(FList, IXMLDOMNamedNodeMap) then
    Result := (FList as IXMLDOMNamedNodeMap).length
  else
    Result := (FList as IXMLDOMNodeList).length;
end;

{ TDocument }

constructor TDocument.Create(Document: IXMLDOMDocument);
begin
  inherited Create(Document);
  FDocument := Document;
end;

procedure TDocument.CreateAttribute(const Name: PAnsiChar; out Node: INode);
begin
  Node := TNode.Create(FDocument.createAttribute(Name));
end;

procedure TDocument.CreateElement(const Name: PAnsiChar; out Node: INode);
begin
  Node := TNode.Create(FDocument.createElement(Name));
end;

procedure TDocument.SetRoot(Node: INode);
var
  N: IXMLDOMElement;
begin
  N := TNode(Node.Instance).FNode as IXMLDOMElement;
  if N <> nil then
    FDocument.documentElement := N;
end;

function TDocument.GetRoot(out Node: INode): Boolean;
var
  N: IXMLDOMElement;
begin
  N := FDocument.documentElement;
  Result := N <> nil;
  if Result then
    Node := TNode.Create(N)
  else
    Node := nil;
end;

procedure TDocument.Load(FileName: PAnsiChar);
begin
  FDocument.load(StrPas(FileName));
end;

procedure TDocument.Save(FileName: PAnsiChar);
begin
  FDocument.save(StrPas(FileName));
end;

{ DocumentCreate }

threadvar
  CoInitialized: Boolean;

procedure XmlDocumentCreate(out Document: IDocument);
begin
  if not CoInitialized then
  begin
    CoInitialize(nil);
    CoInitialized := True;
  end;
  Document := TDocument.Create(CreateOleObject('Msxml2.DOMDocument') as IXMLDOMDocument);
end;
