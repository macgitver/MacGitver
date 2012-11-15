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

#ifndef MGV_WIDGETS_STRING_SELECTOR_WIDGET_PRIVATE_H
#define MGV_WIDGETS_STRING_SELECTOR_WIDGET_PRIVATE_H

#include <QIcon>
#include <QVariant>
#include <QTreeWidget>

class QVBoxLayout;
class QPushButton;

#include "StringSelectorWidget.h"

struct StringSelectorWidgetEntry
{
    QString             mDisplay;
    QIcon               mIcon;
    QVariant            mValue;
    QTreeWidgetItem*    mItem;
    bool                mIsSelected;
    int                 mPosition;
};

class StringSelectorWidgetPrivate
{
public:
    typedef QList< StringSelectorWidgetEntry* > Entries;
    typedef QHash< StringSelectorWidget::Button, QPushButton* > ButtonWidgets;

public:
    StringSelectorWidgetPrivate( StringSelectorWidget* owner );
    ~StringSelectorWidgetPrivate();

private:
    void setupUi();

public:
    void clear();
    void updateButtons();
    void updateButtonStates();
    void moveAll( bool select );
    Entries all( bool selected ) const;
    void setEntrySelected( StringSelectorWidgetEntry* entry, bool selected );
    StringSelectorWidgetEntry* entryFromValue( const QVariant& value ) const;
    StringSelectorWidgetEntry* entryFromDisplay( const QString& display ) const;
    void clickedButton( QTreeWidget* widgetFrom, bool newSelection );

public:
    StringSelectorWidget*           mOwner;
    StringSelectorWidget::Buttons   mButtons;
    ButtonWidgets                   mButtonWidgets;
    QVBoxLayout*                    mButtonLayout;
    Entries                         mAllEntries;
    QTreeWidget*                    mSelected;
    QTreeWidget*                    mUnselected;
};

class StringSelectorWidgetTree : public QTreeWidget
{
    Q_OBJECT
public:
    StringSelectorWidgetTree( QWidget* parent = 0 );

public:
    QSize sizeHint() const;
};

#endif
