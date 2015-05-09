/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
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

#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"

IMPLEMENT_NESTED_PAGE_CREATOR(GeneralConfigPage, 200)

GeneralConfigPage::GeneralConfigPage( ConfigDialog* dlg )
    : ConfigPage( dlg )
{
    setupUi(this);

    fontGeneral->setSelectedFont(Config::defaultFont());
    connect(fontGeneral, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(onFontChanged()) );

    fontDialogs->setSelectedFont(Config::defaultDialogFont());
    connect(fontDialogs, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(onFontChanged()));

    fontSourceCode->setFontFilters(QFontComboBox::MonospacedFonts);
    fontSourceCode->setSelectedFont(Config::defaultFixedFont());

    connect(fontSourceCode, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(onFontChanged()) );
}

GeneralConfigPage::~GeneralConfigPage()
{
}

void GeneralConfigPage::init()
{
}

void GeneralConfigPage::apply()
{
    Config::self().setDefaultFont      (fontGeneral->selectedFont());
    Config::self().setDefaultDialogFont(fontDialogs->selectedFont());
    Config::self().setDefaultFixedFont (fontSourceCode->selectedFont());
    setModified( false );
}

void GeneralConfigPage::onFontChanged()
{
    setModified();
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
