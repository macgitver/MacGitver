/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include "Libs/Git/Git.h"
#include "Libs/Git/ObjectId.h"
#include "Libs/Git/Object.h"

namespace Git
{

	class ObjectTree : public Object
	{
	public:
		ObjectTree();
		ObjectTree( ObjectPrivate* _d );
		ObjectTree( const ObjectTree& o );

	public:
		ObjectTree subPath( const QByteArray& pathName ) const;
	};

}

#endif
