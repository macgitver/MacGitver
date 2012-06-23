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

#include "git2.h"

#include "ObjectId.h"

namespace Git
{

	ObjectId::ObjectId()
	{
		memset( data, 0, 20 );
	}

	ObjectId::ObjectId( const QByteArray& d )
	{
		Q_ASSERT( d.length() == 20 );
		memcpy( data, d.constData(), 20 );
	}

	ObjectId ObjectId::fromRaw( const unsigned char* d, int len )
	{
		ObjectId id;
		memcpy( id.data, d, qMin( len, 20 ) );
		return id;
	}

	QString ObjectId::toString() const
	{
		return QString( toAscii() );
	}

	QByteArray ObjectId::toAscii() const
	{
		QByteArray id( 41, 0 );
		git_oid_tostr( id.data(), 41, (const git_oid*) data );
		return id;
	}

	bool ObjectId::operator==( const ObjectId& other ) const
	{
		return !memcmp( data, other.data, 20 );
	}

	bool ObjectId::operator!=( const ObjectId& other ) const
	{
		return !!memcmp( data, other.data, 20 );
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
