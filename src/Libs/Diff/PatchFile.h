
#ifndef MGV_DIFF_PATCH_FILE_H
#define MGV_DIFF_PATCH_FILE_H

#include <QStringList>

class QTextStream;

class DifferenceHunk;

class PatchFile
{
public:
	PatchFile( const QStringList& pathNames );
	~PatchFile();

public:
	QStringList pathNames() const;
	QList< DifferenceHunk* > allHunks() const;
	void addHunk( DifferenceHunk* hunk );
	int numHunks() const;

	void exportRaw( QTextStream& stream );

	void addOptionLine( const QString& line );
	void addOption( const QString& option );

private:
	QStringList					mPathNames;
	QStringList					mOptions;
	QStringList					mOptionLines;
	QList< DifferenceHunk* >	mHunks;
};

#endif
