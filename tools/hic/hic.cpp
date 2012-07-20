
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QUuid>

#include "hic.h"

QHash< QString, HIDTokenId > HeavenInterfaceCompiler::sTokens;

/*
 * HADFile				:= RootObjectDef
 *
 * RootObjectDef		:= Keyword_Ui <identifer> '{' PropertyList '}' ';'
 *						:= ObjectFwdDef RootObjectDef;
 *
 * ObjectDef			:= ObjectFwdDef
 *						:= RealObjectDef
 *
 * RealObjectDef		:= ClassName <identifier> '{' PropertyList '}' ';'
 *
 * ObjectFwdDef			:= ClassName <identifier> ';'
 *
 * PropertyList			:= PropertyListEntry
 *						:= PropertyListEntry PropertyList
 *
 * PropertyListEntry	:= PropertyDef
 *						:= ObjectDef
 *						:= ContentList
 *						:= ContentRef
 *
 * PropertyDef			:= <identifier> PropertyValue ';'
 *
 * PropertyValue		:= Keyword_True
 *						:= Keyword_False
 *						:= <identifier>
 *						:= TranslatedString
 *
 * TranslatedString		:= '"' AnyChars '"'
 *
 * ContentList			:= Keyword_Content '[' ContentRefs ']'
 *
 * ContentRefs			:= ContentRef
 *						:= ContentRefs ContentRef
 *
 * ContentRef			:= ObjectDef
 *						:= Separator
 *
 * ClassName			:= Keyword_Action
 *						:= Keyword_Menu
 *						:= Keyword_MenuBar
 *						:= Keyword_ToolBar
 *						:= Keyword_MergePoint
 *						:= Keyword_Container
 *
 * Separator			:= Keyword_Separator ';'
 *
 * Keyword_True			:= "true"
 * Keyword_False		:= "false"
 * Keyword_Content		:= "Content"
 * Keyword_Ui			:= "Ui"
 * Keyword_Action		:= "Action"
 * Keyword_Menu			:= "Menu"
 * Keyword_MenuBar		:= "MenuBar"
 * Keyword_ToolBar		:= "ToolBar"
 * Keyword_Separator	:= "Sep"
 *						:= "Separator"
 * Keyword_MergePlace	:= "MergePlace"
 * Keyword_Container	:= "Container"
 *
 * AnyChars				:= >> Every Char other than " <<
 *
 */

static inline QString latin1Encode( const QString& src )
{
	return src;
}

static inline QString utf8Encode( const QString& src )
{
	QByteArray a = src.toUtf8();
	QString result;
	result.reserve( a.count() );

	for( int i = 0; i < a.count(); i++ )
	{
		unsigned char c = a[ i ];
		if( c < 128 )
		{
			result += c;
		}
		else
		{
			result += '\\';
			result += QString::number( c, 8 ).rightJustified( 3, L'0' );
		}
	}

	return result;
}

HICObjects HICObjects::byType( ObjectTypes type ) const
{
	HICObjects result;

	foreach( HICObject* obj, *this )
	{
		if( obj->type() == type )
		{
			result.append( obj );
		}
	}

	return result;
}

HeavenInterfaceCompiler::HeavenInterfaceCompiler( int argc , char** argv )
	: QCoreApplication( argc, argv )
{
	QStringList sl = arguments();

	if( sTokens.count() == 0 )
	{
		#define T(t) sTokens[ #t ] = Token_##t
		#define T2(t1,t2) sTokens[ #t1 ] = Token_##t2
		T(Ui);
		T(Action);
		T(Menu);
		T(MenuBar);
		T(ToolBar);
		T(Separator);
		T(Container);
		T(MergePlace);
		T(WidgetAction);
		T2(Sep,Separator);
		T(Content);
		T(true);
		T(false);
		#undef T1
		#undef T2
	}

	currentUiObject = lastCreatedObject = currentObject = NULL;
}

bool HeavenInterfaceCompiler::parseNewObject()
{
	HICObject* oldCurrent = currentObject;

	ObjectTypes t;
	switch( tokens[ tokenPos ].id )
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
	tokenPos++;

	if( tokens[ tokenPos ].id != Token_string )
	{
		error( "Expected object name" );
		return false;
	}

	QString oname = tokens[ tokenPos ].value;

	if( objects.contains( oname ) )
	{
		currentObject = objects[ oname ];
		if( currentObject->type() != t )
		{
			error( "Redeclaration of object with different type" );
			return false;
		}
	}
	else
	{
		currentObject = new HICObject( t );
		currentObject->setName( oname );
		objects[ oname ] = currentObject;
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

	tokenPos++;

	switch( tokens[ tokenPos ].id )
	{
	case Token_Semicolon:
		tokenPos++;
		lastCreatedObject = currentObject;
		currentObject = oldCurrent;
		return true;

	case Token_OpenCurly:
		tokenPos++;

		if( !parseProperty() )
		{
			error( "Expected Property List" );
			return false;
		}

		if( tokens[ tokenPos ].id != Token_CloseCurly )
		{
			error( "Expected closing curly brace" );
			return false;
		}
		tokenPos++;
		if( tokens[ tokenPos ].id != Token_Semicolon )
		{
			error( "Expected semicolon" );
			return false;
		}
		tokenPos++;

		lastCreatedObject = currentObject;
		currentObject = oldCurrent;
		return true;

	default:
		error( "Expected Properties or semicolon" );
		return false;
	}
}

bool HeavenInterfaceCompiler::parseObjectContent()
{
	tokenPos++;

	if( tokens[ tokenPos ].id != Token_OpenSquare )
	{
		error( "Expected [" );
		return false;
	}
	tokenPos++;

	do
	{
		switch( tokens[ tokenPos ].id )
		{
		case Token_CloseSquare:
			tokenPos++;
			if( tokens[ tokenPos ].id != Token_Semicolon )
			{
				error( "Expected Semicolon" );
				return false;
			}
			tokenPos++;
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
			tokenPos++;
			if( tokens[ tokenPos ].id != Token_Semicolon )
			{
				error( "Expected Semicolon" );
				return false;
			}
			tokenPos++;
			currentObject->addContent( new HICObject( HACO_Separator ) );
			break;

		default:
			error( "Expected Ref or new object" );
			return false;
		}

	} while( 1 );
}

bool HeavenInterfaceCompiler::parseProperty()
{
	do
	{
		switch( tokens[ tokenPos ].id )
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
			tokenPos++;
			if( tokens[ tokenPos ].id != Token_Semicolon )
			{
				error( "Expected Semicolon" );
				return false;
			}
			tokenPos++;
			currentObject->addContent( new HICObject( HACO_Separator ) );
			break;

		case Token_string:
			{
				QString pname = tokens[ tokenPos++ ].value;
				switch( tokens[ tokenPos ].id )
				{
				case Token_translateString:
					currentObject->addProperty( pname,
								HICProperty( tokens[ tokenPos ].value, HICP_TRString ) );
					break;

				case Token_string:
					currentObject->addProperty( pname,
								HICProperty( tokens[ tokenPos ].value, HICP_String ) );
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
				tokenPos++;
				if( tokens[ tokenPos ].id != Token_Semicolon )
				{
					error( "Expected Semicolon" );
					return false;
				}
				tokenPos++;
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

void HeavenInterfaceCompiler::error(const char *pszText )
{
	error( pszText, tokens[ tokenPos ].line );
}

void HeavenInterfaceCompiler::error(const char *pszText, int line)
{
	fprintf( stderr, "Error: %s at about line %i or before\n", pszText, line );
}

bool HeavenInterfaceCompiler::parse()
{
	tokenPos = 0;
	while( tokenPos < tokens.count() )
	{
		if( tokens[ tokenPos ].id == Token_EOF )
			break;

		if( !parseNewObject() )
		{
			return false;
		}
	}

	return true;
}

void HeavenInterfaceCompiler::flush( int line )
{
	if( currentText.count() )
	{
		HIDToken t;
		if( sTokens.contains( currentText ) )
		{
			t.id = sTokens[ currentText ];
		}
		else
		{
			t.id = Token_string;
			t.value = currentText;
		}
		t.line = line;
		tokens.append( t );
		currentText = QString();
	}
}

bool HeavenInterfaceCompiler::tokenize( const QString& text )
{
	int line = 1;
	int pos = 0;

	while( pos < text.count() )
	{
		Next:
		QChar c = text[ pos++ ];
		switch( c.unicode() )
		{
		case L'\n': flush( line++ ); continue;
		case L' ':
		case L'\t':
		case L'\r':
			flush( line );
			continue;

		case L'{': { flush( line ); HIDToken t; t.line = line; t.id = Token_OpenCurly; tokens.append(t); continue; }
		case L'}': { flush( line ); HIDToken t; t.line = line; t.id = Token_CloseCurly; tokens.append(t); continue; }
		case L'[': { flush( line ); HIDToken t; t.line = line; t.id = Token_OpenSquare; tokens.append(t); continue; }
		case L']': { flush( line ); HIDToken t; t.line = line; t.id = Token_CloseSquare; tokens.append(t); continue; }
		case L';': { flush( line ); HIDToken t; t.line = line; t.id = Token_Semicolon; tokens.append(t); continue; }
		case L',': { flush( line ); HIDToken t; t.line = line; t.id = Token_Comma; tokens.append(t); continue; }

		case L'/':
			flush( line );
			if( text[ pos ] == L'/' )
			{
				while( text[ pos ] != L'\n' )
					if( ++pos == text.count() )
						return true;	// actually, it's a successful lexer run, if we encounter
										// EOF in a one-line-comment
				line++;
				continue;
			}

			if( text[ pos ] == L'*' )
			{
				pos++;
				while( pos < text.count() )
				{
					switch( text[ pos ].unicode() )
					{
					case L'*':
						pos++;
						if( text[ pos ] == L'/' )
						{
							pos++;
							goto Next;
						}
						continue;

					case L'\n':
						line++;
					default:
						pos++;
						continue;
					}
				}
			}
			return false;

		case L'"':
			{
				flush( line );
				QString s;
				while( text[ pos ] != L'"' )
				{
					s += text[ pos++ ];
					if( pos == text.count() )
					{
						return false;
					}
				}
				pos++;

				HIDToken t;
				t.id = Token_translateString;
				t.value = s;
				t.line = line;
				tokens.append( t );
			}
			continue;

		default:
			currentText += c;
			break;
		}
	}

	flush( line );

	HIDToken tEof; tEof.id = Token_EOF; tokens.append( tEof );

#if 0
	foreach( HADToken t, tokens )
	{
		switch( t.id )
		{
		case Token_string: printf( "'%s'\n", qPrintable( t.value ) ); break;
		case Token_translateString: printf( "\"%s\"\n", qPrintable( t.value ) ); break;
		default:
			printf( "T(%i)\n", int(t.id) );
			break;
		}
	}
#endif
	return true;
}

HICObjects HeavenInterfaceCompiler::allObjects( ObjectTypes byType ) const
{
	HICObjects result;

	foreach( HICObject* obj, objects )
	{
		if( obj->type() == byType )
		{
			result.append( obj );
		}
	}

	return result;
}

void HeavenInterfaceCompiler::spitSetProperties( QTextStream& tsOut, HICObject* obj,
												 const char* whitespace,
												 const char* prefix )
{
	foreach( QString pname, obj->propertyNames() )
	{
		if( pname.startsWith( L'_' ) )
		{
			continue;
		}
		HICProperty p = obj->getProperty( pname );

		tsOut << whitespace << prefix << obj->name() << "->set" << pname << "( ";

		switch( p.type() )
		{
		case HICP_String:
			tsOut << "QLatin1String( \"" << latin1Encode( p.value().toString() ) << "\" )";
			break;

		case HICP_TRString:
			tsOut << "trUtf8( \"" << utf8Encode( p.value().toString() ) << "\" )";
			break;

		case HICP_Boolean:
			tsOut << ( p.value().toBool() ? "true" : "false" );
			break;

		default:
			tsOut << "WTF?";
		}

		tsOut << " );\n";
	}

	switch( obj->type() )
	{
	case HACO_Action:
		if( obj->hasProperty( "_ConnectTo", HICP_String ) )
		{
			HICProperty p = obj->getProperty( "_ConnectTo" );

			QString slot = p.value().toString();
			const char* signal = "triggered()";
			if( slot.contains( "(bool)" ) )
			{
				signal = "toggled(bool)";
			}

			QByteArray receiver = "parent";
			if( obj->hasProperty( "_ConnectContext", HICP_String ) )
			{
				HICProperty p2 = obj->getProperty( "_ConnectContext" );
				receiver = p2.value().toString().toLocal8Bit();
			}

			tsOut << whitespace << "QObject::connect( " << prefix << obj->name() << ", SIGNAL(" << signal
				  << "), " << receiver << ", SLOT(" << slot << ") );\n";
		}
		break;

	default:
		break;
	}

}

bool HeavenInterfaceCompiler::spitHeader( QTextStream& tsOut )
{
	QUuid id = QUuid::createUuid();
	QString idstr = id
					.toString()
					.toUpper()
					.replace( "{", "" )
					.replace( "}", "" )
					.replace( "-", "_" );

	tsOut << "/**********************************************************************************\n"
			 "*\n"
			 "* This file is generated by HIC, the Heaven Interface Compiler\n"
			 "*\n"
			 "* Any modifications will be lost on the next gererator run. You've been warned.\n"
			 "*\n"
			 "* " << QDateTime::currentDateTime().toString( Qt::ISODate ) << "\n"
			 "*\n"
			 "**********************************************************************************/\n"
			 "\n"
			 "#ifndef HIC_" << idstr << "\n"
			 "#define HIC_" << idstr << "\n\n"
			 "class QObject;\n"
			 "\n"
			 "#include \"Heaven/Actions/Action.h\"\n"
			 "#include \"Heaven/Actions/WidgetAction.h\"\n"
			 "#include \"Heaven/Actions/Menu.h\"\n"
			 "#include \"Heaven/Actions/MenuBar.h\"\n"
			 "#include \"Heaven/Actions/ToolBar.h\"\n"
			 "#include \"Heaven/Actions/MergePlace.h\"\n"
			 "#include \"Heaven/Actions/ActionContainer.h\"\n"
			 "\n";

	foreach( HICObject* uiObject, allObjects( HACO_Ui ) )
	{
		tsOut << "#ifndef HIC_" << uiObject->name() << "\n"
				 "#define HIC_" << uiObject->name() << "\n\n"

				 "class " << uiObject->name() << "\n"
				 "{\n"
				 "public:\n"
				 "\tvoid setupActions( QObject* parent );\n"
				 "\n"
				 "private:\n"
				 "\tstatic QString trUtf8( const char* sourceText );\n"
				 "\n"
				 "public:\n";

		foreach( HICObject* object, uiObject->content( HACO_Action ) )
		{
			tsOut << "\tHeaven::Action*          act" << object->name() << ";\n";
		}

		foreach( HICObject* object, uiObject->content( HACO_WidgetAction ) )
		{
			tsOut << "\tHeaven::WidgetAction*    wac" << object->name() << ";\n";
		}

		foreach( HICObject* object, uiObject->content( HACO_MergePlace ) )
		{
			tsOut << "\tHeaven::MergePlace*      mp" << object->name() << ";\n";
		}

		foreach( HICObject* object, uiObject->content( HACO_Menu ) )
		{
			tsOut << "\tHeaven::Menu*            menu" << object->name() << ";\n";
		}

		foreach( HICObject* object, uiObject->content( HACO_MenuBar ) )
		{
			tsOut << "\tHeaven::MenuBar*         mb" << object->name() << ";\n";
		}

		foreach( HICObject* object, uiObject->content( HACO_ToolBar ) )
		{
			tsOut << "\tHeaven::ToolBar*         tb" << object->name() << ";\n";
		}

		foreach( HICObject* object, uiObject->content( HACO_Container ) )
		{
			tsOut << "\tHeaven::ActionContainer* ac" << object->name() << ";\n";
		}

		tsOut << "};\n\n";

		tsOut << "#endif\n\n";

		tsOut << "#endif\n\n";
	}

	return true;
}

bool HeavenInterfaceCompiler::spitSource( QTextStream& tsOut, const QString& baseName )
{
	tsOut << "/**********************************************************************************\n"
			 "*\n"
			 "* This file is generated by HIC, the Heaven Interface Compiler\n"
			 "*\n"
			 "* Any modifications will be lost on the next gererator run. You've been warned.\n"
			 "*\n"
			 "* " << QDateTime::currentDateTime().toString( Qt::ISODate ) << "\n"
			 "*\n"
			 "**********************************************************************************/\n"
			 "\n"
			 "#include <QApplication>\n"
			 "#include <QObject>\n"
			 "\n"
			 "#include \"" << baseName << "\"\n\n";

	foreach( HICObject* uiObject, allObjects( HACO_Ui ) )
	{
		QString ctx;

		if( uiObject->hasProperty( "TrContext", HICP_String ) )
		{
			ctx = uiObject->getProperty( "TrContext" ).value().toString();
		}

		if( ctx.isEmpty() )
		{
			ctx = uiObject->name();
		}

		tsOut << "QString "<< uiObject->name() << "::" << "trUtf8( const char* sourceText )\n"
				 "{\n"
				 "\treturn QApplication::translate( \"" << latin1Encode( ctx ) << "\", sourceText, "
					"NULL, QCoreApplication::UnicodeUTF8 );\n"
				 "}\n"
				 "\n";

		tsOut << "void " << uiObject->name() << "::" << "setupActions( QObject* parent )\n"
				 "{\n"
				 "\t//Setup the actions\n\n";

		foreach( HICObject* actionObject, uiObject->content( HACO_Action ) )
		{
			tsOut << "\tact" << actionObject->name() << " = new Heaven::Action( parent );\n";
			spitSetProperties( tsOut, actionObject, "\t", "act" );
			tsOut << "\n";
		}

		foreach( HICObject* actionObject, uiObject->content( HACO_WidgetAction ) )
		{
			tsOut << "\twac" << actionObject->name() << " = new Heaven::WidgetAction( parent );\n";
			spitSetProperties( tsOut, actionObject, "\t", "wac" );
			tsOut << "\n";
		}

		tsOut << "\t//Setup Mergeplaces\n\n";
		foreach( HICObject* mpObject, uiObject->content( HACO_MergePlace ) )
		{
			tsOut << "\tmp" << mpObject->name() << " = new Heaven::MergePlace( parent );\n"
					 "\tmp" << mpObject->name() << "->setName( QByteArray( \"" <<
					 latin1Encode( mpObject->name() ) << "\" ) );\n";
			spitSetProperties( tsOut, mpObject, "\t", "mp" );
			tsOut << "\n";
		}

		tsOut << "\t//Setup containers\n\n";
		foreach( HICObject* menuObject, uiObject->content( HACO_Menu ) )
		{
			tsOut << "\tmenu" << menuObject->name() << " = new Heaven::Menu( parent );\n";
			spitSetProperties( tsOut, menuObject, "\t", "menu" );
			tsOut << "\n";
		}
		foreach( HICObject* menuObject, uiObject->content( HACO_MenuBar ) )
		{
			tsOut << "\tmb" << menuObject->name() << " = new Heaven::MenuBar( parent );\n";
			spitSetProperties( tsOut, menuObject, "\t", "mb" );
			tsOut << "\n";
		}
		foreach( HICObject* menuObject, uiObject->content( HACO_ToolBar ) )
		{
			tsOut << "\ttb" << menuObject->name() << " = new Heaven::ToolBar( parent );\n";
			spitSetProperties( tsOut, menuObject, "\t", "tb" );
			tsOut << "\n";
		}
		foreach( HICObject* menuObject, uiObject->content( HACO_Container ) )
		{
			tsOut << "\tac" << menuObject->name() << " = new Heaven::ActionContainer( parent );\n";
			spitSetProperties( tsOut, menuObject, "\t", "ac" );
			tsOut << "\n";
		}

		tsOut << "\t//Give containers some content\n\n";

		foreach( HICObject* object, uiObject->content() )
		{
			const char* prefix = "";
			switch( object->type() )
			{
			case HACO_Action:
			case HACO_MergePlace:
			case HACO_Ui:
			case HACO_Separator:
			case HACO_WidgetAction:
				continue;

			case HACO_Menu:
				prefix = "\tmenu";
				break;

			case HACO_MenuBar:
				prefix = "\tmb";
				break;

			case HACO_ToolBar:
				prefix = "\ttb";
				break;

			case HACO_Container:
				prefix = "\tac";
				break;
			}

			foreach( HICObject* child, object->content() )
			{
				switch( child->type() )
				{
				case HACO_Separator:
					tsOut << prefix << object->name() << "->addSeparator();\n";
					break;

				case HACO_Action:
					tsOut << prefix << object->name() << "->add( act" << child->name() << " );\n";
					break;

				case HACO_WidgetAction:
					tsOut << prefix << object->name() << "->add( wac" << child->name() << " );\n";
					break;

				case HACO_Container:
					tsOut << prefix << object->name() << "->add( ac" << child->name() << " );\n";
					break;

				case HACO_MergePlace:
					tsOut << prefix << object->name() << "->add( mp" << child->name() << " );\n";
					break;

				case HACO_Menu:
					tsOut << prefix << object->name() << "->add( menu" << child->name() << " );\n";
					break;

				default:
					tsOut << "\t\tWTF?";
				}
			}

			tsOut << "\n";
		}

		tsOut << "}\n\n";
	}

	return true;
}

int HeavenInterfaceCompiler::run()
{
	QStringList sl = arguments();

	if( sl.count() != 4 )
	{
		fprintf( stderr, "Usage: %s <input> <output-header> <output-source\n",
				 qPrintable( sl[ 0 ] ) );
		return -1;
	}

	QFile fInput( sl[ 1 ] );
	if( !fInput.open( QFile::ReadOnly ) )
	{
		fprintf( stderr, "Cannot read from %s\n", qPrintable( sl[ 1 ] ) );
		return -1;
	}
	QTextStream tsInput( &fInput );
	tsInput.setCodec( "UTF-8" );
	if( !tokenize( tsInput.readAll() ) )
	{
		fprintf( stderr, "Could not tokenize input from %s\n", qPrintable( sl[ 1 ] ) );
		return -1;
	}

	if( !parse() )
	{
		fprintf( stderr, "Could not parse input from %s\n", qPrintable( sl[ 1 ] ) );
		return -1;
	}

	QFile fOutput1( sl[ 2 ] );
	if( !fOutput1.open( QFile::WriteOnly ) )
	{
		fprintf( stderr, "Could not open %s for output\n", qPrintable( sl[ 2 ] ) );
		return -1;
	}

	QTextStream tsOutput1( &fOutput1 );
	spitHeader( tsOutput1 );

	QFile fOutput2( sl[ 3 ] );
	if( !fOutput2.open( QFile::WriteOnly ) )
	{
		fprintf( stderr, "Could not open %s for output\n", qPrintable( sl[ 3 ] ) );
		return -1;
	}

	QTextStream tsOutput2( &fOutput2 );
	spitSource( tsOutput2, QFileInfo( fOutput1 ).fileName() );

	return 0;
}

int main( int c, char** v )
{
	return HeavenInterfaceCompiler( c, v ).run();
}
