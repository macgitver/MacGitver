
#ifndef MGV_MODULE_HISTORY_H
#define MGV_MODULE_HISTORY_H

#include "Libs/Core/Module.h"

class HistoryView;

class HistoryModule : public Module
{
	Q_OBJECT
public:
	HistoryModule();

public:
	void repositoryChanged( Git::Repository newRepository );

	ConfigPage* configPage( ConfigWidget* widget );
	Types providesModuleTypes() const;

	void initialize();
	void deinitialize();

private:
	HistoryView*			mView;
};

#endif
