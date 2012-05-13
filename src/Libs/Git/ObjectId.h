
#ifndef GIT_OBJECT_ID_H
#define GIT_OBJECT_ID_H

#include <QString>
#include <QByteArray>
#include <QDebug>

#include "Libs/Git/Git.h"

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

		const unsigned char* raw() const
		{
			return (const unsigned char*) d.constData();
		}

		bool operator==( const ObjectId& other ) const;
		bool operator!=( const ObjectId& other ) const;

	private:
		QByteArray	d;
	};

	uint qHash( const ObjectId& sha1 );

}

inline QDebug operator<<( QDebug debug, const Git::ObjectId& id )
{
	return debug << "SHA1(" << id.toString() << ")";
}

#endif
