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

#ifndef GIT_OBJECT_TREE_H
#define GIT_OBJECT_TREE_H

#include "Git.h"
#include "ObjectId.h"
#include "Object.h"

namespace Git
{

	class DiffList;

	class GITWRAP_API ObjectTree : public Object
	{
	public:
		ObjectTree();
		ObjectTree( ObjectPrivate* _d );
		ObjectTree( const ObjectTree& o );

	public:
		ObjectTree subPath( const QString& pathName ) const;

		DiffList diffToTree( ObjectTree newTree );
		DiffList diffToIndex();
		DiffList diffToWorkingDir();
	};

}

#endif
