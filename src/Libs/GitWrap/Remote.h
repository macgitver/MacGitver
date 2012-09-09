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

    /**
     * @brief The Remote class represents a Git remote item, used to synchronize with remote repositories.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API Remote
	{
	public:
		Remote();
		Remote( RemotePrivate* _d );
		Remote( const Remote& other );
		~Remote();
		Remote& operator=( const Remote& other );

	public:
		bool isValid() const;

		bool save();

		QString name() const;
		QString url() const;

		bool setFetchSpec( const QString& spec );
		bool setPushSpec( const QString& spec );

		RefSpec fetchSpec() const;
		RefSpec pushSpec() const;

		static bool isValidUrl( const QString& url );
		static bool isSupportedUrl( const QString& url );

		bool connect( bool forFetch );
		void disconnect();
		bool download();
		bool updateTips();

	private:
		GitPtr< RemotePrivate > d;
	};

    /**@}*/
}

#endif
