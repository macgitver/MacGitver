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

#include "PatchConsumer.h"

namespace Git
{

	PatchConsumer::PatchConsumer()
	{
	}

	PatchConsumer::~PatchConsumer()
	{
	}

	bool PatchConsumer::startFile( const QString& oldPath, const QString& newPath, Type type,
								   unsigned int similarity, bool isBinary )
	{
		Q_UNUSED( oldPath );
		Q_UNUSED( newPath );
		Q_UNUSED( type );
		Q_UNUSED( similarity );
		Q_UNUSED( isBinary );

		return false;
	}

	bool PatchConsumer::startHunk( int newStart, int newLines, int oldStart, int oldLines,
								   const QString& header )
	{
		Q_UNUSED( newStart );
		Q_UNUSED( newLines );
		Q_UNUSED( oldStart );
		Q_UNUSED( oldLines );
		Q_UNUSED( header );

		return false;
	}

	bool PatchConsumer::appendContext( const QString& content )
	{
		Q_UNUSED( content );

		return false;
	}

	bool PatchConsumer::appendAddition( const QString& content )
	{
		Q_UNUSED( content );

		return false;
	}

	bool PatchConsumer::appendDeletion( const QString& content )
	{
		Q_UNUSED( content );

		return false;
	}

}
