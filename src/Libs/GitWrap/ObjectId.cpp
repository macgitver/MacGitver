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
		memset( data, 0, SHA1_Length );
	}

	ObjectId::ObjectId( const QByteArray& d )
	{
		Q_ASSERT( d.length() == SHA1_Length );
		memcpy( data, d.constData(), SHA1_Length );
	}


	ObjectId ObjectId::fromString( const QString& oid, int max, bool* success )
	{
		return fromAscii( oid.toAscii(), max, success );
	}

	ObjectId ObjectId::fromAscii( const QByteArray& oid, int max, bool* success )
	{
		git_oid gitoid;

		if( git_oid_fromstrn( &gitoid, oid.constData(),
							  qMin( qMin( max, int(SHA1_LengthHex) ), oid.length() ) ) < 0 )
		{
			if( success )
			{
				*success = false;
			}
			return ObjectId();
		}

		if( success )
		{
			*success = true;
		}

		return fromRaw( gitoid.id, SHA1_Length );
	}

	ObjectId ObjectId::fromRaw( const unsigned char* d, int len )
	{
		ObjectId id;
		memcpy( id.data, d, qMin( len, int( SHA1_Length ) ) );
		return id;
	}

	QString ObjectId::toString() const
	{
		return QString::fromUtf8( toAscii().constData() );	// UTF-8 is Ascii, actually :-)
	}

	QByteArray ObjectId::toAscii() const
	{
		QByteArray id( SHA1_LengthHex + 1, 0 );
		git_oid_tostr( id.data(), SHA1_LengthHex +  1, (const git_oid*) data );
		return id;
	}

	bool ObjectId::operator==( const ObjectId& other ) const
	{
		return !memcmp( data, other.data, SHA1_Length );
	}

	bool ObjectId::operator!=( const ObjectId& other ) const
	{
		return !!memcmp( data, other.data, SHA1_Length );
	}

	bool ObjectId::isNull() const
	{
		for( int i = 0; i < SHA1_Length; i++ )
			if( data[ i ] )
				return false;

		return true;
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
