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

#include "WriteClassSource.hpp"

QString utf8Encoded( QString str )
{
	return str;
}

WriteClassSource::WriteClassSource( const QString& outFile, const QString& headerName,
									const ConfigSection& section )
	: mOutFileName( outFile )
	, mOutFile( outFile )
	, mSection( section )
	, mHeaderName( headerName )
{
	if( !mOutFile.open( QFile::WriteOnly ) )
	{
		/**/
	}

	mOutStream.setDevice( &mOutFile );
}

void WriteClassSource::generate()
{
	mOutStream << "\n"
				  "#include \"" << mHeaderName << "\"\n"
				  "\n"
			   << mSection.className() << "::" << mSection.className() << "( QObject* parent )\n"
				  "\t: QObject( parent )\n"
				  "\t, ConfigUser( \"" << mSection.configPath() << "\" )\n";

	foreach( ConfigSetting* setting, mSection.settings() )
	{
		QString defaultValue = setting->defaultValue();
		if( defaultValue.isEmpty() )
		{
			continue;
		}

		mOutStream << "\t, mValue" << setting->name() << "( ";
		if( setting->typeName() == QLatin1String( "String" ) )
		{
			mOutStream << "QString::fromUtf8( \"" << utf8Encoded( defaultValue ) << "\" )";
		}
		else
		{
			mOutStream << defaultValue;
		}
		mOutStream << " )\n";
	}

	mOutStream << "{\n"
				  "\tread();\n"
				  "}\n"
				  "\n"
				  "void " << mSection.className() << "::read()\n"
				  "{\n";

	foreach( ConfigSetting* setting, mSection.settings() )
	{

	}

	mOutStream << "}\n"
				  "\n";
}
