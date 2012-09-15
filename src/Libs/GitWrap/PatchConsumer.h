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

#ifndef GITWRAP_PATCH_CONSUMER_H
#define GITWRAP_PATCH_CONSUMER_H

#include "GitWrap.h"

namespace Git
{

    /**
     * @brief The PatchConsumer class
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API PatchConsumer
	{
	public:
		enum Type
		{
			FileUnmodified,
			FileAdded,
			FileDeleted,
			FileModified,
			FileRenamed,
			FileCopied,
			FileIgnored,
			FileUntracked
		};

	public:
		PatchConsumer();
		virtual ~PatchConsumer();

	public:
		virtual bool startFile( const QString& oldPath, const QString& newPath, Type type,
								unsigned int similarity, bool isBinary );

		virtual bool startHunk( int newStart, int newLines, int oldStart, int oldLines,
								const QString& header );

		virtual bool appendContext( const QString& content );
		virtual bool appendAddition( const QString& content );
		virtual bool appendDeletion( const QString& content );
	};

    /**@}*/
}

#endif
