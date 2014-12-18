#include "CloneOptionsWdgt.hpp"


CloneOptionsWdgt::CloneOptionsWdgt(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

CloneOptionsWdgt::~CloneOptionsWdgt()
{
}

void CloneOptionsWdgt::on_txtCloneMode_currentIndexChanged(int index)
{
    // Clone modes are fixed
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
