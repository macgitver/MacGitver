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

#ifndef GIT_REFERENCE_H
#define GIT_REFERENCE_H

#include "Git.h"

namespace Git
{

	class ReferencePrivate;

	class ObjectId;

	class GITWRAP_API Reference
	{
	public:
		enum Type
		{
			Direct, Symbolic
		};

	public:
		Reference();
		Reference( ReferencePrivate* p );

	public:
		bool isValid() const;
		void destroy();
		QByteArray name() const;

		Type type() const;
		ObjectId objectId() const;
		QByteArray target() const;

	private:
		ReferencePrivate* d;
	};

}

#endif
