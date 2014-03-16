#ifndef CREATEBRANCHDIALOG_H
#define CREATEBRANCHDIALOG_H

#include <QDialog>

namespace Ui {
    class CreateBranchDialog;
}

class CreateBranchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateBranchDialog(QWidget *parent = 0);
    ~CreateBranchDialog();

    QString branchName() const;

    bool checkoutBranch() const;

private:
    Ui::CreateBranchDialog *ui;
};

#endif // CREATEBRANCHDIALOG_H
