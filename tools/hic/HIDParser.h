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

#ifndef HID_PARSER_H
#define HID_PARSER_H

class HICObject;

class HIDTokenStream;
class HIDModel;

class HIDParser
{
private:
	HIDParser( const HIDTokenStream& tokenStream, HIDModel& model );

public:
	static bool parse( const HIDTokenStream& tokenStream, HIDModel& model );

private:
	bool parse();
	bool parseNewObject();
	bool parseProperty();
	bool parseObjectContent();
	void error( const char* pszText, int line );
	void error( const char* pszText );

private:
	HICObject* currentObject;
	HICObject* lastCreatedObject;
	HICObject* currentUiObject;

	const HIDTokenStream& mTokenStream;
	HIDModel& mModel;
};

#endif
