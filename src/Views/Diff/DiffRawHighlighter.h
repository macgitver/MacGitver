
#ifndef MGV_DIFF_RAW_HIGHLIGHTER_H
#define MGV_DIFF_RAW_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class DiffRawHighlighter : public QSyntaxHighlighter
{
public:
	DiffRawHighlighter( QTextEdit* editor );

protected:
	void highlightBlock( const QString& text );
};

#endif
