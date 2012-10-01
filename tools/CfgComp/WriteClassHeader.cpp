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

#include "WriteClassHeader.hpp"

WriteClassHeader::WriteClassHeader( const QString& outFile, const ConfigSection& section )
	: mOutFileName( outFile )
	, mOutFile( outFile )
	, mSection( section )
{
	if( !mOutFile.open( QFile::WriteOnly ) )
	{
		/**/
	}

	mOutStream.setDevice( &mOutFile );
}

void WriteClassHeader::generate()
{
	QString protector = QLatin1String( "CFGCOMP_" ) + mSection.className().toUpper() +
			QLatin1String( "_HPP" );

	mOutStream << "\n"
				  "#ifndef " << protector << "\n"
				  "#define " << protector << "\n"
				  "\n"
				  "#include <QVariant>\n"
				  "#include <QObject>\n"
				  "\n"
				  "#include \"Config/ConfigUser.h\"\n"
				  "\n"
				  "class " << mSection.className() << " : public QObject, private ConfigUser\n"
				  "{\n"
				  "\tQ_OBJECT\n"
				  "public:\n"
				  "\t" << mSection.className() << "( QObject* parent = 0 );\n"
				  "\t~" << mSection.className() << "();\n"
				  "\n"
				  "private:\n"
				  "\tvoid read();\n"
				  "\n"
				  "public:\n";

	foreach( ConfigSetting* setting, mSection.settings() )
	{
		QString getName = setting->name();
		getName[ 0 ] = getName[ 0 ].toLower();

		mOutStream << "\tstatic " << setting->type().cppType() << " " << getName << "();\n"
					  "\tstatic void set" << setting->name() << "( " << setting->type().cppType()
				   << " value );\n";

	}

	mOutStream << "\n"
				  "private:\n"
				  "\tstatic " << mSection.className() << "* sSelf;\n\n";

	foreach( ConfigSetting* setting, mSection.settings() )
	{
		mOutStream << "\t" << setting->type().cppType() << " mValue" << setting->name() << ";\n";
	}

	mOutStream << "};\n";

	mOutStream << "\n"
				  "#endif\n"
				  "\n";
}
