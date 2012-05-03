
#ifndef GIT_OBJECT_ID_H
#define GIT_OBJECT_ID_H

#include "Core/Git.h"

namespace Git
{

	class ObjectId
	{
	public:
		ObjectId();
		ObjectId( const QByteArray& raw );
		
	public:
		static ObjectId fromRaw( const unsigned char* raw, int n = 20 );

		QString toString() const;
		QByteArray toAscii() const;

	private:
		QByteArray	d;
	};

}

#endif
