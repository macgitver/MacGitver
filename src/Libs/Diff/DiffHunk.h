
#ifndef MGV_DIFF_HUNK_H
#define MGV_DIFF_HUNK_H

#include <QList>

class QTextStream;

class Difference;

class DifferenceHunk
{
public:
	DifferenceHunk();
	~DifferenceHunk();

public:
	void addDifference( Difference* difference );
	QList< Difference* > differences() const;

	int numLines( int side ) const;
	int firstLine( int side ) const;

	void exportRaw( QTextStream& stream );

	void setHunkName( const QString& name );
	QString hunkName() const;

private:
	QString					mHunkName;
	QList< Difference* >	mDifferences;
};

#endif
