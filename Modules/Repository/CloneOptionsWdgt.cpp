#include "CloneOptionsWdgt.hpp"

#include <QLayout>


CloneOptionsWdgt::CloneOptionsWdgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    // margin is controlled by the parent widget's layout
    layout()->setMargin(0);
}

CloneOptionsWdgt::~CloneOptionsWdgt()
{
}

void CloneOptionsWdgt::on_txtCloneMode_currentIndexChanged(int index)
{
    // Note: clone modes are fixed
    mCloneMode = static_cast<CloneMode>( index );
    grpSubmodules->setEnabled( mCloneMode == cmCheckout );
}

void CloneOptionsWdgt::on_chkInitSubmodules_toggled(bool checked)
{
    chkSubmodulesRecursive->setEnabled( checked );
    if( !checked )
    {
        chkSubmodulesRecursive->setChecked( false );
    }
}
