/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#include "Libs/Core/ConfigPage.h"
#include "Libs/Core/ConfigDlg.h"

ConfigPageGroup::ConfigPageGroup()
	: mParent( NULL )
{
}

ConfigPageGroup::ConfigPageGroup( ConfigPageGroup* parent,
								  const QString& name,
								  const QByteArray &id )
	: mParent( parent )
	, mName( name )
	, mId( id )
{

}

ConfigPageGroup* ConfigPageGroup::getPageGroup( const QByteArray& id )
{
	return mChildren.value( id, NULL );
}

void ConfigPageGroup::addPageGroup( ConfigPageGroup* child )
{
	mChildren.insert( child->id(), child );
}

QByteArray ConfigPageGroup::id() const
{
	return mId;
}

QString ConfigPageGroup::name() const
{
	return mName;
}

ConfigPageGroup* ConfigPageGroup::parent()
{
	return mParent;
}

ConfigPage::ConfigPage( ConfigDlg* dlg )
	: QWidget( NULL )
{
	dlg->addPage( this );
}

ConfigPageGroup* ConfigPage::getOrMakeGroup( const QString& name,
											 const QByteArray& id,
											 ConfigPageGroup* parent )
{
	if( !parent )
	{
		if( mDialog )
		{
			parent = mDialog->rootConfigGroup();
		}
	}

	if( !parent )
	{
		return NULL;
	}

	ConfigPageGroup* me = parent->getPageGroup( id );

	if( !me )
	{
		me = new ConfigPageGroup( parent, name, id );
		parent->addPageGroup( me );
	}

	return me;
}
