
#ifndef HISTORY_BUILDER_H
#define HISTORY_BUILDER_H

#include <QMap>

#include "Libs/Git/ObjectCommit.h"
#include "Libs/Git/Repository.h"
#include "Libs/Git/RevisionWalker.h"

class HistoryEntries;

class HistoryBuilder
{
public:
	HistoryBuilder( Git::Repository repo, HistoryEntries* entries );
	~HistoryBuilder();

public:
	void addHEAD();
	void addAllRefs();

public:
	void start();

private:
	Git::Repository				mRepo;
	Git::RevisionWalker			mWalker;
	HistoryEntries*				mEntries;
};

#endif
