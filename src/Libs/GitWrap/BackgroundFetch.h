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

#ifndef GIT_BACKGROUND_FETCH_H
#define GIT_BACKGROUND_FETCH_H

#include "GitWrap.h"
#include "BackgroundTasks.h"
#include "Repository.h"
#include "Remote.h"

namespace Git
{

    /**
     * @brief The BackgroundFetch class provides functionality to fetch a repository in a background thread.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API BackgroundFetch : public BackgroundTask
	{
		Q_OBJECT
	public:
		BackgroundFetch();

	public:
		void setRepository( Repository repo );
		void setRemote( Remote remote );

	public:
		bool execute();

	private:
		Repository	mRepo;
		Remote		mRemote;
	};

    /**@}*/
}

#endif
