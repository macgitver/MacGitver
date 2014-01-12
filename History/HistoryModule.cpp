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

#include "HistoryMode.hpp"
#include "HistoryModule.h"
#include "HistoryView.h"
#include "HistoryConfigPage.h"

HistoryModule::HistoryModule()
    : mMode(NULL)
{
}

HistoryModule::~HistoryModule()
{
}

void HistoryModule::setupConfigPages( ConfigDialog* dialog )
{
    dialog->addPage( new HistoryConfigPage( dialog ) );
}

void HistoryModule::initialize()
{
    mMode = new HistoryMode(this);

    registerView<HistoryView>(tr("History"));
    registerMode(mMode);
}

void HistoryModule::deinitialize()
{
    unregisterView<HistoryView>();
    unregisterMode(mMode);

    delete mMode;
    mMode = NULL;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(History, HistoryModule)
#endif
