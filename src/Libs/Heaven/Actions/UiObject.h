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

#ifndef MGV_HEAVEN_UIOBJECT_H
#define MGV_HEAVEN_UIOBJECT_H

#include <QObject>
#include <QSet>

#include "Heaven/Heaven.h"

namespace Heaven
{

	class UiContainer;

	class UiObject : public QObject
	{
		friend class UiContainer;
		Q_OBJECT
	protected:
		UiObject( QObject* owner );
		~UiObject();

	public:
		virtual UiObjectTypes type() const = 0;

	protected:
		void addedToContainer( UiContainer* container );
		void removeFromContainer( UiContainer* container );
		void removedFromContainer( UiContainer* container );

	private:
		QSet< UiContainer* >	mContainers;
	};

}

#endif
