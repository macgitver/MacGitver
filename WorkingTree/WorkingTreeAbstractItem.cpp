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

#include <QDebug>

#include "WorkingTreeModel.h"
#include "WorkingTreeDirItem.h"
#include "WorkingTreeAbstractItem.h"

WorkingTreeAbstractItem::WorkingTreeAbstractItem( WorkingTreeModel* model, WorkingTreeAbstractItem* parent )
{
	mModel = model;
	mParent = parent;
}

WorkingTreeAbstractItem::~WorkingTreeAbstractItem()
{
}

WorkingTreeModel* WorkingTreeAbstractItem::model()
{
	return mModel;
}

WorkingTreeAbstractItem* WorkingTreeAbstractItem::parent()
{
	return mParent;
}

QModelIndex WorkingTreeAbstractItem::index() const
{
	if( !mParent )
	{
		return QModelIndex();
	}

    QModelIndex idx = mModel->index( row(), 0, mParent->index() );
	return idx;
}
