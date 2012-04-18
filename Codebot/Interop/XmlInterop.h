#pragma once
#include <Codebot/Interop/IInterface.h>

namespace Codebot
{
namespace Interop
{

struct IDocument;
struct INodeList;

struct INode : public IInterface
{
	static const Guid Id;
	virtual Pointer stdcall Instance() = 0;
	virtual Boolean stdcall GetDocument(IDocument*& document) = 0;
	virtual Boolean stdcall GetParent(INode*& node) = 0;
	virtual Boolean stdcall SelectNode(const PChar xpath, INode*& node) = 0;
	virtual Boolean stdcall SelectList(const PChar xpath, INodeList*& list) = 0;
	virtual void stdcall Attributes(INodeList*& list) = 0;
	virtual void stdcall Nodes(INodeList*& list) = 0;
	virtual Integer stdcall GetKind() = 0;
	virtual const PChar stdcall GetName() = 0;
	virtual const PChar stdcall GetText() = 0;
	virtual void stdcall SetText(const PChar value) = 0;
	virtual const PChar stdcall GetXml() = 0;
	virtual void stdcall SetXml(const PChar value) = 0;
};

struct INodeList : public IInterface
{
	static const Guid Id;
	virtual void stdcall Clear() = 0;
	virtual void stdcall AddNode(INode* node) = 0;
	virtual void stdcall AddName(const PChar name, INode*& node) = 0;
	virtual void stdcall RemoveNode(INode* node) = 0;
	virtual void stdcall RemoveName(const PChar name) = 0;
	virtual void stdcall Item(Integer index, INode*& node) = 0;
	virtual Boolean stdcall Named(const PChar name, INode*& node) = 0;
	virtual Integer stdcall Length() = 0;
};

struct IDocument : public INode
{
	static const Guid Id;
	virtual void stdcall Beautify() = 0;
	virtual void stdcall CreateAttribute(const PChar name, INode*& attribute) = 0;
	virtual void stdcall CreateElement(const PChar name, INode*& element) = 0;
	virtual void stdcall SetRoot(INode* element) = 0;
	virtual Boolean stdcall GetRoot(INode*& element) = 0;
	virtual void stdcall Load(const PChar fileName) = 0;
	virtual void stdcall Save(const PChar fileName) = 0;
};

extern "C"
{
	import void XmlDocumentCreate(IDocument*& document);
	import Integer XmlGetCreated();
	import Integer XmlGetRefCount();
};

}
}
