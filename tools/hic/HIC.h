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

#ifndef HIC_H
#define HIC_H

#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QList>

#include "HICObject.h"
#include "HIDLexer.h"

class HeavenInterfaceCompiler : public QCoreApplication
{
public:
	HeavenInterfaceCompiler( int argc, char** argv );

public:
	int run();

private:
	HIDTokenId cur(){ return ( tokenPos < mTokenStream.count() ) ? mTokenStream[ tokenPos ].id : Token_EOF; }
	bool parse();
	bool parseNewObject();
	bool parseProperty();
	bool parseObjectContent();

	void error( const char* pszText, int line );
	void error( const char* pszText );

	bool spitHeader( QTextStream& tsOut );
	bool spitSource( QTextStream& tsOut, const QString& basename );
	void spitSetProperties(QTextStream& tsOut, HICObject* obj,
						   const char* whitespace, const char* prefix );
	HICObjects allObjects( ObjectTypes byType ) const;

private:
	int	tokenPos;

	HICObject* currentObject;
	HICObject* lastCreatedObject;
	HICObject* currentUiObject;
	QHash< QString, HICObject* > objects;

	HIDTokenStream mTokenStream;
};

#endif
