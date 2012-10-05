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

#include <QRegExpValidator>

#include "Widgets/SHA1Input.h"

SHA1Input::SHA1Input( QWidget* parent )
    : LineEdit( parent )
{
    QRegExp re( QLatin1String( "[0-9a-fA-F]{5,40}" ) );
    setValidator( new QRegExpValidator( re, this ) );
}

bool SHA1Input::isValid() const
{
    return true;
}

Git::ObjectId SHA1Input::objectId() const
{
    return Git::ObjectId::fromString( text() );
}

void SHA1Input::setObjectId( const Git::ObjectId& sha1 )
{
    setText( sha1.toString() );
}
