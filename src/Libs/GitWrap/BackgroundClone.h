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

#ifndef GIT_BACKGROUND_CLONE_H
#define GIT_BACKGROUND_CLONE_H

#include "GitWrap.h"
#include "Repository.h"
#include "BackgroundTasks.h"

namespace Git
{

    /**
     * @brief The BackgroundClone class provides functionality to clone a repository in a background thread.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API BackgroundClone : public BackgroundTask
	{
		Q_OBJECT
	public:
		BackgroundClone();

	public:
		void setFrom( const QString& url, const QString& path );
		void setRemoteName( const QString& remoteName );
		void setType( bool bare, bool mirror, bool checkout );
		void setBranch( const QString& name, bool onlyThisOne );
		void setSubmodule( bool init, bool recursive );

	public:
		Repository repository();

	public:
		bool prepare();
		bool execute();

	private:
		Repository		mRepo;
		QString			mUrl;
		QString			mPath;
		QString			mRemoteName;
		bool			mBare;
		bool			mMirror;
		bool			mCheckout;
		bool			mFetchOnlyBranch;
		bool			mInitSubmodules;
		bool			mInitSubmodulesRecursive;
		bool			mPrepared;
		QString			mCheckoutBranch;
	};

    /**@}*/
}

#endif
