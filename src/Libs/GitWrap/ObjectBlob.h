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

#ifndef GIT_OBJECT_BLOB_H
#define GIT_OBJECT_BLOB_H

#include "GitWrap.h"
#include "ObjectId.h"
#include "Object.h"

namespace Git
{

    /**
	 * @ingroup		GitWrap
	 * @brief		Provides access to git BLOB (Binary Large Object) objects
	 *
	 */
	class GITWRAP_API ObjectBlob : public Object
	{
	public:
		ObjectBlob();
		ObjectBlob( Internal::ObjectPrivate* _d );
		ObjectBlob( const ObjectBlob& o );
	};

}

Q_DECLARE_METATYPE( Git::ObjectBlob )

#endif
