
#include "Libs/Core/Modules.h"

Modules::Modules()
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
