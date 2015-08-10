#include <Codebot/Text/Xml/Document.h>
#include <Codebot/Text/Xml/XmlExceptions.h>
#include <Codebot/Interop/CoreInterop.h>
#include <Codebot/Memory/Buffer.h>
#include <Codebot/VectorEnumerator.h>

namespace Codebot
{
namespace Text
{
namespace Xml
{

using namespace Interop;
using namespace Memory;
using namespace Codebot::Time;

Boolean IsNumerical(const Char& c)
{
	return CharInSet(c, '0', '9') || (c == '-');
}

Boolean IsAlphaNumerical(const Char& c)
{
	return IsAlpha(c) || IsNumerical(c);
}

Boolean IsAttribute(const String& value)
{
	auto length = value.Length();
	if (length < 2)
		return false;
	if (value[0] != '@')
		return false;
	String s = value.ToUpper();
	while (true)
	{
		auto c = s[--length];
		if (length == 1)
			return IsAlpha(c);
		else if (!IsAlphaNumerical(c))
			return false;
	}
	return false;
}

Boolean IsIdentifier(const String& value)
{
	auto length = value.Length();
	if (length == 0)
		return false;
	String s = value.ToUpper();
	while (true)
	{
		auto c = s[--length];
		if (length == 0)
			return IsAlpha(c);
		else if (!IsAlphaNumerical(c))
			return false;
	}
	return false;
}

void CheckIdentifier(const String& method, const String& value)
{
	if (!IsIdentifier(value))
		ThrowXmlNameException(method, value);
}

void CheckItems(const String& method, const String& value, Array<String>& items)
{
	items = value.Split("/");
	auto length = items.Length();
	if (length == 0)
		ThrowXmlNameException(method, value);
	while (length != 0)
		CheckIdentifier(method, items[--length]);
}

void CheckFilerItems(const String& method, const String& value, Array<String>& items)
{
	items = value.Split("/");
	auto length = items.Length();
	if (length == 0)
		ThrowXmlNameException(method, value);
	for (Cardinal i = 0; i < length - 1; i++)
		CheckIdentifier(method, items[i]);
	String s = items[length - 1];
	if (IsIdentifier(s) || IsAttribute(s))
		return;
	ThrowXmlNameException(method, s);
}

// Filer

Filer::Filer(Interop::INode* n)
	: node(n)
{
}

String Filer::Read(const String& path, const String& value, Boolean stored)
{
	Interface<INode> n;
	if (node->SelectNode(path, Out(n)))
		return n->GetText();
	if (stored)
		Write(path, value);
	return value;
}

void Filer::Write(const String& path, const String& value)
{
	Interface<INode> n;
	if (node->SelectNode(path, Out(n)))
	{
		n->SetText(value);
		return;
	}
	Array<String> items;
	CheckFilerItems(ThisMethod, path, items);
	Node current = Node(node);
	auto length = items.Length();
	String s = items[length - 1];
	if (length > 1)
	{
		items.Length(length - 1);
		current = current.Force(StrJoin(items, "/"));
	}
	Interface<IDocument> d;
	current.node->GetDocument(Out(d));
	if (IsAttribute(s))
		d->CreateAttribute(s.SubString(1, s.Length() - 1), Out(n));
	else
		d->CreateElement(s, Out(n));
	n->SetText(value);
	Interface<INodeList> l;
	current.node->Nodes(Out(l));
	l->AddNode(n);
}

Boolean Filer::ReadData(const String& path, ISerializable* data, const String& format)
{
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	String s = Read(path);
	return data->FromFormat(s, format);
}

void Filer::WriteData(const String& path, ISerializable* data, const String& format)
{
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	Write(path, data->ToFormat(format));
}

String Filer::ReadStr(const String& path, const String& value, Boolean stored)
{
	return Read(path, value, stored);
}

void Filer::WriteStr(const String& path, const String& value)
{
	return Write(path, value);
}

Integer Filer::ReadInt(const String& path, const Integer& value, Boolean stored)
{
	String s = Read(path, AnyToStr(value), stored);
	auto r = value;
	IntegerParse(s, r);
	return r;
}

void Filer::WriteInt(const String& path, const Integer& value)
{
	Write(path, AnyToStr(value));
}

Double Filer::ReadFloat(const String& path, const Double& value, Boolean stored)
{
	String s = Read(path, AnyToStr(value), stored);
	auto r = value;
	DoubleParse(s, r);
	return r;
}

void Filer::WriteFloat(const String& path, const Double& value)
{
	Write(path, AnyToStr(value));
}

TimeSpan Filer::ReadTime(const String& path, const TimeSpan& value, Boolean stored)
{
	String s = Read(path, value.ToString(), stored);
	auto r = value;
	return r.FromFormat(s, String::Empty) ? r : value;
}

void Filer::WriteTime(const String& path, const TimeSpan& value)
{
	Write(path, value.ToString());
}

DateTime Filer::ReadDate(const String& path, const DateTime& value, Boolean stored)
{
	String s = Read(path, value.ToString(), stored);
	auto r = value;
	return r.FromFormat(s, String::Empty) ? r : value;
}

void Filer::WriteDate(const String& path, const DateTime& value)
{
	Write(path, value.ToString());
}

// Node

Node::Node(INode* n)
	: node(n)
{
}

Node::operator String () const
{
	return node->GetText();
}

Node& Node::operator = (const String& s)
{
	node->SetText(s);
	return *this;
}

Nullable<Node> Node::Parent() const
{
	Interface<INode> n;
	if (node->GetParent(Out(n)))
		return Node(n);
	else
		return null;
}

Nullable<Document> Node::Owner() const
{
	Interface<IDocument> d;
	if (node->GetDocument(Out(d)))
		return Document(d);
	else
		return null;
}

AttributeList Node::Attributes() const
{
	Interface<INodeList> a;
	node->Attributes(Out(a));
	return AttributeList(a);
}

NodeList Node::Nodes() const
{
	Interface<INodeList> n;
	node->Nodes(Out(n));
	return NodeList(n);
}

Node Node::Force(const String& path)
{
	Array<String> items;
	CheckItems(ThisMethod, path, items);
	String item = items[0];
	auto parent = node;
	auto child = parent;
	auto length = items.Length();
	for (Cardinal i = 0; i < length; i++)
	{
		item = items[i];
		if (parent->SelectNode(item, Out(child)))
			parent = child;
		else
		{
			Interface<INodeList> nodes;
			parent->Nodes(Out(nodes));
			nodes->AddName(item, Out(child));
			parent = child;
		}
	}
	return Node(parent);
}

Nullable<Node> Node::SelectNode(const String& path) const
{
	Interface<INode> n;
	if (node->SelectNode(path, Out(n)))
		return Node(n);
	else
		return null;
}

Nullable<NodeList> Node::SelectList(const String& path) const
{
	Interface<INodeList> n;
	if (node->SelectList(path, Out(n)))
		return NodeList(n);
	else
		return null;
}

String Node::Path() const
{
	String s = "/" + Name();
	Nullable<Node> a = Parent();
	Nullable<Node> b;
	while (a.HasValue())
	{
		b = a.Value().Parent();
		if (!b.HasValue())
			break;
		s = "/" + a.Value().Name() + s;
		a = b;
	}
	return s;
}

String Node::Name() const
{
	return node->GetName();
}

String Node::Text() const
{
	return node->GetText();
}

void Node::Text(const String& value)
{
	node->SetText(value);
}

String Node::Xml() const
{
	return node->GetXml();
}

void Node::Xml(const String& value)
{
	node->SetXml(value);
}

String Node::ToFormat(const String& format) const
{
	return node->GetText();
}

Filer Node::Filer() const
{
	return Xml::Filer(node);
}

// NodeList

NodeList::NodeList(INodeList* n)
	: nodes(n)
{
}

void NodeList::Clear()
{
	nodes->Clear();
}

void NodeList::Add(const Node& node)
{
	nodes->AddNode(node.node);
}

Node NodeList::Add(const String& name)
{
	CheckIdentifier(ThisMethod, name);
	Interface<INode> n;
	nodes->AddName(name, Out(n));
	return Node(n);
}

void NodeList::Remove(const Node& node)
{
	nodes->RemoveNode(node.node);
}

void NodeList::Remove(const String& name)
{
	nodes->RemoveName(name);
}

Nullable<Node> NodeList::operator [] (const String& name) const
{
	CheckIdentifier(ThisMethod, name);
	Interface<INode> n;
	if (nodes->Named(name, Out(n)))
		return Node(n);
	else
		return null;
}

Node NodeList::operator [] (Integer index) const
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	if (index >= nodes->Length())
		ThrowRangeException(ThisMethod);
	Interface<INode> n;
	nodes->Item(index, Out(n));
	return Node(n);
}

Integer NodeList::Length() const
{
	return nodes->Length();
}

// NodeList::IEnumerable<Node>

IEnumerator<Node>* NodeList::GetEnumerator() const
{
	std::vector<Node> v;
	Integer length = nodes->Length();
	v.reserve(length);
	for (Integer i = 0; i < length; i++)
	{
		Interface<INode> n;
		nodes->Item(i, Out(n));
		v.push_back(Node(n));
	}
	return new VectorEnumerator<Node>(v);
}

// Attribute

Attribute::Attribute(INode* a)
	: attribute(a)
{
}

Attribute::operator String () const
{
	return attribute->GetText();
}

Attribute& Attribute::operator = (const String& s)
{
	attribute->SetText(s);
	return *this;
}

Nullable<Node> Attribute::Parent() const
{
	Interface<INode> n;
	if (attribute->GetParent(Out(n)))
		return Node(n);
	else
		return null;
}

Nullable<Document> Attribute::Owner() const
{
	Interface<IDocument> d;
	if (attribute->GetDocument(Out(d)))
		return Document(d);
	else
		return null;
}

String Attribute::Name() const
{
	return attribute->GetName();
}

String Attribute::Path() const
{
	String s = "/@" + Name();
	Nullable<Node> a = Parent();
	if (a.HasValue())
		return a.Value().Path() + s;
	else
		return s;
}

String Attribute::Text() const
{
	return attribute->GetText();
}

void Attribute::Text(const String& value)
{
	attribute->SetText(value);
}

String Attribute::Xml() const
{
	return attribute->GetXml();
}

void Attribute::Xml(const String& value)
{
	attribute->SetXml(value);
}

String Attribute::ToFormat(const String& format) const
{
	return attribute->GetText();
}

// AttributeList

AttributeList::AttributeList(INodeList* a)
	: attributes(a)
{
}

void AttributeList::Clear()
{
	attributes->Clear();
}

void AttributeList::Add(const Attribute& attribute)
{
	attributes->AddNode(attribute.attribute);
}

Attribute AttributeList::Add(const String& name)
{
	CheckIdentifier(ThisMethod, name);
	Interface<INode> n;
	attributes->AddName(name, Out(n));
	return Attribute(n);
}

void AttributeList::Remove(const Attribute& attribute)
{
	attributes->RemoveNode(attribute.attribute);
}

void AttributeList::Remove(const String& name)
{
	CheckIdentifier(ThisMethod, name);
	attributes->RemoveName(name);
}

Nullable<Attribute> AttributeList::operator [] (const String& name) const
{
	CheckIdentifier(ThisMethod, name);
	Interface<INode> a;
	if (attributes->Named(name, Out(a)))
		return Attribute(a);
	else
		return null;
}

Attribute AttributeList::operator [] (Integer index) const
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	if (index >= attributes->Length())
		ThrowRangeException(ThisMethod);
	Interface<INode> n;
	attributes->Item(index, Out(n));
	return Attribute(n);
}

Integer AttributeList::Length() const
{
	return attributes->Length();
}

// AttributeList::IEnumerable<Attribute>

IEnumerator<Attribute>* AttributeList::GetEnumerator() const
{
	std::vector<Attribute> v;
	Integer length = attributes->Length();
	v.reserve(length);
	for (Integer i = 0; i < length; i++)
		v.push_back((*this)[i]);
	return new VectorEnumerator<Attribute>(v);
}

// Document

Document::Document(IDocument* d)
	: document(d)
{
}

Document::Document()
{
	XmlDocumentCreate(Out(document));
}

Document::Document(const String& s)
{
	XmlDocumentCreate(Out(document));
	document->SetXml(s);
}

Document::operator String () const
{
	return document->GetXml();
}

String Document::operator ()() const
{
	return document->GetXml();
}

Document& Document::operator ()(const String& s)
{
	document->SetXml(s);
	return *this;
}

Document& Document::operator = (const String& s)
{
	document->SetXml(s);
	return *this;
}

void Document::Beautify()
{
	document->Beautify();
}

Attribute Document::CreateAttribute(const String& name) const
{
	CheckIdentifier(ThisMethod, name);
	Interface<INode> n;
	document->CreateAttribute(name, Out(n));
	return Attribute(n);
}

Node Document::CreateNode(const String& name) const
{
	CheckIdentifier(ThisMethod, name);
	Interface<INode> n;
	document->CreateElement(name, Out(n));
	return Node(n);
}

Node Document::Force(const String& path)
{
	Array<String> items;
	CheckItems(ThisMethod, path, items);
	String item = items[0];
	Interface<INode> parent;
	if (document->GetRoot(Out(parent)) && parent->GetName() == item)
	{
		// do nothing
	}
	else
	{
		document->CreateElement(item, Out(parent));
		document->SetRoot(parent);
	}
	auto child = parent;
	auto length = items.Length();
	for (Cardinal i = 1; i < length; i++)
	{
		item = items[i];
		if (parent->SelectNode(item, Out(child)))
			parent = child;
		else
		{
			Interface<INodeList> nodes;
			parent->Nodes(Out(nodes));
			nodes->AddName(item, Out(child));
			parent = child;
		}
	}
	return Node(parent);
}

Nullable<Node> Document::SelectNode(const String& path) const
{
	Interface<INode> n;
	if (document->SelectNode(path, Out(n)))
		return Node(n);
	else
		return null;
}

Nullable<NodeList> Document::SelectList(const String& path) const
{
	Interface<INodeList> n;
	if (document->SelectList(path, Out(n)))
		return NodeList(n);
	else
		return null;
}

void Document::Load(const String& fileName)
{
	document->Load(fileName);
}

void Document::Save(const String& fileName) const
{
	document->Save(fileName);
}

Nullable<Node> Document::GetRoot() const
{
	Interface<INode> n;
	if (document->GetRoot(Out(n)))
		return Node(n);
	else
		return null;
}

void Document::SetRoot(const Node& node)
{
	document->SetRoot(node.node);
}

Node Document::DocumentNode() const
{
	return Node(QueryAs<INode>(document));
}

String Document::Text() const
{
	return document->GetText();
}

void Document::Text(const String& value)
{
	document->SetText(value);
}

String Document::Xml() const
{
	return document->GetXml();
}

void Document::Xml(const String& value)
{
	document->SetXml(value);
}

String Document::ToFormat(const String& format) const
{
	return document->GetXml();
}

}
}
}
