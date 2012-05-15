
#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "Libs/Git/Repository.h"

#include "Libs/Core/ConfigPage.h"

class Module : public QObject
{
	Q_OBJECT
public:
	Module();

public:
	virtual void repositoryChanged( Git::Repository newRepository );

	virtual ConfigPage* configPage( ConfigWidget* widget );
};

#endif
