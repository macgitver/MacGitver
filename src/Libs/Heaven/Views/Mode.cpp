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

#include "Mode.h"
#include "ModePrivate.h"
#include "MainWindow.h"

namespace Heaven
{

	Mode::Mode( MainWindow* owner )
		: QObject( owner )
		, d( new ModePrivate )
	{
		Q_ASSERT( owner );
	}

	Mode::~Mode()
	{
		delete d;
	}

	QString Mode::name() const
	{
		return d->mName;
	}

	void Mode::setName( const QString& name )
	{
		if( name != d->mName )
		{
			d->mName = name;
			emit nameChanged( name );
		}
	}

	void Mode::activate()
	{
	}

	void Mode::deactivate()
	{
	}

}
