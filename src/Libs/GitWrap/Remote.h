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

#ifndef GIT_REMOTE_H
#define GIT_REMOTE_H

#include "Git.h"

namespace Git
{

	class RemotePrivate;

	class ObjectId;
	class Reference;
	class RefSpec;

	class GITWRAP_API Remote
	{
	public:
		Remote();
		Remote( RemotePrivate* _d );
		~Remote();
		Remote& operator=( const Remote& other );

	public:
		bool isValid() const;

		void save();

		QByteArray name() const;
		QByteArray url() const;

		void setFetchSpec( const QByteArray& spec );
		void setPushSpec( const QByteArray& spec );

		RefSpec fetchSpec() const;
		RefSpec pushSpec() const;

		static bool isValidUrl( const QByteArray& url );
		static bool isSupportedUrl( const QByteArray& url );

	private:
		GitPtr< RemotePrivate > d;
	};

}

#endif
