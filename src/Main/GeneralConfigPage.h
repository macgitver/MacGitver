
#ifndef MGV_GENERAL_CONFIG_PAGE_H
#define MGV_GENERAL_CONFIG_PAGE_H

#include "Libs/Core/ConfigPage.h"

namespace Ui
{
	class GeneralConfigPage;
}

class GeneralConfigPage : public ConfigPage
{
public:
	GeneralConfigPage( ConfigDlg* dlg );
	~GeneralConfigPage();

public:
	void apply();
	void init();

	ConfigPageGroup* group();
	QString name() const;

private:
	Ui::GeneralConfigPage*		ui;
};

#endif
