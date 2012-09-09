/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GIT_OBJECT_ID_H
#define GIT_OBJECT_ID_H

#include <QString>
#include <QByteArray>
#include <QDebug>

#include "Git.h"

namespace Git
{

    /**
     * @brief The ObjectId class represents the object identifier (OID) of a Git object.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API ObjectId
	{
	public:
		ObjectId();
		ObjectId( const QByteArray& raw );

	public:
		static ObjectId fromString( const QString& oid, int max = 40, bool *success = NULL );
		static ObjectId fromAscii( const QByteArray& oid, int max = 40, bool *success = NULL );
		static ObjectId fromRaw( const unsigned char* raw, int n = 20 );

		QString toString() const;
		QByteArray toAscii() const;

		const unsigned char* raw() const
		{
			return data;
		}

		bool isNull() const;

		bool operator==( const ObjectId& other ) const;
		bool operator!=( const ObjectId& other ) const;

	private:
		unsigned char data[ 20 ];
	};

	typedef QVector< ObjectId > ObjectIdList;

	GITWRAP_API uint qHash( const ObjectId& sha1 );

    /**@}*/
}

inline QDebug operator<<( QDebug debug, const Git::ObjectId& id )
{
	return debug << "SHA1(" << id.toString() << ")";
}

#endif
