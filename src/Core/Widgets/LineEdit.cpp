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

#include <QPalette>
#include <QApplication>

#include "Widgets/LineEdit.h"

LineEdit::LineEdit( QWidget* parent )
    : QLineEdit( parent )
    , mMandatory( false )
{
    init();
}

LineEdit::LineEdit( const QString& text, QWidget* parent )
    : QLineEdit( text, parent )
    , mMandatory( false )
{
    init();
}

void LineEdit::init()
{
    connect( this, SIGNAL(textChanged(QString)),
             this, SLOT(updatePalette()) );
}

void LineEdit::setMandatory( bool mandatory )
{
    if( mMandatory != mandatory )
    {
        mMandatory = mandatory;
        updatePalette();
    }
}

bool LineEdit::isMandatory() const
{
    return mMandatory;
}

void LineEdit::updatePalette()
{
    QPalette p;

    if( mMandatory && text().isEmpty() )
    {
        p = QApplication::palette();
        QColor c = p.color( QPalette::Active, QPalette::Highlight );
        p.setColor( QPalette::Active, QPalette::Base, c.lighter( 130 ) );
    }

    setPalette( p );
}
