#include "CreateTagDialog.h"
#include "ui_CreateTagDialog.h"

CreateTagDialog::CreateTagDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTagDialog)
{
    ui->setupUi(this);
}

CreateTagDialog::~CreateTagDialog()
{
    delete ui;
}

QString CreateTagDialog::tagName() const
{
    return ui->textTagName->text();
}
