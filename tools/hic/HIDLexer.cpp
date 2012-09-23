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

#include <QTextStream>

#include "HIDLexer.h"

QHash< QString, HIDTokenId > HIDLexer::sTokens;

bool HIDLexer::lex( QIODevice& fInput, HIDTokenStream& stream )
{
	HIDLexer lexer( stream );
	QTextStream tsInput( &fInput );
	tsInput.setCodec( "UTF-8" );

	return lexer.tokenize( tsInput.readAll() );
}

HIDLexer::HIDLexer( HIDTokenStream& stream )
	: mOutStream( stream )
{
	if( sTokens.count() == 0 )
	{
		#define T(t) sTokens[ QLatin1String( #t ) ] = Token_##t
		#define T2(t1,t2) sTokens[ QLatin1String( #t1 ) ] = Token_##t2
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
}

bool HIDLexer::tokenize( const QString& text )
{
	HIDToken nextToken;
	int line = 1;
	int pos = 0;

	while( pos < text.count() )
	{
		Next:
		QChar c = text[ pos++ ];
		switch( c.unicode() )
		{
		case L'\n':
			flush( line++ );
			continue;

		case L' ':
		case L'\t':
		case L'\r':
			flush( line );
			continue;

		case L'{':
			flush( line );
			nextToken.line = line;
			nextToken.id = Token_OpenCurly;
			mOutStream.append( nextToken );
			continue;

		case L'}':
			flush( line );
			nextToken.line = line;
			nextToken.id = Token_CloseCurly;
			mOutStream.append( nextToken );
			continue;

		case L'[':
			flush( line );
			nextToken.line = line;
			nextToken.id = Token_OpenSquare;
			mOutStream.append( nextToken );
			continue;

		case L']':
			flush( line );
			nextToken.line = line;
			nextToken.id = Token_CloseSquare;
			mOutStream.append( nextToken );
			continue;

		case L';':
			flush( line );
			nextToken.line = line;
			nextToken.id = Token_Semicolon;
			mOutStream.append( nextToken );
			continue;

		case L',':
			flush( line );
			nextToken.line = line;
			nextToken.id = Token_Comma;
			mOutStream.append( nextToken );
			continue;

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
				mOutStream.append( t );
			}
			continue;

		default:
			currentText += c;
			break;
		}
	}

	flush( line );

	nextToken.id = Token_EOF;
	mOutStream.append( nextToken );

#if 0
	foreach( HIDToken t, mOutStream )
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

void HIDLexer::flush( int line )
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
		mOutStream.append( t );
		currentText = QString();
	}
}

