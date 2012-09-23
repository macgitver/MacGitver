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

#include "HIGeneratorBase.h"

HIGeneratorBase::HIGeneratorBase( const HIDModel& model, const QString& fileName )
	: mModel( model )
	, mFileName( fileName )
{

}

QTextStream& HIGeneratorBase::out()
{
	return mOutStream;
}

QString HIGeneratorBase::latin1Encode( const QString& src )
{
	return src;
}

QString HIGeneratorBase::utf8Encode( const QString& src )
{
	QByteArray a = src.toUtf8();
	QString result;
	result.reserve( a.count() );

	for( int i = 0; i < a.count(); i++ )
	{
		unsigned char c = a[ i ];
		if( c < 128 )
		{
			result += QLatin1Char( c );
		}
		else
		{
			result += L'\\';
			result += QString::number( c, 8 ).rightJustified( 3, L'0' );
		}
	}

	return result;
}

bool HIGeneratorBase::generate()
{
	mOutFile.setFileName( mFileName );
	if( !mOutFile.open( QFile::WriteOnly ) )
	{
		fprintf( stderr, "Cannot open %s for output.", qPrintable( mFileName ) );
		exit( -1 );
	}
	mOutStream.setDevice( &mOutFile );
	mOutStream.setCodec( "UTF-8" );
	return run();
}


const HIDModel& HIGeneratorBase::model() const
{
	return mModel;
}
