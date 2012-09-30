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

#include <stdlib.h>
#include <stdio.h>

#include <QFile>
#include <QDir>
#include <QString>

#include <QStringBuilder>
#include <QDomDocument>

#include "Model.hpp"

#include "WriteClassSource.hpp"
#include "WriteClassHeader.hpp"

void printUsage()
{
	fputs( "MacGitver's Config Compiler\n"
		   "\n"
		   "Usage:\n"
		   "\tCfgComp <input file> <output-dir>\n"
		   "\n"
		   "\t\t<input file>\tFile to process\n"
		   "\t\t<output-dir>\tPath where to put output files\n\n",
		   stderr
	);
}

int main( int argc, char** argv )
{
	if( argc != 3 )
	{
		printUsage();
		return -1;
	}

	QString inputFileName = QDir::fromNativeSeparators( QString::fromUtf8( argv[ 1 ] ) );
	QString outputFileDir = QDir::fromNativeSeparators( QString::fromUtf8( argv[ 2 ] ) );

	if( !QFile::exists( inputFileName ) )
	{
		fprintf( stderr, "File %s not found\n", argv[ 1 ] );
		printUsage();
		return -1;
	}

	if( !QFile::exists( outputFileDir ) )
	{
		fprintf( stderr, "File %s not found\n", argv[ 2 ] );
		printUsage();
		return -1;
	}

	if( !outputFileDir.endsWith( L'/' ) )
		outputFileDir += L'/';

	QFile fInput( inputFileName );
	if( !fInput.open( QFile::ReadOnly ) )
	{
		fprintf( stderr, "Cannot open %s for reading\n", qPrintable( inputFileName ) );
		printUsage();
		return -1;
	}

	QDomDocument doc;
	if( !doc.setContent( &fInput ) )
	{
		fprintf( stderr, "Cannot parse %s", qPrintable( inputFileName ) );
		printUsage();
		return -1;
	}

	ConfigSection sect( doc.documentElement() );

	WriteClassSource wcs( outputFileDir % sect.className() % QLatin1String( ".cpp" ), sect );
	wcs.generate();

	WriteClassHeader wch( outputFileDir % sect.className() % QLatin1String( ".hpp" ), sect );
	wch.generate();

	return 0;
}
