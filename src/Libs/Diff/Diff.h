
#ifndef DIFF_DIFF_H
#define DIFF_DIFF_H

#include <QList>
#include <QString>
#include <QStringList>

class DiffLines
{
public:
	DiffLines();

public:
	int numLines() const;
	QString lineAt( int i ) const;
	void addLine( const QString& line );

	int firstLine() const;
	int lastLine() const;

	void setFirstLine( int i );

private:
	int					mFirstLine;
	QStringList			mLines;
};

class Difference
{
public:
	enum Type
	{
		Context,
		Delete,
		Insert,
		Change
	};

public:
	Difference( int numSides, Type t = Context );
	~Difference();

public:
	void setType( Type t );
	Type type() const;

	DiffLines* sideLines( int side );

private:
	Type				mType;
	QList< DiffLines* >	mSides;
};

#endif
