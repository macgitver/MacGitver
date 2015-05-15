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

#pragma once

#include "libMacGitverCore/MacGitverApi.hpp"

#include <QWidget>
#include <QFontComboBox>

class QCheckBox;
class QComboBox;

class MGV_CORE_API FontSelectWidget : public QWidget
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
