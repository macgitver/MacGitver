/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QPushButton>

#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"
#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/Config/Ui/ConfigPageProvider.hpp"

#include "ui_ConfigDialog.h"

ConfigDialog::ConfigDialog()
{
    ui = new Ui::ConfigDialog;
    ui->setupUi( this );

    connect( ui->widgetTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
             this, SLOT(onWidgetChange(QTreeWidgetItem*)) );

    connect( ui->buttonBox->button( QDialogButtonBox::Apply ), SIGNAL(clicked()),
             this, SLOT(onApply()) );

    setupConfigPages();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

QMap< int, QSet<ConfigPageProvider*> > ConfigDialog::sProviders;

void ConfigDialog::registerProvider(ConfigPageProvider* provider)
{
    QSet<ConfigPageProvider*>& set = sProviders[provider->configPagePriority()];
    set.insert(provider);
}

void ConfigDialog::unregisterProvider(ConfigPageProvider* provider)
{
    sProviders[provider->configPagePriority()].remove(provider);
}

void ConfigDialog::setupConfigPages()
{
    foreach (int prio, sProviders.keys()) {
        foreach (ConfigPageProvider* cpp, sProviders[prio]) {
            cpp->setupConfigPages(this);
        }
    }
}

void ConfigDialog::addPage( ConfigPage* page )
{
    ui->widgets->addWidget( page );

    QTreeWidgetItem* groupItem = mGroupsById.value( page->groupId(), NULL );
    if( !groupItem )
    {
        groupItem = new QTreeWidgetItem( ui->widgetTree, QStringList( page->groupName() ) );
        mGroupsById.insert( page->groupId(), groupItem );
        QFont f = font();
        f.setBold( true );
        groupItem->setFont( 0, f );
        groupItem->setExpanded( true );
        groupItem->setFlags( Qt::ItemIsEnabled );
    }

    Q_ASSERT( groupItem );
    QTreeWidgetItem* pageItem = new QTreeWidgetItem( groupItem, QStringList( page->pageName() ) );
    QByteArray fullPageId = page->groupId() + "/" + page->pageId();
    mPagesById.insert( fullPageId, page );
    mPageIdsByTree.insert( pageItem, fullPageId );

    page->init();
}

void ConfigDialog::setModified( ConfigPage* page, bool value )
{
    Q_ASSERT( page );
    ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( value );
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( value );
}

void ConfigDialog::onApply()
{
    QWidget* w = ui->widgets->currentWidget();
    ConfigPage* page = qobject_cast< ConfigPage* >( w );
    if( !page )
    {
        return;
    }

    page->apply();
}

int ConfigDialog::exec()
{
    ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );

    return QDialog::exec();
}

void ConfigDialog::onWidgetChange( QTreeWidgetItem* newCurrent )
{
    if( !newCurrent )
    {
        return;
    }

    QByteArray pageId = mPageIdsByTree.value( newCurrent, QByteArray() );
    if( pageId.isEmpty() )
    {
        return;
    }

    ConfigPage* page = mPagesById.value( pageId, NULL );
    if( !page )
    {
        return;
    }

    ui->widgets->setCurrentWidget( page );
}
