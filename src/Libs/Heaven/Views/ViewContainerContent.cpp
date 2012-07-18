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

#include "Heaven/Views/ViewContainerContent.h"
#include "Heaven/Views/ViewContainer.h"

namespace Heaven
{

	ViewContainerContent::ViewContainerContent( ViewContainer* parentContainer )
		: mParentContainer( NULL )
	{
	}

	ViewContainerContent::~ViewContainerContent()
	{
		if( mParentContainer )
		{
			mParentContainer->take( this );
		}
	}

	View* ViewContainerContent::asView()
	{
		return NULL;
	}

	ViewContainer* ViewContainerContent::asContainer()
	{
		return NULL;
	}

	QWidget* ViewContainerContent::widget()
	{
		return isContainer() ? asContainer()->containerWidget() : asView();
	}

	ViewContainer* ViewContainerContent::container() const
	{
		return mParentContainer;
	}

	void ViewContainerContent::setContainer( ViewContainer* container )
	{
		mParentContainer = container;
	}

}
