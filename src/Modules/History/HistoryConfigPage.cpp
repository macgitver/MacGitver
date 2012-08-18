
#include "HistoryConfigPage.h"
#include "HistoryDetails.h"

HistoryConfigPage::HistoryConfigPage( IConfigDialog* dlg )
	: IConfigPage( dlg )
{
	setupUi( this );
	init();
}

HistoryConfigPage::~HistoryConfigPage()
{
}

void HistoryConfigPage::apply()
{
}

void HistoryConfigPage::init()
{
	sswDetailsOverview->addEntry( trUtf8( "Author" ), false, HHD_Author );
	sswDetailsOverview->addEntry( trUtf8( "Author date" ), false, HHD_AuthorDate );
	sswDetailsOverview->addEntry( trUtf8( "Author name" ), false, HHD_AuthorName );
	sswDetailsOverview->addEntry( trUtf8( "Author mail" ), false, HHD_AuthorMail );
	sswDetailsOverview->addEntry( trUtf8( "Committer" ), false, HHD_Committer );
	sswDetailsOverview->addEntry( trUtf8( "Committer date" ), false, HHD_CommitterDate );
	sswDetailsOverview->addEntry( trUtf8( "Committer name" ), false, HHD_CommitterName );
	sswDetailsOverview->addEntry( trUtf8( "Committer mail" ), false, HHD_CommitterMail );
}

QByteArray HistoryConfigPage::pageId() const
{
	return "History";
}

QByteArray HistoryConfigPage::groupId() const
{
	return "Views";
}

QString HistoryConfigPage::pageName() const
{
	return trUtf8( "History" );
}

QString HistoryConfigPage::groupName() const
{
	return trUtf8( "Views" );
}

QWidget* HistoryConfigPage::widget()
{
	return this;
}
