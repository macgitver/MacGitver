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

#include "Heaven/Views/View.h"

namespace Heaven
{

	View::View( const QString& identifier, ViewTypes type )
		: mIdentifier( identifier )
		, mType( type )
		, mAction( NULL )
	{
	}

	QString View::viewName() const
	{
		return mViewName;
	}

	void View::setViewName( const QString& name )
	{
		if( name != mViewName )
		{
			mViewName = name;
			emit nameChanged( mViewName );
		}
	}

	QString View::identifier() const
	{
		return mIdentifier;
	}


	ViewTypes View::type() const
	{
		return mType;
	}

}
