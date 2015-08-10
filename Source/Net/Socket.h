#pragma once
#include <Codebot/Object.h>

namespace Codebot
{
namespace Net
{

class AddressData : public ValueType
{
private:
	LongWord data;
public:
	AddressData();
	AddressData(Byte a, Byte b, Byte c, Byte d);
	AddressData(LongWord address);
	operator LongWord () const;
	AddressData& operator = (LongWord address);
	Byte& operator [] (Integer index);
	const Byte& operator [] (Integer index) const;
	Boolean Valid() const;
};

class AddressName : public ValueType, public ISerializable
{
private:
	String name;
	String location;
	AddressData address;
	Boolean resolved;
public:
	AddressName();
	AddressName(const String& s);
	AddressName(const AddressData& a);
	AddressName(Byte a, Byte b, Byte c, Byte d);
	operator String ();
	AddressName& operator = (const String& s);
	Boolean Resolve();
	String Name() const;
	String Location() const;
	AddressData Address() const;
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

enum SocketState
{
	SocketStateNone,
	SocketStateServer,
	SocketStateClient,
	SocketStateRemote
};

class Socket : public Object
{
private:
	Socket* server;
	Integer handle;
	AddressName addressName;
	Word port;
	SocketState state;
public:
	Socket();
	Socket(Socket* server);
	~Socket();
	void Close();
	Boolean Connect(const AddressName& host, Word port);
	Boolean Listen(const AddressName& adapter, Word port);
	Boolean Listen(Word port);
	Boolean Accept(Socket* socket);
	Integer Read(Pointer data, Cardinal size);
	Integer Write(Pointer data, Cardinal size);	
	Boolean Read(String& text);
	Boolean Write(const String& text);	
	// Properties
	const AddressName& Address() const;
	Word Port() const;
	SocketState State() const;
	Socket* Server() const;
};

}
}
