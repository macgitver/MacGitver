#ifndef CREATETAGDIALOG_H
#define CREATETAGDIALOG_H

#include <QDialog>

namespace Ui {
    class CreateTagDialog;
}

class CreateTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTagDialog(QWidget *parent = 0);
    ~CreateTagDialog();

    QString tagName() const;

private:
    Ui::CreateTagDialog *ui;
};

#endif // CREATETAGDIALOG_H
