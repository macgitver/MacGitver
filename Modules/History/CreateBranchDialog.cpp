#include "CreateBranchDialog.h"
#include "ui_CreateBranchDialog.h"

CreateBranchDialog::CreateBranchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateBranchDialog)
{
    ui->setupUi(this);
}

CreateBranchDialog::~CreateBranchDialog()
{
    delete ui;
}

QString CreateBranchDialog::branchName() const
{
    return ui->textBranchName->text();
}

bool CreateBranchDialog::checkoutBranch() const
{
    return ui->btnCheckoutBranch->isChecked();
}
