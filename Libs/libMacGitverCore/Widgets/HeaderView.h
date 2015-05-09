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

#ifndef MGV_HEADER_VIEW_H
#define MGV_HEADER_VIEW_H

#include "libMacGitverCore/MacGitverApi.hpp"

#include <QHeaderView>

class MGV_CORE_API HeaderView : public QHeaderView
{
    Q_OBJECT
public:
    HeaderView( Qt::Orientation orientation, QWidget* parent = 0 );

public:
    void setConfigName( const QString& name );
    QString configName() const;

    void setModel( QAbstractItemModel* model );

public slots:
    void loadSizes();
    void saveSizes();

private:
    QString mConfigName;
};

#endif

