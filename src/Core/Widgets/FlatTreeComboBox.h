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

#ifndef MGV_FLAT_TREE_COMBOBOX_H
#define MGV_FLAT_TREE_COMBOBOX_H

#include "Widgets/WidgetsApi.h"

#include <QComboBox>

class FlatTreeModel;

class WIDGETS_API FlatTreeComboBox : public QComboBox
{
    Q_OBJECT
public:
    FlatTreeComboBox( QWidget* parent = 0 );

public:
    void clear();

    void setModel( FlatTreeModel* model );

    void add( const QString& entry );
    void add( const QStringList& entries );

    void remove( const QString& entry );
    void remove( const QStringList& entries );
};

#endif
