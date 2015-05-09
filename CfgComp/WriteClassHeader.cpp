/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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
    QString protector = QStringLiteral( "CFGCOMP_" ) + mSection.className().toUpper() +
            QStringLiteral( "_HPP" );

    mOutStream << "\n"
                  "#ifndef " << protector << "\n"
                  "#define " << protector << "\n"
                  "\n"
                  "#include <QObject>\n"
                  "\n"
                  "class QVariant;\n"
                  "\n"
                  "#include \"libMacGitverCore/Config/ConfigUser.h\"\n"
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
                  "\n";

    foreach( ConfigSetting* setting, mSection.allSettings() )
    {
        QString getName = setting->fullName();
        getName[ 0 ] = getName[ 0 ].toLower();

        mOutStream << "public:\t// " << mSection.configPath() << "/" << setting->fullPath() << "\n"
                      "\tstatic " << setting->type().cppType() << " " << getName << "();\n"
                      "\tstatic void set" << setting->fullName() << "( " << setting->type().cppType()
                   << " value );\n";

        if( setting->emitSignal() )
        {
            mOutStream << "signals:\n"
                          "\tvoid " << getName << "Changed( " << setting->type().cppType()
                       << " newValue );\n";
        }

        mOutStream << "\n";
    }

    mOutStream << "protected:\n"
                  "\tvoid configChanged( const QString& subPath, const QVariant& value );\n";

    mOutStream << "\n"
                  "private:\n"
                  "\tstatic " << mSection.className() << "* sSelf;\n\n";

    foreach( ConfigSetting* setting, mSection.allSettings() )
    {
        mOutStream << "\t" << setting->type().cppType() << " mValue" << setting->fullName() << ";\n";
    }

    mOutStream << "};\n";

    mOutStream << "\n"
                  "#endif\n"
                  "\n";
}
