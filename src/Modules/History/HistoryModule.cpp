
#include "Libs/Core/MacGitver.h"
#include "Libs/Core/MainWindow.h"

#include "Modules/History/HistoryModule.h"
#include "Modules/History/HistoryView.h"

HistoryModule::HistoryModule()
	: mView( NULL )
{
}

void HistoryModule::repositoryChanged( Git::Repository newRepository )
{
	if( mView )
	{
		mView->repositoryChanged( newRepository );
	}
}

ConfigPage* HistoryModule::configPage( ConfigWidget* widget )
{
	return NULL;
}

Module::Types HistoryModule::providesModuleTypes() const
{
	return View;
}

void HistoryModule::initialize()
{
	mView = new HistoryView();
	MacGitver::self().mainWindow()->integrateView( mView, Heaven::Central );
}

void HistoryModule::deinitialize()
{
	delete mView;
	mView = NULL;
}


IMPLEMENT_INTERNAL_MODULE( History )
