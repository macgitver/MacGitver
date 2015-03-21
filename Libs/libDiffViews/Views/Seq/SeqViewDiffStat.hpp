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

#ifndef MGV_DIFF_SEQ_VIEW_DIFF_STAT_HPP
#define MGV_DIFF_SEQ_VIEW_DIFF_STAT_HPP

#include "SeqViewItem.hpp"

namespace DiffViews
{

    class SeqViewDiffStat : public SeqViewItem
    {
    public:
        SeqViewDiffStat( SeqViewInfo* info );

    public:
        void paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* );

    public:
        qreal setWidth( qreal width );
        void postRendering();

        void setPathName( const QString& name );
        void setChangeCount( int added, int removed );
        void setBinary( bool bin );

    private:
        QString statsText() const;

    private:
        qreal               mTextWidth;
        qreal               mStatsWidth;
        QString             mPathName;
        int                 mAdded;
        int                 mRemoved;
        int                 mMaxChange;
        bool                mBinary;
    };

}

#endif
