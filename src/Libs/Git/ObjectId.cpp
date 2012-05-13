
#include "git2.h"

#include "Core/ObjectId.h"

namespace Git
{

	ObjectId::ObjectId()
	{
	}

	ObjectId::ObjectId( const QByteArray& _d )
		: d( _d )
	{
	}

	ObjectId ObjectId::fromRaw( const unsigned char* d, int len )
	{
		return ObjectId( QByteArray( (const char*) d, len ) );
	}

	QString ObjectId::toString() const
	{
		return QString( toAscii() );
	}

	QByteArray ObjectId::toAscii() const
	{
		QByteArray id( 41, 0 );
		git_oid_tostr( id.data(), 41, (const git_oid*) d.data() );
		return id;
	}

	bool ObjectId::operator==( const ObjectId& other ) const
	{
		if( d.length() == 20 && other.d.length() == 20 )
		{
			return !memcmp( d.constData(), other.d.constData(), 20 );
		}

		return false;
	}

	bool ObjectId::operator!=( const ObjectId& other ) const
	{
		if( d.length() == 20 && other.d.length() == 20 )
		{
			return !!memcmp( d.constData(), other.d.constData(), 20 );
		}

		return false;
	}

	static uint hash(const unsigned char *p, int n)
	{
		uint h = 0;

		while (n--) {
			h = (h << 4) + *p++;
			h ^= (h & 0xf0000000) >> 23;
			h &= 0x0fffffff;
		}
		return h;
	}

	uint qHash( const ObjectId& sha1 )
	{
		return hash( sha1.raw(), 20 );
	}

}
