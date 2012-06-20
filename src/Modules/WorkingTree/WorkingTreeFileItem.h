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

#ifndef MGV_WORKING_TREE_FILE_ITEM_H
#define MGV_WORKING_TREE_FILE_ITEM_H

#include <QList>
#include <QDateTime>
#include <QString>
#include <QIcon>

#include "WorkingTreeAbstractItem.h"
#include "WorkingTreeFilters.h"

class WorkingTreeDirItem;

class WorkingTreeFileItem : public WorkingTreeAbstractItem
{
public:
	WorkingTreeFileItem( WorkingTreeAbstractItem* parent );
	~WorkingTreeFileItem();

public:
	void setName( const QString& name );
	void setState( WorkingTreeFilter state );
	void setIcon( const QIcon& icon );
	void setSize( qint64 size );
	void setOwner( const QString& owner );
	void setLastModified( const QDateTime& lastMod );

public:
	QString name() const;
	bool isDirectory() const;
	int visibleChildren() const;
	WorkingTreeAbstractItem* visibleChildAt( int index );
	QVariant data( int column, int role ) const;
	WorkingTreeAbstractItem* parent();
	int visibleIndex() const;
	WorkingTreeAbstractItem* childByName( const QString& name );
	void removeChild( WorkingTreeAbstractItem* child );

	bool refilter( WorkingTreeFilters filters );

private:
	WorkingTreeAbstractItem*	mParent;
	QString						mName;
	QIcon						mIcon;
	QDateTime					mLastMod;
	QString						mOwner;
	qint64						mSize;
	WorkingTreeFilter			mState;
};

#endif
