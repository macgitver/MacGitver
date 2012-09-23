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

#include <QDebug>

#include <stdio.h>

#include "HICObject.h"
#include "HICProperty.h"

#include "HIDParser.h"
#include "HIDToken.h"

HIDParser::HIDParser( const HIDTokenStream& tokenStream, HIDModel& model )
	: mTokenStream( tokenStream )
	, mModel( model )
{
	currentUiObject = lastCreatedObject = currentObject = NULL;
}

void HIDParser::error( const char* pszText )
{
	error( pszText, mTokenStream.curToken().line );
}

void HIDParser::error( const char* pszText, int line )
{
	fprintf( stderr, "Error: %s at about line %i or before\n", pszText, line );
//exit( -1 );
}

bool HIDParser::parseNewObject()
{
	HICObject* oldCurrent = currentObject;

	ObjectTypes t;
	switch( mTokenStream.cur() )
	{
	case Token_Ui:				t = HACO_Ui;			break;
	case Token_Action:			t = HACO_Action;		break;
	case Token_Container:		t = HACO_Container;		break;
	case Token_Menu:			t = HACO_Menu;			break;
	case Token_MenuBar:			t = HACO_MenuBar;		break;
	case Token_MergePlace:		t = HACO_MergePlace;	break;
	case Token_ToolBar:			t = HACO_ToolBar;		break;
	case Token_WidgetAction:	t = HACO_WidgetAction;	break;
	default:
		error( "Expected new object" );
		return false;
	}
	mTokenStream.advance();

	if( mTokenStream.cur() != Token_string )
	{
		error( "Expected object name" );
		return false;
	}

	QString objName = mTokenStream.curValue();

	if( mModel.objects().contains( objName ) )
	{
		currentObject = mModel.objects()[ objName ];
		if( currentObject->type() != t )
		{
			error( "Redeclaration of object with different type" );
			return false;
		}
	}
	else
	{
		currentObject = new HICObject( t );
		currentObject->setName( objName );
		//qDebug() << "Inserting" << objName;
		mModel.objects().insert( objName, currentObject );
		//qDebug() << "Count is now" << mModel.objects().count();
	}

	if( currentObject->type() == HACO_Ui )
	{
		currentUiObject = currentObject;
	}
	else
	{
		if( currentUiObject && !currentUiObject->hasReferenceTo( currentObject ) )
		{
			currentUiObject->addContent( currentObject );
		}
	}

	mTokenStream.advance();

	switch( mTokenStream.cur() )
	{
	case Token_Semicolon:
		mTokenStream.advance();
		lastCreatedObject = currentObject;
		currentObject = oldCurrent;
		return true;

	case Token_OpenCurly:
		mTokenStream.advance();

		if( !parseProperty() )
		{
			error( "Expected Property List" );
			return false;
		}

		if( mTokenStream.cur() != Token_CloseCurly )
		{
			error( "Expected closing curly brace" );
			return false;
		}
		mTokenStream.advance();
		if( mTokenStream.cur() != Token_Semicolon )
		{
			error( "Expected semicolon" );
			return false;
		}

		mTokenStream.advance();

		lastCreatedObject = currentObject;
		currentObject = oldCurrent;
		return true;

	default:
		error( "Expected Properties or semicolon" );
		return false;
	}
}

bool HIDParser::parseObjectContent()
{
	mTokenStream.advance();

	if( mTokenStream.cur() != Token_OpenSquare )
	{
		error( "Expected [" );
		return false;
	}
	mTokenStream.advance();

	do
	{
		switch( mTokenStream.cur() )
		{
		case Token_CloseSquare:
			mTokenStream.advance();
			if( mTokenStream.cur() != Token_Semicolon )
			{
				error( "Expected Semicolon" );
				return false;
			}
			mTokenStream.advance();
			return true;

		case Token_Ui:
		case Token_Action:
		case Token_Container:
		case Token_Menu:
		case Token_MenuBar:
		case Token_MergePlace:
		case Token_ToolBar:
		case Token_WidgetAction:
			if( !parseNewObject() )
			{
				return false;
			}
			if( !currentObject->hasReferenceTo( lastCreatedObject ) )
			{
				currentObject->addContent( lastCreatedObject );
			}
			break;

		case Token_Separator:
			mTokenStream.advance();
			if( mTokenStream.cur() != Token_Semicolon )
			{
				error( "Expected Semicolon" );
				return false;
			}
			mTokenStream.advance();
			currentObject->addContent( new HICObject( HACO_Separator ) );
			break;

		default:
			error( "Expected Ref or new object" );
			return false;
		}

	} while( 1 );
}

bool HIDParser::parseProperty()
{
	do
	{
		switch( mTokenStream.cur() )
		{
		case Token_CloseCurly:
			return true;

		case Token_Ui:
		case Token_Action:
		case Token_Container:
		case Token_Menu:
		case Token_MenuBar:
		case Token_MergePlace:
		case Token_ToolBar:
			if( !parseNewObject() )
			{
				return false;
			}
			if( !currentObject->hasReferenceTo( lastCreatedObject ) )
			{
				currentObject->addContent( lastCreatedObject );
			}
			break;

		case Token_Separator:
			mTokenStream.advance();
			if( mTokenStream.cur() != Token_Semicolon )
			{
				error( "Expected Semicolon" );
				return false;
			}
			mTokenStream.advance();

			currentObject->addContent( new HICObject( HACO_Separator ) );
			break;

		case Token_string:
			{
				QString pname = mTokenStream.curValue();
				mTokenStream.advance();

				switch( mTokenStream.cur() )
				{
				case Token_translateString:
					currentObject->addProperty( pname,
												HICProperty( mTokenStream.curValue(),
															 HICP_TRString ) );
					break;

				case Token_string:
					currentObject->addProperty( pname,
												HICProperty( mTokenStream.curValue(),
															 HICP_String ) );
					break;

				case Token_true:
					currentObject->addProperty( pname,
												HICProperty( true, HICP_Boolean ) );
					break;

				case Token_false:
					currentObject->addProperty( pname,
												HICProperty( false, HICP_Boolean ) );
					break;
				default:
					error( "Expected property value" );
					return false;
				}

				mTokenStream.advance();
				if( mTokenStream.cur() != Token_Semicolon )
				{
					error( "Expected Semicolon" );
					return false;
				}
				mTokenStream.advance();
			}
			break;

		case Token_Content:
			if( !parseObjectContent() )
			{
				return false;
			}
			break;

		default:
			error( "Expected }, Property-Assignment or content list" );
			return false;
		}
	} while( 1 );
}

bool HIDParser::parse( const HIDTokenStream& tokenStream, HIDModel& model )
{
	HIDParser parser( tokenStream, model );
	return parser.parse();
}

bool HIDParser::parse()
{
	while( !mTokenStream.endOfStream() )
	{
		if( mTokenStream.cur() == Token_EOF )
			break;

		if( !parseNewObject() )
		{
			return false;
		}
	}

	return true;
}
