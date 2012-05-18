
#ifndef MGV_DIFF_PATCH_H
#define MGV_DIFF_PATCH_H

#include <QList>
class QString;
class QIODevice;
class QTextStream;

class PatchFile;

class Patch
{
public:
	Patch();
	~Patch();

public:
	void addPath( PatchFile* diff );
	QList< PatchFile* > allPaths() const;
	int numPaths() const;
	PatchFile* pathAt( int index );

	void setNumSides( int sides );
	int numSides() const;

	static Patch* readPatch( const QString& fileName );
	static Patch* readPatch( QIODevice* device );

	void exportRaw( QTextStream& stream );

	QString toString();

private:
	int					mNumSides;
	QList< PatchFile* >	mPaths;
};

#endif
