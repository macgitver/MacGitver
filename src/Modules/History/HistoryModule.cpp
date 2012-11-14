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

#include <QtPlugin>

#include "Interfaces/IMainWindow.h"
#include "Interfaces/IConfigDialog.h"

#include "HistoryModule.h"
#include "HistoryView.h"
#include "HistoryConfigPage.h"

HistoryModule::HistoryModule()
{
}

void HistoryModule::setupConfigPages( IConfigDialog* dialog )
{
    dialog->addPage( new HistoryConfigPage( dialog ) );
}

Heaven::View* HistoryModule::createHistoryView()
{
    return new HistoryView();
}

void HistoryModule::initialize()
{
    registerView( QLatin1String( "History" ),
                  Heaven::GlobalViewType,
                  &HistoryModule::createHistoryView );
}

void HistoryModule::deinitialize()
{
    unregisterView( QLatin1String( "History" ) );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( History, HistoryModule )
#endif
