/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#ifndef HEAVEN_VIEW_H
#define HEAVEN_VIEW_H

#include <QWidget>

#include "Heaven/Heaven.h"

class HEAVEN_API HeavenView : public QWidget
{
	Q_OBJECT
public:
	enum Type
	{
		Multiviewable,
		Singleviewable
	};


public:
	HeavenView();

public:
	QString viewName() const;
	void setViewName( const QString& name );

signals:
	void nameChanged( const QString& viewName );

private:
	QString				mViewName;
	QAction*			mAction;
};

#endif
