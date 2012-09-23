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

#ifndef HI_GEN_BASE_H
#define HI_GEN_BASE_H

#include <QString>
#include <QFile>
#include <QTextStream>

#include "HICObject.h"

class HIGeneratorBase
{
public:
	HIGeneratorBase( const HIDModel& model, const QString& fileName );

public:
	bool generate();

protected:
	QTextStream& out();
	const HIDModel& model() const;

	QString latin1Encode( const QString& src );
	QString utf8Encode( const QString& src );

	virtual bool run() = 0;

private:
	const HIDModel&		mModel;
	QString				mFileName;
	QFile				mOutFile;
	QTextStream			mOutStream;
};

#endif
