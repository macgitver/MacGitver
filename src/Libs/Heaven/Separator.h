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

#ifndef MGV_HEAVEN_SEPARATOR_H
#define MGV_HEAVEN_SEPARATOR_H

#include "Heaven/UiObject.h"

namespace Heaven
{

	class Separator : public UiObject
	{
		Q_OBJECT
	public:
		Separator();
		~Separator();

	public:
		UiObjectTypes type() const;
	};

}

#endif
