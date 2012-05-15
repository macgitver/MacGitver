
#ifndef MGV_MODULES_H
#define MGV_MODULES_H

#include <QSet>

#include "Libs/Core/Module.h"

class Modules : public QObject
{
	Q_OBJECT
public:
	Modules( QObject* parent );
	~Modules();

public:
	void addModule( Module* mod );
	void delModule( Module* mod );

public:
	void repositoryChanged( Git::Repository newRepository );

private:
	QSet< Module* >			mModules;
};

#endif
