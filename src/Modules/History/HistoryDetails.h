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

#ifndef HISTORY_DETAILS_H
#define HISTORY_DETAILS_H

#include "GitWrap/ObjectId.h"
#include "GitWrap/Repository.h"

#include <QAbstractScrollArea>

enum HistoryHeaderDetails
{
	HHD_Subject = 1,
	HHD_Author,
	HHD_AuthorName,
	HHD_AuthorMail,
	HHD_AuthorDate,
	HHD_Committer,
	HHD_CommitterName,
	HHD_CommitterDate,
	HHD_CommitterMail,
	HHD_SHA1,
	HHD_ChildrenList,
	HHD_ParentsList
};

class HistoryDetails : public QAbstractScrollArea
{
	Q_OBJECT
public:
	HistoryDetails( QWidget* parent = 0 );

public:
	void setRepository( Git::Repository repo );
	void setCommit( const Git::ObjectId& sha1 );

private:
	void calculate();

protected:
	void paintEvent( QPaintEvent* ev );
	void resizeEvent( QResizeEvent* ev );
	void mouseMoveEvent( QMouseEvent* ev );
	void mousePressEvent( QMouseEvent* ev );
	void mouseReleaseEvent( QMouseEvent* ev );

public:
	void clear();
	void readConfig();

private:
	struct HeaderEntry
	{
		HeaderEntry( const QString& param, const QString& value, bool fixed = false )
			: mParameter( param )
			, mValue( value )
			, mFixedFont( fixed )
			, mHovered( false )
		{
		}

		QString	mParameter;
		QString	mValue;
		QRect	mValueRect;
		bool	mFixedFont	: 1;
		bool	mHovered	: 1;
	};

	typedef QList< HeaderEntry > HeaderEntries;

	QString							mTitle;
	Git::ObjectId					mCurrentSHA1;
	HeaderEntries					mHeaders;
	QStringList						mDetails;
	QRect							mDetailsRect;
	QRect							mHeader;
	int								mParamNameWidth;
	Git::Repository					mRepo;
	QList< HistoryHeaderDetails >	mViewDetailRows;
	bool							mViewDetails : 1;
};

#endif
