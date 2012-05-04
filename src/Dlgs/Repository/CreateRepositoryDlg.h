
#ifndef CREATE_REPOSITORY_DLG_H
#define CREATE_REPOSITORY_DLG_H

#include "ui_CreateRepositoryDlg.h"

class CreateRepositoryDlg : public QDialog, Ui::CreateRepositoryDlg
{
	Q_OBJECT
public:
	CreateRepositoryDlg();

protected:
	void accept();

private slots:
	void onBrowse();
	void checkValid();
};

#endif
