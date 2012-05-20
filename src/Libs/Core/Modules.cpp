
#include "Libs/Core/Modules.h"

#define DECLARE_INTERNAL_MODULE(Name) \
	extern Module* createIntModule_##Name();

#define INTERNAL_MODULE(Name) \
	{ #Name, &createIntModule_##Name }

struct InternalModules
{
	const char* szName;
	Module* (*creator)();
};

DECLARE_INTERNAL_MODULE( Diff )
DECLARE_INTERNAL_MODULE( Branches )
DECLARE_INTERNAL_MODULE( History )
DECLARE_INTERNAL_MODULE( Refs )
DECLARE_INTERNAL_MODULE( Tags )
DECLARE_INTERNAL_MODULE( WorkingTree )

static InternalModules sInternals[] =
{
//	INTERNAL_MODULE( Diff ),
//	INTERNAL_MODULE( Branches ),
	INTERNAL_MODULE( History ),
//	INTERNAL_MODULE( Refs ),
//	INTERNAL_MODULE( Tags ),
//	INTERNAL_MODULE( WorkingTree ),
	{ NULL, NULL }
};

Modules::Modules( QObject* parent )
	: QObject( parent )
{
}

Modules::~Modules()
{
	foreach( Module* module, mModules )
	{
		module->repositoryChanged( Git::Repository() );
		delete module;
	}
}

void Modules::initialize()
{
	setupInternals();

	foreach( Module* module, mModules )
	{
		module->initialize();
	}
}

void Modules::setupInternals()
{
	int i = 0;
	while( sInternals[ i ].szName )
	{
		qDebug( "Setting up module %s.", sInternals[ i ].szName );

		Module* m = sInternals[ i ].creator();
		addModule( m );

		i++;
	}
}

void Modules::repositoryChanged( Git::Repository newRepository )
{
	foreach( Module* module, mModules )
	{
		module->repositoryChanged( newRepository );
	}
}

void Modules::addModule( Module *mod )
{
	mModules.insert( mod );
}

void Modules::delModule( Module *mod )
{
	mModules.remove( mod );
}
