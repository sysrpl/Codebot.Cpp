#include <Codebot/Types.h>
#ifdef LinuxOS
#include <Codebot/Net/Socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


namespace Codebot
{
namespace Net
{

// AddressData

AddressData::AddressData()
	: data(0)
{
}

AddressData::AddressData(LongWord address)
	: data(address)
{
}

AddressData::AddressData(Byte a, Byte b, Byte c, Byte d)
	: data(a | b << 8 | c << 16 | d << 24)
{
}

AddressData::operator LongWord () const
{
	return data;
}

AddressData& AddressData::operator = (LongWord address)
{
	data = address;
	return *this;
}

Byte& AddressData::operator [] (Integer index)
{
	Byte* b = (Byte*)&data;
	if (index < 0)
		index = 0;
	else if (index > 3)
		index = 3;
	return b[index];
}

const Byte& AddressData::operator [] (Integer index) const
{
	Byte* b = (Byte*)&data;
	if (index < 0)
		index = 0;
	else if (index > 3)
		index = 3;
	return b[index];
}

Boolean AddressData::Valid() const
{
	return data != 0;
}

// AddressName

AddressName::AddressName()
	: name(), location(), address(), resolved(false)
{
}

AddressName::AddressName(const String& s)
	: name(s), location(), address(), resolved(false)
{
}

AddressName::AddressName(const AddressData& a)
	: name(), location(), address(a), resolved(true)
{
	LongWord addr = address;
	location = inet_ntoa(*(in_addr*)&addr);
}

AddressName::AddressName(Byte a, Byte b, Byte c, Byte d)
	: name(), location(), address(a, b, c, d), resolved(true)
{
	LongWord addr = address;
	location = inet_ntoa(*(in_addr*)&addr);
}

AddressName::operator String ()
{
	Resolve();
	return location;
}

AddressName& AddressName::operator = (const String& s)
{
	name = s;
	location = String();
	address = Address();
	resolved = false;
	return *this;
}

Boolean AddressName::Resolve()
{
	if (resolved)
		return location.Length() > 0;
	resolved = true;
	hostent* h = gethostbyname(name);
	if (h == null)
		return false;
	in_addr a =*(in_addr*)h->h_addr;
	address = a.s_addr;
	location = inet_ntoa(a);
	return true;
}

String AddressName::Name() const
{
	return name;
}

String AddressName::Location() const
{
	return location;
}

AddressData AddressName::Address() const
{
	return address;
}

// ISerializable

String AddressName::ToFormat(const String& format) const
{
	return location;
}

Boolean AddressName::FromFormat(const String& value, const String& format)
{
	*this = value;
	return Resolve();
}

// Socket

#define INVALID_SOCKET -1

Socket::Socket()
	: server(), handle(INVALID_SOCKET), addressName(), port(0), state(SocketStateNone)
{
}

Socket::Socket(Socket* server)
	: handle(INVALID_SOCKET), addressName(), port(0), state(SocketStateNone)
{
	this->server = server;
}

Socket::~Socket()
{
	Close();
}

void Socket::Close()
{
	if (handle < 0)
		return;
	Handle h = handle;
	handle = INVALID_SOCKET;
	addressName = AddressName();
	port = 0;
	state = SocketStateNone;
	shutdown(h, SHUT_RDWR);
	close(h);
}

Boolean Socket::Connect(const AddressName& host, Word port)
{
	Close();
	addressName = host;
	this->port = port;
	if (!addressName.Resolve())
		return false;
	if (!addressName.Address().Valid())
		return false;
	if (port == 0)
		return false;
	handle = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (handle < 0)
		return false;
    sockaddr_in a;
    a.sin_family = AF_INET;
	a.sin_addr.s_addr = addressName.Address();
    a.sin_port = htons(port);
	if (connect(handle, (sockaddr*)&a, sizeof(a)) < 0)
	{
		Close();
		return false;
	}
	LongWord timeout = 4000;
	setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	state = SocketStateClient;
	return true;
}

Boolean Socket::Listen(const AddressName& adapter, Word port)
{
	Close();
	addressName = adapter;
	this->port = port;
	if (!addressName.Resolve())
		return false;
	if (port == 0)
		return false;
	handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (handle < 0)
	{
		handle = INVALID_SOCKET;
		return false;
	}
    sockaddr_in a;
    a.sin_family = AF_INET;
	if (addressName.Address().Valid())
		a.sin_addr.s_addr = addressName.Address();
	else
		a.sin_addr.s_addr = INADDR_ANY;
    a.sin_port = htons(port);
	if (bind(handle, (sockaddr*)&a, sizeof(a)) < 0)
	{
		Close();
		return false;
	}
	if (listen(handle, SOMAXCONN) < 0)
	{
		Close();
		return false;
	}
	state = SocketStateServer;
	return true;
}

Boolean Socket::Listen(Word port)
{
	return Listen(AddressName(), port);
}

Boolean Socket::Accept(Socket* socket)
{
	if (socket == null)
		ThrowNullReferenceException(ThisMethod, "socket");
	socket->Close();
	if (state != SocketStateServer)
		return false;
	sockaddr_in a;
	socklen_t i = sizeof(sockaddr_in);
	Integer h = accept(handle, (sockaddr*)&a, &i);
	if (h < 0)
		return false;
	socket->handle = h;
	LongWord timeout = 4000;
	setsockopt(socket->handle, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(socket->handle, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	socket->addressName = AddressName(AddressData(a.sin_addr.s_addr));
	socket->port = ntohs(a.sin_port);
	socket->state = SocketStateRemote;
	return true;
}

Integer Socket::Read(Pointer data, Cardinal size)
{
	if (state < SocketStateClient)
		return 0;
	auto count = recv(handle, (PChar)data, size, 0);
	if (count < 0)
	{
		Close();
		return -1;
	}
	return count;
}

Integer Socket::Write(Pointer data, Cardinal size)
{
	if (state < SocketStateClient)
		return 0;
	auto count = send(handle, (PChar)data, size, 0);
	if (count < 0)
	{
		Close();
		return -1;
	}
	return count;
}

Boolean Socket::Read(String& text)
{
	text = String::Empty;
	if (state < SocketStateClient)
		return false;
	text.Length(0x1000);
	Integer i = text.Length();
	i = recv(handle, &text[0], i, 0);
	if (i < 0)
	{
		Close();
		return false;
	}
	text.Length(i);
	return true;
}

Boolean Socket::Write(const String& text)
{
	if (state < SocketStateClient)
		return 0;
	Integer length = text.Length();
	Integer i = 0;
	while (i < length)
	{
		i += send(handle, &text[i], length - i, 0);
		if (i < 0)
		{
			Close();
			return false;
		}
	}
	return true;
}

const AddressName& Socket::Address() const
{
	return addressName;
}

Word Socket::Port() const
{
	return port;
}

SocketState Socket::State() const
{
	return state;
}

Socket* Socket::Server() const
{
	return server;
}

}
}
#endif
