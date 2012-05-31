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

#include <QAction>

#include "Heaven/ActionPrivate.h"

namespace Heaven
{

	ActionPrivate::ActionPrivate( Action* owner )
		: UiObject( owner )
		, mEnabled( true )
		, mCheckable( false )
		, mChecked( false )
	{
	}

	ActionPrivate::~ActionPrivate()
	{
	}

	void ActionPrivate::setText( const QString& text )
	{
		mText = text;

		foreach( QAction* act, mQActions )
		{
			act->setText( text );
		}
	}

	void ActionPrivate::setStatusTip( const QString& text )
	{
		mStatusTip = text;

		foreach( QAction* act, mQActions )
		{
			act->setStatusTip( text );
		}
	}

	void ActionPrivate::setToolTip( const QString& text )
	{
		mToolTip = text;

		foreach( QAction* act, mQActions )
		{
			act->setToolTip( text );
		}
	}

	void ActionPrivate::setEnabled( bool v )
	{
		mEnabled = v;

		foreach( QAction* act, mQActions )
		{
			act->setEnabled( v );
		}
	}

	void ActionPrivate::setChecked( bool v )
	{
		mChecked = v;

		foreach( QAction* act, mQActions )
		{
			act->setChecked( v );
		}
	}

	void ActionPrivate::setCheckable( bool v )
	{
		mCheckable = v;

		foreach( QAction* act, mQActions )
		{
			act->setCheckable( v );
		}
	}

	void ActionPrivate::qactionDestroyed()
	{
		mQActions.remove( qobject_cast< QAction* >( sender() ) );
	}

	void ActionPrivate::qactionTriggered()
	{
		emit triggered();
	}

	void ActionPrivate::qactionToggled( bool checked )
	{
		QAction* from = qobject_cast< QAction* >( sender() );

		foreach( QAction* act, mQActions )
		{
			if( act != from )
			{
				act->setChecked( checked );
			}
		}

		emit toggled( checked );
	}

	QAction* ActionPrivate::createQAction( QObject* forParent )
	{
		QAction* a = new QAction( forParent );

		a->setText( mText );
		a->setStatusTip( mStatusTip );
		a->setToolTip( mToolTip );
		a->setEnabled( mEnabled );
		a->setCheckable( mCheckable );
		a->setChecked( mChecked );

		connect( a, SIGNAL(destroyed()), this, SLOT(qactionDestroyed()) );
		connect( a, SIGNAL(triggered()), this, SLOT(qactionTriggered()) );
		connect( a, SIGNAL(toggled(bool)), this, SLOT(qactionToggled(bool)) );

		mQActions.insert( a );
		return a;
	}

	QAction* ActionPrivate::getOrCreateQAction( QObject* forParent )
	{
		foreach( QAction* act, mQActions )
		{
			if( act->parent() == forParent )
			{
				return act;
			}
		}

		return createQAction( forParent );
	}

	UiObjectTypes ActionPrivate::type() const
	{
		return ActionType;
	}

	Action::Action( QObject* parent )
		: QObject( parent )
	{
		d = new ActionPrivate( this );

		connect( d, SIGNAL(triggered()), this, SIGNAL(triggered()) );
		connect( d, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)) );
	}

	Action::~Action()
	{
		delete d;
	}

	QAction* Action::actionFor( QObject* parent )
	{
		return d->getOrCreateQAction( parent );
	}

	UiObject* Action::uiObject()
	{
		return d;
	}

	QString Action::text() const
	{
		return d->mText;
	}

	QString Action::statusTip() const
	{
		return d->mStatusTip;
	}

	QString Action::toolTip() const
	{
		return d->mToolTip;
	}

	bool Action::isEnabled() const
	{
		return d->mEnabled;
	}

	bool Action::isCheckable() const
	{
		return d->mCheckable;
	}

	bool Action::isChecked() const
	{
		return d->mChecked;
	}

	void Action::setText( const QString& text )
	{
		if( text != d->mText )
		{
			d->setText( text );
		}
	}

	void Action::setStatusTip( const QString& text )
	{
		if( text != d->mStatusTip )
		{
			d->setStatusTip( text );
		}
	}

	void Action::setToolTip( const QString& text )
	{
		if( text != d->mToolTip )
		{
			d->setToolTip( text );
		}
	}

	void Action::setStatusToolTip( const QString& text )
	{
		setStatusTip( text );
		setToolTip( text );
	}

	void Action::setEnabled( bool v )
	{
		d->setEnabled( v );
	}

	void Action::setDisabled( bool v )
	{
		setEnabled( !v );
	}

	void Action::setChecked( bool v )
	{
		d->setChecked( v );
	}

	void Action::setCheckable( bool v )
	{
		d->setCheckable( v );
	}

}
