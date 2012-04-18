#pragma once
#include <Codebot/Nullable.h>
#include <Codebot/Interop/Interface.h>
#include <Codebot/Interop/XmlInterop.h>
#include <Codebot/Time/DateTime.h>
#include <Codebot/Stream.h>
#include <Codebot/IEnumerable.h>

namespace Codebot
{
namespace Text
{
namespace Xml
{

class Node;
class NodeList;
class Attribute;
class AttributeList;
class Document;

class Filer : public ValueType
{
private:
	Interop::Interface<Interop::INode> node;
	Filer(Interop::INode* n);
public:
	friend class Node;
	String Read(const String& path, const String& value = "", Boolean stored = false);
	void Write(const String& path, const String& value);
	Boolean ReadData(const String& path, ISerializable* data, const String& format = "");
	void WriteData(const String& path, ISerializable* data, const String& format = "");
	String ReadStr(const String& path, const String& value = "", Boolean stored = false);
	void WriteStr(const String& path, const String& value);
	Integer ReadInt(const String& path, const Integer& value = 0, Boolean stored = false);
	void WriteInt(const String& path, const Integer& value);
	Double ReadFloat(const String& path, const Double& value = 0.0, Boolean stored = false);
	void WriteFloat(const String& path, const Double& value);
	Time::TimeSpan ReadTime(const String& path, const Time::TimeSpan& value = Time::TimeSpan::Empty, Boolean stored = false);
	void WriteTime(const String& path, const Time::TimeSpan& value);
	Time::DateTime ReadDate(const String& path, const Time::DateTime& value = Time::DateTime::Empty, Boolean stored = false);
	void WriteDate(const String& path, const Time::DateTime& value);
};

class Node : public ValueType
{
private:
	Interop::Interface<Interop::INode> node;
	Node(Interop::INode* n);
public:
	friend class Attribute;
	friend class Filer;
	friend class Document;
	friend class NodeList;
	operator String () const;
	Node& operator = (const String& s);
	Node Force(const String& path);
	Nullable<Node> SelectNode(const String& path) const;
	Nullable<NodeList> SelectList(const String& path) const;
	// Override methods
	String ToFormat(const String& format) const;
	// Properties
	Nullable<Node> Parent() const;
	Nullable<Document> Owner() const;
	AttributeList Attributes() const;
	NodeList Nodes() const;
	String Path() const;
	String Name() const;
	String Text() const;
	void Text(const String& value);
	String Xml() const;
	void Xml(const String& value);
	Xml::Filer Filer() const;
};

class NodeList : public ValueType, public IEnumerable<Node>
{
private:
	Interop::Interface<Interop::INodeList> nodes;
	NodeList(Interop::INodeList* n);
public:
	friend class Document;
	friend class Node;
	void Clear();
	void Add(const Node& node);
	Node Add(const String& name);
	void Remove(const Node& node);
	void Remove(const String& name);
	Nullable<Node> operator [] (const String& name) const;
	Node operator [] (Integer index) const;
	// Properties
	Integer Length() const;
	// IEnumerable<Node>
	IEnumerator<Node>* GetEnumerator() const;
};

class Attribute : public ValueType
{
private:
	Interop::Interface<Interop::INode> attribute;
	Attribute(Interop::INode* a);
public:
	friend class AttributeList;
	friend class Document;
	operator String () const;
	Attribute& operator = (const String& s);
	// Override methods
	String ToFormat(const String& format) const;
	// Properties
	Nullable<Node> Parent() const;
	Nullable<Document> Owner() const;
	String Path() const;
	String Name() const;
	String Text() const;
	void Text(const String& value);
	String Xml() const;
	void Xml(const String& value);
};

class AttributeList : public ValueType, public IEnumerable<Attribute>
{
private:
	Interop::Interface<Interop::INodeList> attributes;
	AttributeList(Interop::INodeList* a);
public:
	friend class Node;
	void Clear();
	void Add(const Attribute& attribute);
	Attribute Add(const String& name);
	void Remove(const String& name);
	void Remove(const Attribute& attribute);
	Nullable<Attribute> operator [] (const String& name) const;
	Attribute operator [] (Integer index) const;
	// Properties
	Integer Length() const;
	// IEnumerable<Attribute>
	IEnumerator<Attribute>* GetEnumerator() const;
};

class Document : public ValueType
{
private:
	Interop::Interface<Interop::IDocument> document;
	Document(Interop::IDocument* d);
public:
	friend class Attribute;
	friend class Node;
	Document();
	Document(const String& s);
	operator String () const;
	String operator ()() const;
	Document& operator ()(const String& s);
	Document& operator = (const String& s);
	void Beautify();
	Attribute CreateAttribute(const String& name) const;
	Node CreateNode(const String& name) const;
	Node Force(const String& path);
	Nullable<Node> SelectNode(const String& path) const;
	Nullable<NodeList> SelectList(const String& path) const;
	void Load(const String& fileName);
	void Save(const String& fileName) const;
	Nullable<Node> GetRoot() const;
	void SetRoot(const Node& node);
	// Override methods
	String ToFormat(const String& format) const;
	// Properties
	Node DocumentNode() const;
	String Text() const;
	void Text(const String& value);
	String Xml() const;
	void Xml(const String& value);
};

}
}
}
