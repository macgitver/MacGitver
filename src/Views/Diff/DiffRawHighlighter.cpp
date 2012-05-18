
#include "Views/Diff/DiffRawHighlighter.h"


DiffRawHighlighter::DiffRawHighlighter( QTextEdit* editor )
	: QSyntaxHighlighter( editor )
{
}

void DiffRawHighlighter::highlightBlock( const QString& text )
{
	QColor c = Qt::black;

	if( text.startsWith( "diff" ) || text.startsWith( "---" || text.startsWith( "+++" ) ) )
	{
		c = Qt::darkGreen;
	}
	else if( text.startsWith( '-' ) )
	{
		c = Qt::darkRed;
	}
	else if( text.startsWith( '+' ) )
	{
		c = Qt::darkCyan;
	}
	else if( text.startsWith( ' ' ) )
	{
		c = Qt::gray;
	}
	else if( text.startsWith( "@@" ) )
	{
		int i = text.indexOf( "@@", 2 );
		i += 2;
		setFormat( 0, i, Qt::darkYellow );
		setFormat( i, text.length() - i, Qt::blue );
		return;
	}

	setFormat( 0, text.length(), c );
}
