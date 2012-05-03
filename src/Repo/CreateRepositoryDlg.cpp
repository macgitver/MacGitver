
#include <QDir>
#include <QFileDialog>

#include "Core/Git.h"

#include "Repo/CreateRepositoryDlg.h"

#include "MainWindow.h"

CreateRepositoryDlg::CreateRepositoryDlg()
{
	setupUi( this );

	connect( btnSelectPath, SIGNAL(clicked()), SLOT(onBrowse()) );
	connect( txtPath, SIGNAL(textChanged(QString)), SLOT(checkValid()) );

	checkValid();
}

void CreateRepositoryDlg::onBrowse()
{
	QString fn = txtPath->text();
	fn = QFileDialog::getExistingDirectory( this, trUtf8( "Browse" ), fn );
	if( fn.isEmpty() )
		return;

	txtPath->setText( fn );
}

void CreateRepositoryDlg::checkValid()
{
	bool okay = !txtPath->text().isEmpty();

	QDir wanted( txtPath->text() );
	okay &= wanted.exists();

	buttonBox->button( QDialogButtonBox::Ok )->setEnabled( okay );

	if( okay )
	{
		QStringList sl = wanted.entryList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
		chkMakeBare->setEnabled( sl.count() == 0 );
	}
}

void CreateRepositoryDlg::accept()
{
	QString fn = QDir::toNativeSeparators( txtPath->text() );
	bool makeBare = chkMakeBare->isChecked() && chkMakeBare->isEnabled();
	Git::Repository* repo = Git::createRepository( fn, makeBare );
	QDialog::accept();

	MainWindow::self().switchToRepo( repo );
}
