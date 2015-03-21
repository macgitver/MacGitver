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

#ifndef MGV_DIFF_SEQ_VIEW_HPP
#define MGV_DIFF_SEQ_VIEW_HPP

class QGraphicsView;
class QGraphicsScene;

#include "libDiffViews/Views/Interfaces/DiffView.hpp"

namespace DiffViews
{

    class SeqViewContainer;

    class SeqViewInfo
    {
    public:
        qreal           minWidth;
        qreal           maxDiffStatTextLength;
        qreal           maxDiffStatStatLength;
        int             maxChange;

        QFont           mFixed;
        QFont           mVariable;

        QColor          clrSeparator;
        QColor          clrText;
        QColor          clrAdded;
        QColor          clrRemoved;
        QColor          clrContext;
        QColor          clrDeltaFirst;
        QColor          clrDeltaSecond;
    };

    class SequentialView : public DiffView
    {
        Q_OBJECT
    public:
        SequentialView( QWidget* parent = 0 );
        ~SequentialView();

    public:
        void setPatch( Patch::Ptr patch );

    private:
        void generateScene();
        void updateWidth();
        qreal realWidth() const;

    private slots:
        void maybeUpdateWidth();

    protected:
        bool eventFilter( QObject*, QEvent* );
        void resizeEvent( QResizeEvent* );
        void showEvent( QShowEvent* );

    private slots:
        void fontsChanged();

    private:
        QFont               mFixedFont;
        QGraphicsView*      mView;
        QGraphicsScene*     mScene;
        SeqViewContainer*   mDiffStats;
        SeqViewContainer*   mDeltas;
        SeqViewContainer*   mRoot;
        Patch::Ptr          mCurrentPatch;
        qreal               mCachedWidth;
        SeqViewInfo         mInfo;
    };

}

#endif
