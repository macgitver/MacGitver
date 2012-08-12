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

#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>

#include "GitWrap/Reference.h"
#include "GitWrap/ObjectCommit.h"

#include "MacGitver/MacGitver.h"

#include "HistoryView.h"
#include "HistoryEntry.h"
#include "HistoryModel.h"
#include "HistoryBuilder.h"
#include "HistoryList.h"

void HistoryViewDelegate::paintGraphLane( QPainter* p, GraphGlyphs glyph, int x1, int x2,
										  int height, const QColor& col, const QColor& activeCol,
										  const QBrush& back ) const
{
	const int padding = 2;
	x1 += padding;
	x2 += padding;

	int h = height / 2;
	int m = (x1 + x2) / 2;
		int r = (x2 - x1) * 1 / 3;
	int d =  2 * r;

	#define P_CENTER m , h
	#define P_0      x2, h      // >
	#define P_90     m , 0      // ^
	#define P_180    x1, h      // <
	#define P_270    m , height  // v
	#define DELTA_UR 2*(x1 - m), height ,   0*16, 90*16  // -,
	#define DELTA_DR 2*(x1 - m), -height, 270*16, 90*16  // -'
	#define DELTA_UL 2*(x2 - m), height ,  90*16, 90*16  //  ,-
	#define DELTA_DL 2*(x2 - m), -height, 180*16, 90*16  //  '-
	#define CENTER_UR x1, height, 225
	#define CENTER_DR x1, 0  , 135
	#define CENTER_UL x2, height, 315
	#define CENTER_DL x2, 0  ,  45
	#define R_CENTER m - r, h - r, d, d

	QPen lanePen( Qt::black, 2 );

	// arc
	switch( glyph )
	{
	case ggJoin:
	case ggJoinRight:
	case ggHead:
	case ggHeadRight:
	{
		QConicalGradient gradient(CENTER_UR);
		gradient.setColorAt(0.375, col);
		gradient.setColorAt(0.625, activeCol);
		lanePen.setBrush(gradient);
		p->setPen(lanePen);
		p->drawArc(P_CENTER, DELTA_UR);
		break;
	}
	case ggJoinLeft:
	{
		QConicalGradient gradient(CENTER_UL);
		gradient.setColorAt(0.375, activeCol);
		gradient.setColorAt(0.625, col);
				lanePen.setBrush(gradient);
				p->setPen(lanePen);
		p->drawArc(P_CENTER, DELTA_UL);
		break;
	}
	case ggTail:
	case ggTailRight:
	{
		QConicalGradient gradient(CENTER_DR);
		gradient.setColorAt(0.375, activeCol);
		gradient.setColorAt(0.625, col);
				lanePen.setBrush(gradient);
				p->setPen(lanePen);
		p->drawArc(P_CENTER, DELTA_DR);
		break;
	}
	default:
		break;
	}

	lanePen.setColor(col);
	p->setPen(lanePen);

	// vertical line
	switch( glyph )
	{
	case ggCurrent:
	case ggNotCurrent:
	case ggMergeFork:
	case ggMergeForkRight:
	case ggMergeForkLeft:
	case ggJoin:
	case ggJoinRight:
	case ggJoinLeft:
	case ggCross:
		p->drawLine(P_90, P_270);
		break;
	case ggHeadLeft:
	case ggBranch:
		p->drawLine(P_CENTER, P_270);
		break;
	case ggTailLeft:
	case ggInitial:
		p->drawLine(P_90, P_CENTER);
		break;
	default:
		break;
	}

	lanePen.setColor(activeCol);
	p->setPen(lanePen);

	// horizontal line
	switch( glyph )
	{
	case ggMergeFork:
	case ggJoin:
	case ggHead:
	case ggTail:
	case ggCross:
	case ggCrossUnused:
		p->drawLine(P_180, P_0);
		break;
	case ggMergeForkRight:
		p->drawLine(P_180, P_CENTER);
		break;
	case ggMergeForkLeft:
	case ggHeadLeft:
	case ggTailLeft:
		p->drawLine(P_CENTER, P_0);
		break;
	default:
		break;
	}

	// center symbol, e.g. rect or ellipse
	switch( glyph )
	{
	case ggCurrent:
	case ggInitial:
	case ggBranch:
		p->setPen(Qt::black);
		p->setBrush(col);
		p->drawEllipse(R_CENTER);
		break;
	case ggMergeFork:
	case ggMergeForkLeft:
	case ggMergeForkRight:
		p->setPen(Qt::black);
		p->setBrush(col);
		p->drawRect(R_CENTER);
		break;
	default:
		break;
	}
	#undef P_CENTER
	#undef P_0
	#undef P_90
	#undef P_180
	#undef P_270
	#undef DELTA_UR
	#undef DELTA_DR
	#undef DELTA_UL
	#undef DELTA_DL
	#undef CENTER_UR
	#undef CENTER_DR
	#undef CENTER_UL
	#undef CENTER_DL
	#undef R_CENTER
}

void HistoryViewDelegate::paintGraph( QPainter* p, const QStyleOptionViewItem& opt,
									  const QModelIndex& i) const
{
	static const QColor colors[] = { Qt::red, //DARK_GREEN,
											   Qt::blue, Qt::darkGray, //BROWN,
											   Qt::magenta //, ORANGE
											 };
	if (opt.state & QStyle::State_Selected)
		p->fillRect(opt.rect, opt.palette.highlight());
	else if (i.row() & 1)
		p->fillRect(opt.rect, opt.palette.alternateBase());
	else
		p->fillRect(opt.rect, opt.palette.base());


	HistoryModel* m = const_cast< HistoryModel* >( qobject_cast< const HistoryModel* >( i.model() ) );
	HistoryEntry* e = m->entries()->at( i.row() );

	p->save();
	p->setRenderHint( QPainter::Antialiasing );
	p->setClipRect(opt.rect, Qt::IntersectClip);
	p->translate(opt.rect.topLeft());

	QBrush back = opt.palette.base();
	GraphGlyphVector lanes = e->glyphs();
	uint laneNum = lanes.count();
	uint activeLane = 0;
	for (uint i = 0; i < laneNum; i++)
		if (lanes[i] == ggCurrent || lanes[i] == ggInitial || lanes[i] == ggBranch ||
				lanes[i] == ggMergeFork || lanes[i] == ggMergeForkLeft || lanes[i] == ggMergeForkRight )
		{
			activeLane = i;
			break;
		}

	int x1 = 0, x2 = 0;
	int height = opt.rect.height();
	int maxWidth = opt.rect.width();
	int lw = 3 * 18 / 4;
	QColor activeColor = colors[activeLane % 4];
//	if (opt.state & QStyle::State_Selected)
//		activeColor = blend(activeColor, opt.palette.highlightedText().color(), 208);
	for (uint i = 0; i < laneNum && x2 < maxWidth; i++) {

		x1 = x2;
		x2 += lw;

		GraphGlyphs ln = lanes[i];
		if (ln == ggUnused)
			continue;

		QColor color = i == activeLane ? activeColor : colors[i % 4];
		paintGraphLane(p, ln, x1, x2, height, color, activeColor, back);
	}
	p->restore();
}

void HistoryViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
								 const QModelIndex& index ) const
{
	if( index.column() != 0 )
	{
		QItemDelegate::paint( painter, option, index );
		return;
	}

	drawBackground( painter, option, index );

	paintGraph( painter, option, index );
}


HistoryView::HistoryView()
	: GlobalView( QLatin1String( "History" ) )
{
	setViewName( trUtf8( "History" ) );

	setFont( QFont( QLatin1String( "Verdana" ), 8 ) );

	setSizePolicy( QSizePolicy::MinimumExpanding,
				   QSizePolicy::MinimumExpanding );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );
	setLayout( l );

	mToolBar = new QToolBar;

	QAction* a = mToolBar->addAction( trUtf8( "All Branches" ), this, SLOT(onViewAllBranches()) );
	a->setCheckable( true );

	l->addWidget( mToolBar );

	mList = new HistoryList;
	mList->setFrameShape( QFrame::NoFrame );
	l->addWidget( mList );

	mModel = new HistoryModel( this );
	mList->setModel( mModel );

	mList->setItemDelegate( new HistoryViewDelegate );

	mBuilder = NULL;

	connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
			 this, SLOT(repositoryChanged(Git::Repository)) );

	Git::Repository repo = MacGitver::self().repository();
	if( repo.isValid() )
	{
		repositoryChanged( repo );
	}
}

void HistoryView::repositoryChanged( Git::Repository repo )
{
	if( mBuilder )
	{
		delete mBuilder;
		mBuilder = NULL;
	}

	mRepo = repo;

	if( mRepo.isValid() )
	{
		buildHistory();
	}
}

void HistoryView::buildHistory()
{
	if( !mRepo.isValid() )
	{
		return;
	}

	if( !mBuilder )
	{
		mBuilder = new HistoryBuilder( mRepo, mModel->entries() );
	}

	mBuilder->addHEAD();
	mBuilder->start();
}
