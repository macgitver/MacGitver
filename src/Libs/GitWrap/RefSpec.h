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

#include <QByteArray>

#include "Git.h"

namespace Git
{

	class RefSpec
	{
	public:
		RefSpec();
		RefSpec( const RefSpec& other );
		RefSpec( const QByteArray& source, const QByteArray& destination );

	public:
		QByteArray source() const;
		QByteArray destination() const;

	private:
		QByteArray mSrc;
		QByteArray mDst;
	};

}

#endif
