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

#ifndef HEAVEN_VIEW_H
#define HEAVEN_VIEW_H

#include <QWidget>

#include "Heaven/Heaven.h"

namespace Heaven
{
	class ViewContainer;
	class View;

	class HEAVEN_API ContainerContent
	{
	public:
		virtual bool isContainer() const = 0;
		virtual View* asView();
		virtual ViewContainer* asContainer();
	};

	class HEAVEN_API View : public QWidget, public ContainerContent
	{
		Q_OBJECT
	public:
		View( const QString& identifier, ViewTypes type = SingleViewType );

	public:
		ViewTypes type() const;

	public:
		QString identifier() const;
		QString viewName() const;
		void setViewName( const QString& name );

	signals:
		void nameChanged( const QString& viewName );

	protected:
		virtual void aboutToRemove();

	public:	// ContainerContent Interface
		bool isContainer() const;
		View* asView();

	private:
		const QString		mIdentifier;
		QString				mViewName;
		ViewTypes			mType;
		QAction*			mAction;
	};

}

#endif
