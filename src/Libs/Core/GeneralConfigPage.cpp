
#include "Libs/Core/GeneralConfigPage.h"

#include "ui_GeneralConfigPage.h"

GeneralConfigPage::GeneralConfigPage( ConfigDlg* dlg )
	: ConfigPage( dlg )
	, ui( new Ui::GeneralConfigPage )
{
	ui->setupUi( this );
}

GeneralConfigPage::~GeneralConfigPage()
{
	delete ui;
}

void GeneralConfigPage::init()
{
}

void GeneralConfigPage::apply()
{
}

ConfigPageGroup* GeneralConfigPage::group()
{
	ConfigPageGroup* common = getOrMakeGroup( trUtf8( "Common" ), "Common" );
	return getOrMakeGroup( name(), "MacGitver", common );
}

QString GeneralConfigPage::name() const
{
	return trUtf8( "MacGitver" );
}
