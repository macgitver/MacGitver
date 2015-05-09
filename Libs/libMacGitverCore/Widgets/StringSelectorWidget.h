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

#ifndef MGV_WIDGETS_STRING_SELECTOR_WIDGET_H
#define MGV_WIDGETS_STRING_SELECTOR_WIDGET_H

#include <QWidget>
#include <QVariant>

class QString;
class QStringList;
class QIcon;
class QTreeWidgetItem;

#include "libMacGitverCore/MacGitverApi.hpp"

class StringSelectorWidgetPrivate;

class MGV_CORE_API StringSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    enum Button
    {
        btnNone         = 0,

        btnSelectOne    = 1 << 0,
        btnUnselectOne  = 1 << 1,

        btnSelectAll    = 1 << 2,
        btnSelectNone   = 1 << 3,

        btnMoveUp       = 1 << 4,
        btnMoveDown     = 1 << 5,

        btnRemove       = 1 << 6,
        btnCreate       = 1 << 7
    };
    typedef QFlags< Button > Buttons;

public:
    StringSelectorWidget( QWidget* parent = 0 );
    ~StringSelectorWidget();

public:
    void clear();
    void addEntry( const QString& display, bool isSelected, const QVariant& value = QVariant() );
    int countEntries() const;

    int countSelected() const;
    QList< QVariant > selectedValues() const;
    QStringList selectedStrings() const;

    bool isStringSelected( const QString& display ) const;
    bool isValueSelected( const QVariant& value ) const;

    void setButtons( Buttons buttons );
    Buttons buttons() const;

public slots:
    void setValueSelected( const QVariant& value, bool selected = true );
    void setStringSelected( const QString& display, bool selected = true );

    void selectNone();
    void selectAll();

signals:
    void valueSelected( const QVariant& value );
    void stringSelected( const QString& display );

private slots:
    void selItemDblClcked( QTreeWidgetItem* it );
    void unselItemDblClcked( QTreeWidgetItem* it );
    void selectionChanged();
    void clickedSelect();
    void clickedUnselect();

private:
    StringSelectorWidgetPrivate* d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( StringSelectorWidget::Buttons )

#endif

