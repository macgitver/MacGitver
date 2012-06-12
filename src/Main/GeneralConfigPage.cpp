/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include "GeneralConfigPage.h"

#include "ui_GeneralConfigPage.h"

GeneralConfigPage::GeneralConfigPage( IConfigDialog* dlg )
	: QWidget()
	, IConfigPage()
	, ui( new Ui::GeneralConfigPage )
{
	ui->setupUi( this );
	ui->fontSourceCode->setFontFilters( QFontComboBox::MonospacedFonts );
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
