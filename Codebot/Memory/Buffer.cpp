#include "Codebot/Memory/Buffer.h"

namespace Codebot
{
namespace Memory
{

Buffer::Buffer() :
	data(new Bytes())
{
}

Buffer::Buffer(const Bytes& bytes) :
	data(new Bytes(bytes))
{
}

Buffer::Buffer(Cardinal size) :
	data(new Bytes())
{
	data->resize(size);
}

Buffer::Buffer(Byte* buffer, Cardinal size) :
	data(new Bytes())
{
	data->resize(size);
	MemCopy(&buffer[0], &(*data)[0], size);
}

Buffer::operator Byte* () const
{
	return data->size() == 0 ? null : (Byte*)&(*data)[0];
}

Buffer Buffer::Copy()
{
	auto s = Size();
	Buffer b(s);
	MemCopy((Byte*)*this, (Byte*)b, s);
	return b;
}

Cardinal Buffer::Size() const
{
	return (Cardinal)data->size();
}

void Buffer::Size(Cardinal value)
{
	if (value != (Cardinal)data->size())
	{
		data->resize((Cardinal)value);
		data->shrink_to_fit();
	}
}

// Buffer::ISerializable

static const String hex = "hex";
static const String base64 = "base64";
static const String defaultFormat = "hex";

String Buffer::ToFormat(const String& format) const
{
	String s = format.IsEmpty() ? defaultFormat : format;
	if (s == hex)
		return Hex::Encode(*this);
	if (s == base64)
		return Base64::Encode(*this);
	return String::Empty;
}

Boolean Buffer::FromFormat(const String& value, const String& format)
{
	String s = format.IsEmpty() ? defaultFormat : format;
	if (s == hex)
	{
		*this = Hex::Decode(value);
		return true;
	}
	else if (s == base64)
	{
		*this = Base64::Decode(value);
		return true;
	}
	return false;
}

// String encoding routines

namespace Hex
{
	String Encode(Byte* buffer, Cardinal size)
	{
		static Char hex[] = "0123456789ABCDEF";
		String s;
		Cardinal length = size;
		if (length == 0)
			return s;
		s.Length(length * 2);
		Byte* b = (Byte*)buffer;
		Char* c = &s[0];
		while (length > 0)
		{
			*c++ = hex[*b >> 0x4];
			*c++ = hex[*b & 0xF];
			b++;
			length--;
		}
		return s;
	}

	String Encode(const Buffer& buffer)
	{
		return Encode((Byte*)buffer, buffer.Size());
	}

	Buffer Decode(const String& s)
	{
		Cardinal length = s.Length();
		if (IsOdd(length))
			ThrowArgumentException(ThisMethod, s);
		if (length == 0)
			return Buffer();
		Buffer buffer(length >> 1);
		Byte* b = buffer;
		Char* c = &Var(s)[0];
		while (length > 0)
		{
			Boolean odd = IsOdd(length);
			if (CharInSet(*c, '0', '9'))
				*b = odd ? *b | (*c - '0') : (*c - '0') << 0x4;
			else if (CharInSet(*c, 'A', 'F'))
				*b = odd ? *b | (*c - 'A' + 0xA) : (*c - 'A' + 0xA) << 0x4;
			else
				ThrowArgumentException(ThisMethod, s);
			if (odd)
				b++;
			length--;
			c++;
		}
		return buffer;
	}
}

namespace Base64
{
	static const Char Fill = '=';

	static const String Convert =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

	String Encode(Byte* buffer, Integer length)
	{
		Char c;
		String s;
		for (Integer i = 0; i < length; ++i)
		{
			c = (buffer[i] >> 2) & 0x3f;
			s += Convert[c];
			c = (buffer[i] << 4) & 0x3f;
			if (++i < length)
				c |= (buffer[i] >> 4) & 0x0f;
			s += Convert[c];
			if (i < length)
			{
				c = (buffer[i] << 2) & 0x3f;
				if (++i < length)
					c |= (buffer[i] >> 6) & 0x03;
				s += Convert[c];
			}
			else
			{
				++i;
				s += Fill;
			}
			if (i < length)
			{
				c = buffer[i] & 0x3f;
				s += Convert[c];
			}
			else
				s += Fill;
		}
		return s;
	}

	String Encode(const Buffer& buffer)
	{
		return Encode((Byte*)buffer, buffer.Size());
	}

	Buffer Decode(const String& s)
	{
	  Char a;
	  Char b;
	  Bytes buffer;
	  Integer length = s.Length();
	  for (Integer i = 0; i < length; ++i)
	  {
	    a = (Char)Convert.IndexOf(s[i++]);
	    b = (Char)Convert.IndexOf(s[i++]);
	    a = (a << 2) | ((b >> 4) & 0x3);
	    buffer.push_back(a);
	    if (i < length)
	    {
	      a = s[i];
	      if (Fill == a)
	        break;
	      a = (Char) Convert.IndexOf(a);
	      b = ((b << 4) & 0xf0) | ((a >> 2) & 0xf);
	      buffer.push_back(b);
	    }
	    i++;
	    if (i < length)
	    {
	      b = s[i];
	      if (Fill == b)
	        break;
	      b = (Char)Convert.IndexOf(b);
	      a = ((a << 6) & 0xc0) | b;
	      buffer.push_back(a);
	    }
	  }
	  return buffer;
	}
}

}
}
