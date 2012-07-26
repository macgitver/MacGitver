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

#include "Heaven/Views/TopLevelWidget.h"
#include "Heaven/Views/WindowState.h"

namespace Heaven
{

	WindowStateBase::WindowStateBase( WindowStateBase* parent )
		: mParent( parent )
	{
		if( mParent )
		{
			mParent->mChildren.append( Ptr( this ) );
		}
	}

	WindowStateBase::~WindowStateBase()
	{
		Q_ASSERT( mChildren.count() == 0 );

		if( mParent )
		{
			int i = mParent->mChildren.indexOf( Ptr( this ) );
			if( i != -1 )
			{
				mParent->mChildren.remove( i );
			}
		}
	}

	int WindowStateBase::childrenCount() const
	{
		return mChildren.count();
	}

	WindowStateBase::Ptr WindowStateBase::childAt( int index ) const
	{
		return mChildren.at( index );
	}

	QVector< WindowStateBase::Ptr > WindowStateBase::children() const
	{
		return mChildren;
	}

	bool WindowStateBase::import( ViewContainerContent* cc, WindowStateBase* parent )
	{
		if( cc->isContainer() )
		{
			WindowStateBase* newParent = NULL;
			ViewContainer* vc = cc->asContainer();

			switch( vc->type() & ViewContainer::BaseMask )
			{
			case ViewContainer::Tab:
				newParent = new WindowStateTab( vc, parent );
				break;

			case ViewContainer::Splitter:
				newParent = new WindowStateSplitter( vc, parent );
				break;

			default:
				return false;
			}

			foreach( ViewContainerContent* cc, vc->contents() )
			{
				if( !import( cc, newParent ) )
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			new WindowStateView( cc->asView(), parent );

			return true;
		}
	}

	WindowStateSplitter::WindowStateSplitter( WindowStateBase* parent )
		: WindowStateBase( parent )
	{
	}

	WindowStateSplitter::WindowStateSplitter( ViewContainer* vc, WindowStateBase* parent )
		: WindowStateBase( parent )
	{
	}

	WindowStateBase::Type WindowStateSplitter::type() const
	{
		return WSSplitter;
	}

	void WindowStateSplitter::setVertical( bool value )
	{
		mVertical = value;
	}

	void WindowStateSplitter::apply( ApplyContext& ctx )
	{
	}

	WindowStateTab::WindowStateTab( ViewContainer* vc, WindowStateBase* parent )
		: WindowStateBase( parent )
	{
	}

	WindowStateTab::WindowStateTab( WindowStateBase* parent )
		: WindowStateBase( parent )
	{
	}

	WindowStateBase::Type WindowStateTab::type() const
	{
		return WSTab;
	}

	void WindowStateTab::setTabSubType( ViewContainer::Type type )
	{
		mTabSubType = type;
	}

	void WindowStateTab::apply( ApplyContext& ctx )
	{
	}

	WindowStateView::WindowStateView( WindowStateBase* parent )
		: WindowStateBase( parent )
	{
	}

	WindowStateView::WindowStateView( View* view, WindowStateBase* parent )
		: WindowStateBase( parent )
	{
		mViewId = view->identifier();
	}

	WindowStateBase::Type WindowStateView::type() const
	{
		return WSView;
	}

	void WindowStateView::setViewId( const QString& id )
	{
		mViewId = id;
	}

	void WindowStateView::apply( ApplyContext& ctx )
	{
	}

	WindowStateRoot::WindowStateRoot()
		: WindowStateBase( NULL )
	{
	}

	WindowStateRoot::WindowStateRoot( TopLevelWidget* tlWidget )
		: WindowStateBase( NULL )
	{
		ViewContainer* vc = tlWidget->rootContainer();

		import( vc, this );
	}

	WindowStateRoot::~WindowStateRoot()
	{
	}

	WindowStateBase::Type WindowStateRoot::type() const
	{
		return WSRoot;
	}

	void WindowStateRoot::apply( TopLevelWidget* tlWidget, ViewFactory* factory )
	{
		if( !tlWidget || !factory )
		{
			Q_ASSERT( false );
			return;
		}

		ApplyContext ctx;
		ctx.tlw = tlWidget;
		ctx.factory = factory;
		ctx.container = NULL;
		ctx.previousViews = tlWidget->setOfViews();

		apply( ctx );
	}

	void WindowStateRoot::apply( ApplyContext& ctx )
	{
	}

}
