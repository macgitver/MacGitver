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

#include "FontSelectWidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QFontComboBox>

FontSelectWidget::FontSelectWidget( QWidget* parent )
	: QWidget( parent )
{
	QHBoxLayout* l = new QHBoxLayout;

	mcboFontName = new QFontComboBox;
	mcboFontName->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Preferred );
	l->addWidget( mcboFontName );

	mcboSize = new QComboBox;
	l->addWidget( mcboSize );

	mchkBold = new QCheckBox( trUtf8( "Bold" ) );
	l->addWidget( mchkBold );

	mchkItalic = new QCheckBox( trUtf8( "Italic" ) );
	l->addWidget( mchkItalic );

	l->setMargin( 0 );
	setLayout( l );

	connect( mcboFontName, SIGNAL(currentFontChanged(QFont)),
			 this, SLOT(onCurrentFontChanged(QFont)) );

	connect( mcboSize, SIGNAL(currentIndexChanged(int)),
			 this, SLOT(onSizeChanged()) );

	onCurrentFontChanged( mcboFontName->font() );
}

FontSelectWidget::~FontSelectWidget()
{
}

void FontSelectWidget::setFontFilters( QFontComboBox::FontFilters filters )
{
	mcboFontName->setFontFilters( filters );
}

QFontComboBox::FontFilters FontSelectWidget::fontFilters() const
{
	return mcboFontName->fontFilters();
}

void FontSelectWidget::setFontName( const QString& fontName )
{
	mcboFontName->setCurrentFont( QFont( fontName, fontSize() ) );
}

void FontSelectWidget::setFontSize( int size )
{
	for( int i = 0; i < mcboSize->count(); i++ )
	{
		int s = mcboSize->itemData( i ).toInt();
		if( s == size )
		{
			mcboSize->setCurrentIndex( i );
			return;
		}
		if( s > size && i )
		{
			mcboSize->setCurrentIndex( i - 1 );
			return;
		}
	}
}

void FontSelectWidget::setBold( bool v )
{
	mchkBold->setChecked( v );
}

void FontSelectWidget::setItalic( bool v )
{
	mchkItalic->setChecked( v );
}

bool FontSelectWidget::bold() const
{
	return mchkBold->isChecked();
}

bool FontSelectWidget::italic() const
{
	return mchkItalic->isChecked();
}

QString FontSelectWidget::fontName() const
{
	return mcboFontName->currentFont().family();
}

int FontSelectWidget::fontSize() const
{
	if( mcboSize->currentIndex() != -1 )
	{
		return mcboSize->itemData( mcboSize->currentIndex() ).toInt();
	}
	return -1;
}

void FontSelectWidget::onCurrentFontChanged( QFont font )
{
	QFontDatabase db;

	int oldSize = -1;
	if( mcboSize->currentIndex() != -1 )
	{
		oldSize = mcboSize->itemData( mcboSize->currentIndex() ).toInt();
	}

	font.setBold( bold() );
	font.setItalic( italic() );
	QList< int > sizes = db.pointSizes( font.family(), font.styleName() );

	disconnect( mcboSize, SIGNAL(currentIndexChanged(int)), this, SLOT(onSizeChanged()) );
	mcboSize->clear();
	for( int i = 0; i < sizes.count(); i++ )
	{
		mcboSize->addItem( trUtf8( "%1 pt" ).arg( sizes[ i ] ), sizes[ i ] );
		if( oldSize == sizes[ i ] )
		{
			mcboSize->setCurrentIndex( i );
		}
	}
	connect( mcboSize, SIGNAL(currentIndexChanged(int)), this, SLOT(onSizeChanged()) );

	emit currentFontChanged( font );
}

void FontSelectWidget::onSizeChanged()
{
	QFont f( selectedFont() );
	emit currentFontChanged( f );
}

void FontSelectWidget::setSelectedFont( const QFont& font )
{
	mcboFontName->setCurrentFont( font );
	setFontSize( font.pointSize() );
	qDebug() << font << "=>" << mcboFontName->currentFont();
	setItalic( font.italic() );
	setBold( font.bold() );
}

QFont FontSelectWidget::selectedFont()
{
	QFont f( fontName(), fontSize() );

	f.setBold( bold() );
	f.setItalic( italic() );

	return f;
}
