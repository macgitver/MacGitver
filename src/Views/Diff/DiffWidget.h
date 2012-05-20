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

#ifndef DIFF_DISPLAY_H
#define DIFF_DISPLAY_H

#include <QFont>
#include <QWidget>

class PatchFile;

class DiffWidget : public QWidget
{
	Q_OBJECT
public:
	DiffWidget();
	~DiffWidget();

public:
	void setDifference( PatchFile* diff );

protected:
	void paintEvent( QPaintEvent* ev );
	void resizeEvent( QResizeEvent* ev );

private:
	void calculateHeights();

private:
	PatchFile*			mDiff;
	QFont				mFont;
	int					mTotalHeight;
};

#endif
