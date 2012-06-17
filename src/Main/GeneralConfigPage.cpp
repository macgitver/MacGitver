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

#include "MacGitver/MacGitver.h"

#include "Config/Config.h"
#include "Config/UserLevelDefinition.h"

#include "GeneralConfigPage.h"

#include "ui_GeneralConfigPage.h"

GeneralConfigPage::GeneralConfigPage( IConfigDialog* dlg )
	: QWidget()
	, IConfigPage( dlg )
	, ui( new Ui::GeneralConfigPage )
{
	ui->setupUi( this );
	ui->fontSourceCode->setFontFilters( QFontComboBox::MonospacedFonts );

	foreach( UserLevelDefinition* lvl, Config::self().levels() )
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
}

void GeneralConfigPage::onUserLevelChanged( int index )
{
	if( index == -1 )
	{
		return;
	}

	setModified();

	int data = ui->cboUserLevel->itemData( index ).toInt();

	foreach( UserLevelDefinition* lvl, Config::self().levels() )
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
