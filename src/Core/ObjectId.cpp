
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

}
