#ifndef UICLONEOPTIONS_HPP
#define UICLONEOPTIONS_HPP

#include <QWidget>

#include "ui_CloneOptionsWdgt.h"


class CloneOptionsWdgt : public QWidget, Ui::CloneOptionsWdgt
{
    Q_OBJECT

    friend class CloneRepositoryDlg;

public:
    enum CloneMode {
        cmCheckout  = 0,
        cmBare,
        cmMirror
    };

public:
    explicit CloneOptionsWdgt(QWidget *parent = 0);
    ~CloneOptionsWdgt();

private slots:
    void on_txtCloneMode_currentIndexChanged(int index);

    void on_chkInitSubmodules_toggled(bool checked);

private:
    CloneMode       mCloneMode;
};

#endif // UICLONEOPTIONS_HPP
