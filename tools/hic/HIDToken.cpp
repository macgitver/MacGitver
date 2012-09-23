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

#include "HIDToken.h"

HIDTokenStream::HIDTokenStream()
{
	mReadPos = 0;
}

HIDTokenId HIDTokenStream::cur() const
{
	return endOfStream() ? Token_EOF : at( mReadPos ).id;
}

void HIDTokenStream::append( const HIDToken& token )
{
	QList::append( token );
}

const HIDToken& HIDTokenStream::curToken() const
{
	return at( mReadPos );
}

QString HIDTokenStream::curValue() const
{
	return at( mReadPos ).value;
}

void HIDTokenStream::advance() const
{
	mReadPos++;
}

bool HIDTokenStream::advanceAndExpect( HIDTokenId id ) const
{
	advance();
	return cur() == id;
}

bool HIDTokenStream::endOfStream() const
{
	return !( mReadPos < count() );
}
