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

#ifndef GIT_REFSPEC_H
#define GIT_REFSPEC_H

#include <QString>

#include "GitWrap.h"

namespace Git
{

    /**
	 * @ingroup		GitWrap
	 * @brief		Specification for references
     *
	 * A RefSpec tells git what branches are to be synchronized in a fetch or a push operation.
	 *
	 * Each remote has two lists of refspecs, one for fetching (`remote.<name>.fetchspec`) and for
	 * pulling (`remote.<name>.pullspec`) defaulting to the fetchspec.
	 *
	 * @note		`libgit2` currently supports only one fetchspec and only one pullspec per
	 *				remote.
	 *
	 * @todo		The force flag is still missing. `libgit2` meanwhile supports reading the force
	 *				flag.
	 *
     */
	class RefSpec
	{
	public:
		RefSpec();
		RefSpec( const RefSpec& other );
		RefSpec( const QString& source, const QString& destination );

	public:
		QString source() const;
		QString destination() const;

	private:
		QString mSrc;
		QString mDst;
	};

}

Q_DECLARE_METATYPE( Git::RefSpec )

#endif
