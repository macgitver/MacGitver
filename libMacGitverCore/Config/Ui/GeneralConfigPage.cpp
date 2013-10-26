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

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/UserLevelDefinition.h"

#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"

#include "ui_GeneralConfigPage.h"

IMPLEMENT_NESTED_PAGE_CREATOR(GeneralConfigPage, 200)

GeneralConfigPage::GeneralConfigPage( ConfigDialog* dlg )
    : ConfigPage( dlg )
    , ui( new Ui::GeneralConfigPage )
{
    ui->setupUi( this );

    ui->fontGeneral->setSelectedFont( Config::defaultFont() );
    connect( ui->fontGeneral, SIGNAL(currentFontChanged(QFont)),
             this, SLOT(onFontChanged()) );

    ui->fontDialogs->setSelectedFont( Config::defaultDialogFont() );
    connect( ui->fontDialogs, SIGNAL(currentFontChanged(QFont)),
             this, SLOT(onFontChanged()) );

    ui->fontSourceCode->setFontFilters( QFontComboBox::MonospacedFonts );
    ui->fontSourceCode->setSelectedFont( Config::defaultFixedFont() );
    connect( ui->fontSourceCode, SIGNAL(currentFontChanged(QFont)),
             this, SLOT(onFontChanged()) );

    foreach( UserLevelDefinition::Ptr lvl, Config::self().levels() )
    {
        ui->cboUserLevel->addItem( lvl->name(), lvl->precedence() );
    }

    connect( ui->cboUserLevel, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onUserLevelChanged(int)) );

    onUserLevelChanged( 0 );
}

GeneralConfigPage::~GeneralConfigPage()
{
    delete ui;
}

void GeneralConfigPage::init()
{
}

void GeneralConfigPage::apply()
{
    Config::self().setDefaultFont( ui->fontGeneral->selectedFont() );
    Config::self().setDefaultDialogFont( ui->fontDialogs->selectedFont() );
    Config::self().setDefaultFixedFont( ui->fontSourceCode->selectedFont() );
    setModified( false );
}

void GeneralConfigPage::onFontChanged()
{
    setModified();
}

void GeneralConfigPage::onUserLevelChanged( int index )
{
    if( index == -1 )
    {
        return;
    }

    setModified();

    int data = ui->cboUserLevel->itemData( index ).toInt();

    foreach( UserLevelDefinition::Ptr lvl, Config::self().levels() )
    {
        if( lvl->precedence() == data )
        {
            ui->txtUserLevelDescription->setHtml( lvl->description() );
            return;
        }
    }
}

QByteArray GeneralConfigPage::pageId() const
{
    return "MacGitver";
}

QString GeneralConfigPage::pageName() const
{
    return trUtf8( "MacGitver" );
}

QByteArray GeneralConfigPage::groupId() const
{
    return "General";
}

QString GeneralConfigPage::groupName() const
{
    return trUtf8( "Gerneral" );
}

QWidget* GeneralConfigPage::widget()
{
    return this;
}
