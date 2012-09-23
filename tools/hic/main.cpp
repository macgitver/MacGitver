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

#include <stdio.h>

#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>

#include "HICObject.h"
#include "HIDLexer.h"
#include "HIDParser.h"
#include "HIGenHeader.h"
#include "HIGenSource.h"

int main( int argc, char** argv )
{
	QCoreApplication app( argc, argv );
	HIDTokenStream mTokenStream;
	HIDModel model;

	QStringList sl = QCoreApplication::arguments();

	if( sl.count() != 4 )
	{
		fprintf( stderr, "Usage: %s <input> <output-header> <output-source>\n",
				 sl.count() ? qPrintable( sl[ 0 ] ) : "" );
		return -1;
	}

	QFile fInput( sl[ 1 ] );
	if( !fInput.open( QFile::ReadOnly ) )
	{
		fprintf( stderr, "Cannot read from %s\n", qPrintable( sl[ 1 ] ) );
		return -1;
	}

	if( !HIDLexer::lex( fInput, mTokenStream ) )
	{
		fprintf( stderr, "Could not tokenize input from %s\n", qPrintable( sl[ 1 ] ) );
		return -1;
	}

	if( !HIDParser::parse( mTokenStream, model ) )
	{
		fprintf( stderr, "Could not parse input from %s\n", qPrintable( sl[ 1 ] ) );
		return -1;
	}

	HIGenHeader genHeader( model, sl[ 2 ] );
	if( !genHeader.generate() )
	{
		fprintf( stderr, "Could not generate %s\n", qPrintable( sl[ 2 ] ) );
		return -1;
	}

	HIGenSource genSource( model, sl[ 3 ], QFileInfo( sl[ 2 ] ).fileName() );
	if( !genSource.generate() )
	{
		fprintf( stderr, "Could not generate %s\n", qPrintable( sl[ 3 ] ) );
		return -1;
	}

	return 0;
}
