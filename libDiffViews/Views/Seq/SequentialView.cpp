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

#include <QScrollBar>
#include <QEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>

#include "Views/Interfaces/DiffViews.hpp"

#include "Views/Seq/SequentialView.hpp"
#include "Views/Seq/SeqViewContainer.hpp"
#include "Views/Seq/SeqViewDiffStat.hpp"
#include "Views/Seq/SeqViewDelta.hpp"
#include "Views/Seq/SeqViewHunk.hpp"

namespace DiffViews
{

    SequentialView::SequentialView( QWidget* parent )
        : DiffView( parent )
    {
        mCachedWidth = -1;
        mScene = NULL;

        mView = new QGraphicsView;
        mView->setFrameShape( QFrame::NoFrame );
        mView->setAlignment( Qt::AlignTop | Qt::AlignHCenter );

        mView->verticalScrollBar()->installEventFilter( this );

        QVBoxLayout* l = new QVBoxLayout;
        l->setMargin( 0 );
        l->setSpacing( 0 );
        l->addWidget( mView );
        setLayout( l );

        mInfo.mFixed = DiffViews::self().fixedFont();
        mInfo.mVariable = DiffViews::self().variableFont();

        // setup colors
        mInfo.clrAdded          = QColor( 0xDD, 0xFF, 0xDD );
        mInfo.clrRemoved        = QColor( 0xFF, 0xDD, 0xDD );
        mInfo.clrContext        = QColor( 0x00, 0x00, 0x00 );
        mInfo.clrSeparator      = QColor( 0xCC, 0xCC, 0xCC );
        mInfo.clrDeltaFirst     = QColor( 0xF8, 0xF8, 0xFF );
        mInfo.clrDeltaSecond    = QColor( 0xCC, 0xCC, 0xCC );
        mInfo.clrText           = QColor( 0x00, 0x00, 0x00 );

        connect( &DiffViews::self(), SIGNAL(fontsChanged()),
                 this, SLOT(fontsChanged()) );
    }

    SequentialView::~SequentialView()
    {
    }

    void SequentialView::setPatch( Patch::Ptr patch )
    {
        mCurrentPatch = patch;

        generateScene();
    }

    void SequentialView::generateScene()
    {
        delete mScene;

        mScene = new QGraphicsScene( this );

        mRoot = new SeqViewContainer( &mInfo );

        mDiffStats = new SeqViewContainer( &mInfo );
        mDiffStats->setVerticalMargins( 0., 4. );

        mDeltas = new SeqViewContainer( &mInfo );
        mDeltas->setVerticalMargins( 0., 10. );

        mDiffStats->setParentItem( mRoot );
        mDeltas->setParentItem( mRoot );
        mScene->addItem( mRoot );

        mView->setScene( mScene );

        if( mCurrentPatch )
        {
            FilePatch::List paths = mCurrentPatch->allPaths();
            foreach( FilePatch::Ptr filePatch, paths )
            {
                SeqViewDiffStat* item = new SeqViewDiffStat( &mInfo );
                item->setPathName( filePatch->pathNames()[ 0 ] );
                item->setParentItem( mDiffStats );

                SeqViewContainer* delta = new SeqViewContainer( &mInfo );
                delta->setVerticalMargins( 3., 3. );
                delta->setParentItem( mDeltas );

                SeqViewDeltaHeader* deltaHeader = new SeqViewDeltaHeader( &mInfo, filePatch );
                deltaHeader->setParentItem( delta );

                if( filePatch->type() == FilePatch::BinaryPatch )
                {
                    item->setBinary( true );
                }
                else
                {
                    TextFilePatch::Ptr textPatch = filePatch->asTextFilePatch();

                    int added = 0, removed = 0;
                    textPatch->totalChanges( added, removed );
                    item->setChangeCount( added, removed );

                    foreach( Hunk::Ptr hunk, textPatch->allHunks() )
                    {
                        SeqViewHunkHeader* hunkHead = new SeqViewHunkHeader( &mInfo, hunk );
                        hunkHead->setParentItem( delta );

                        SeqViewHunkContent* hunkContent = new SeqViewHunkContent( &mInfo, hunk );
                        hunkContent->setParentItem( delta );
                    }
                }
            }
        }

        updateWidth();
    }

    void SequentialView::resizeEvent( QResizeEvent* ev )
    {
        QWidget::resizeEvent( ev );
        updateWidth();
    }

    void SequentialView::showEvent( QShowEvent* ev )
    {
        maybeUpdateWidth();
        QWidget::showEvent( ev );
    }

    qreal SequentialView::realWidth() const
    {
        qreal width = mView->viewport()->width();
        width = qMax( mInfo.minWidth, width );
        return width;
    }

    void SequentialView::maybeUpdateWidth()
    {
        if( mCachedWidth != realWidth() )
        {
            updateWidth();
        }
    }

    void SequentialView::updateWidth()
    {
        if( !mScene || !mDiffStats )
        {
            return;
        }

        mInfo.minWidth = 0;
        mInfo.maxDiffStatTextLength = 0;
        mInfo.maxDiffStatStatLength = 0;
        mInfo.maxChange = 0;

        qreal width = realWidth();
        qreal height = mRoot->setWidth( width );

        mScene->setSceneRect( 0., 0., width, height );
        mRoot->postRendering();

        mCachedWidth = width;

        // realWidth() may now soon become smaller. We might have triggered a change in the visibilty
        // of the vertical scrolling bar. Given our full-width policy, this will also change the
        // visibility of the horizontal scrolling bar, which is doomed to be ugly.
        // However, this seems to happen deffered inside Qt - and we cannot catch it here.
    }

    void SequentialView::fontsChanged()
    {
        mInfo.mFixed = DiffViews::self().fixedFont();
        mInfo.mVariable = DiffViews::self().variableFont();
        updateWidth();
    }

    bool SequentialView::eventFilter( QObject* object, QEvent* ev )
    {
        bool result = QWidget::eventFilter( object, ev );

        if( object == mView->verticalScrollBar() &&
                ( ev->type() == QEvent::Show || ev->type() == QEvent::Hide ) )
        {
            QMetaObject::invokeMethod( this, "maybeUpdateWidth", Qt::QueuedConnection );
        }

        return result;
    }

    MGVDV_IMPLEMENT_DIFFVIEWCREATOR(SequentialView)

}
