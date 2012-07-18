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
#include "Heaven/Views/Container.h"

namespace Heaven
{
	ContainerContent::ContainerContent( ViewContainer* parentContainer )
		: mParentContainer( NULL )
	{
	}

	ContainerContent::~ContainerContent()
	{
		if( mParentContainer )
		{
			mParentContainer->take( this );
		}
	}

	View* ContainerContent::asView()
	{
		return NULL;
	}

	ViewContainer* ContainerContent::asContainer()
	{
		return NULL;
	}

	QWidget* ContainerContent::widget()
	{
		return isContainer() ? asContainer()->containerWidget() : asView();
	}

	ViewContainer* ContainerContent::container() const
	{
		return mParentContainer;
	}

	void ContainerContent::setContainer( ViewContainer* container )
	{
		mParentContainer = container;
	}

	View::View( const QString& identifier, ViewTypes type )
		: ContainerContent( NULL )
		, mIdentifier( identifier )
		, mType( type )
		, mAction( NULL )
	{
	}

	View::~View()
	{
	}

	bool View::isContainer() const
	{
		return false;
	}

	View* View::asView()
	{
		return this;
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

	void View::aboutToRemove()
	{
	}

}
