
#ifndef DIFF_DIFF_H
#define DIFF_DIFF_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QByteArray>

class QIODevice;

enum DiffLineType
{
	DiffLineContext,
	DiffLineDeletion,
	DiffLineAdditionBase,
	DiffLineAddition = DiffLineAdditionBase,
	DiffLineAddition1 = DiffLineAdditionBase + 1,
	DiffLineAddition2 = DiffLineAdditionBase + 2,
	DiffLineAddition3 = DiffLineAdditionBase + 3,
	DiffLineAddition4 = DiffLineAdditionBase + 4,
	DiffLineAddition5 = DiffLineAdditionBase + 5
};

class DifferenceLine
{
public:
	DifferenceLine( const QByteArray& ba, DiffLineType type );

public:
	bool isContext(){ return mType == DiffLineContext; }
	bool isDeletion(){ return mType == DiffLineDeletion; }
	bool isAddition(){ return !isContext() && !isDeletion(); }

	QByteArray text() const{ return mText; }

private:
	QByteArray		mText;
	DiffLineType	mType;
};

class DifferenceHunkSide
{
public:
	DifferenceHunkSide();
	~DifferenceHunkSide();

public:
	void addLine( DifferenceLine* line );
	int numLines() const;
	DifferenceLine* line( int index );
	int startingLineNumber() const;
	void setStartingLine( quint32 startLineNumber );

private:
	int							mStartLineNumber;
	QList< DifferenceLine* >	mLines;
};

class DifferenceHunk
{
public:
	DifferenceHunk( int sides );
	~DifferenceHunk();

public:
	DifferenceHunkSide* side( int i );
	const DifferenceHunkSide* side( int i ) const;

	int maximumDisplayLines() const;

private:
	int							mNumSides;
	DifferenceHunkSide*			mSideHunks;
};

class Difference
{
public:
	Difference( const QStringList& pathNames );
	~Difference();

public:
	QStringList pathNames() const;
	QList< DifferenceHunk* > allHunks() const;
	void addHunk( DifferenceHunk* hunk );
	int numHunks() const;

	int maximumDisplayLines() const;

private:
	QStringList					mPathNames;
	QList< DifferenceHunk* >	mHunks;
};

class Differences
{
public:
	Differences();
	~Differences();

public:
	void addPath( Difference* diff );
	QList< Difference* > allPaths() const;
	int numPaths() const;
	Difference* pathAt( int index );

	static Differences* readPatch( const QString& fileName );
	static Differences* readPatch( QIODevice* device );

private:
	QList< Difference* >		mPaths;
};

#endif
