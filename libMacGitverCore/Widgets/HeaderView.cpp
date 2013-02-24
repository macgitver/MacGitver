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

#include "libMacGitverCore/Config/Config.h"

#include "HeaderView.h"

HeaderView::HeaderView( Qt::Orientation orientation, QWidget* parent )
    : QHeaderView( orientation, parent )
{
    connect( this, SIGNAL(sectionResized(int,int,int)),
             this, SLOT(saveSizes()) );
}

void HeaderView::setConfigName( const QString& name )
{
    mConfigName = name;
    loadSizes();
}

QString HeaderView::configName() const
{
    return mConfigName;
}

void HeaderView::loadSizes()
{
    if( mConfigName.isEmpty() )
        return;

    bool b = restoreState( Config::self().get( mConfigName ).toByteArray() );
    Q_ASSERT( b );
}

void HeaderView::saveSizes()
{
    if( mConfigName.isEmpty() )
        return;

    Config::self().set( mConfigName, saveState() );
}

void HeaderView::setModel( QAbstractItemModel* model )
{
    saveSizes();
    QHeaderView::setModel( model );
    loadSizes();
}
