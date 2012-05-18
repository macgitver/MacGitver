
#ifndef MGV_DIFF_HUNK_H
#define MGV_DIFF_HUNK_H

#include <QList>

class Difference;

class DifferenceHunk
{
public:
	DifferenceHunk();
	~DifferenceHunk();

public:
	void addDifference( Difference* difference );
	QList< Difference* > differences() const;

private:
	QList< Difference* >	mDifferences;
};

#endif
