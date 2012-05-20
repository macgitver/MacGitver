
#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "Libs/Git/Repository.h"

#include "Libs/Core/ConfigPage.h"

class Module : public QObject
{
	Q_OBJECT
public:
	enum Type
	{
		View			= 1 << 0,


		None			= 0	// Silly :)
	};
	typedef QFlags< Type > Types;

public:
	Module();

public:
	virtual void repositoryChanged( Git::Repository newRepository );

	virtual ConfigPage* configPage( ConfigWidget* widget );
	virtual Types providesModuleTypes() const = 0;

	virtual void initialize();
	virtual void deinitialize();
};

#define IMPLEMENT_INTERNAL_MODULE(Name) \
	Module* createIntModule_##Name() \
	{ \
		return new Name##Module(); \
	}

#endif
