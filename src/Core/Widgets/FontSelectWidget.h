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

#ifndef MGV_FONT_SELECT_WIDGET_H
#define MGV_FONT_SELECT_WIDGET_H

#include "Widgets/WidgetsApi.h"

#include <QWidget>
#include <QFontComboBox>

class QCheckBox;
class QComboBox;

class WIDGETS_API FontSelectWidget : public QWidget
{
    Q_OBJECT
public:
    FontSelectWidget( QWidget* parent = NULL );
    ~FontSelectWidget();

public:
    void setSelectedFont( const QFont& font );
    QFont selectedFont();

    void setFontName( const QString& fontName );
    void setFontSize( int size );
    void setBold( bool v );
    void setItalic( bool v );

    bool bold() const;
    bool italic() const;
    QString fontName() const;
    int fontSize() const;

    void setFontFilters( QFontComboBox::FontFilters filters );
    QFontComboBox::FontFilters fontFilters() const;

signals:
    void currentFontChanged( const QFont& font );

private slots:
    void onSizeChanged();
    void onCurrentFontChanged( QFont font );

private:
    QFontComboBox*  mcboFontName;
    QCheckBox*      mchkBold;
    QCheckBox*      mchkItalic;
    QComboBox*      mcboSize;
};

#endif
