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

#include "libMacGitverCore/Config/Config.h"

#include "libGitWrap/ObjectCommit.hpp"
#include "libGitWrap/Result.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "HistoryDetails.h"

HistoryDetails::HistoryDetails( QWidget* parent )
    : QTextBrowser( parent )
{
    setFrameShape( NoFrame );

    readConfig();
}

void HistoryDetails::readConfig()
{
    QVariant vList = Config::self().get( "History/Details/List", QLatin1String( "#" ) );
    QString sList = vList.toString();

    if( sList == QLatin1String( "#" ) )
    {
        mViewDetailRows << HHD_Subject << HHD_AuthorName << HHD_AuthorMail << HHD_AuthorDate
                           << HHD_SHA1 << HHD_ChildrenList << HHD_ParentsList;
    }
    else
    {
        QStringList details = sList.split( QChar( L';' ) );

        foreach( QString s, details )
        {
            mViewDetailRows.append( HistoryHeaderDetails( s.toInt() ) );
        }
    }

    setCommit( mCurrentSHA1 );
}

void HistoryDetails::updateText()
{
    Git::Result r;

    Q_ASSERT( mRepo.isValid() || mCurrentSHA1.isNull() );

    if( mCurrentSHA1.isNull() )
    {
        setHtml( QString() );
        return;
    }

    Git::ObjectCommit commit = mRepo.lookupCommit( mCurrentSHA1, r );
    if( !r )
    {
        MacGitver::log( ltError, r, "Reading commit details" );
        return;
    }

    QString detailRows;

    for( int i = 0; i < mViewDetailRows.count(); i++ )
    {
        switch( mViewDetailRows[ i ] )
        {
        case HHD_Subject:
            detailRows += mkRow( trUtf8( "Subject" ),
                                 commit.shortMessage( r ) );
            break;

        case HHD_Author:
            detailRows += mkRow( trUtf8( "Author" ),
                                 commit.author( r ).fullName() );
            break;

        case HHD_AuthorDate:
            detailRows += mkRow( trUtf8( "Author date" ),
                                 commit.author( r ).when().toString() );
            break;

        case HHD_AuthorMail:
            detailRows += mkRow( trUtf8( "Author mail" ),
                                 commit.author( r ).email() );
            break;

        case HHD_AuthorName:
            detailRows += mkRow( trUtf8( "Author name" ),
                                 commit.author( r ).name() );
            break;

        case HHD_Committer:
            detailRows += mkRow( trUtf8( "Committer" ),
                                 commit.committer( r ).fullName() );
            break;

        case HHD_CommitterDate:
            detailRows += mkRow( trUtf8( "Committer date" ),
                                 commit.committer( r ).when().toString() );
            break;

        case HHD_CommitterMail:
            detailRows += mkRow( trUtf8( "Committer mail" ),
                                 commit.committer( r ).email() );
            break;

        case HHD_CommitterName:
            detailRows += mkRow( trUtf8( "Committer name" ),
                                 commit.committer( r ).name() );
            break;

        case HHD_SHA1:
            detailRows += mkRow( trUtf8( "SHA-1" ),
                                 mCurrentSHA1.toString(), true );
            break;

        default:
            break;
        }

        if( !r )
        {
            MacGitver::log( ltError, r, "Reading commit details" );
            r.clear();
        }
    }

    QStringList body = commit.message( r ).split( QChar( L'\n' ) );
    QString head = body.takeFirst();

    if( body.count() && !body.first().isEmpty() )
    {
        // If someone typed a commit message without the second line being empty,
        // let's add an empty line after splitting the subject line away.
        body.prepend( QString() );
    }

    QPalette p;
    QColor clrBackSel = p.color( QPalette::Highlight );
    QColor clrFrntSel = p.color( QPalette::Text ); // Don't want HighlightedText, probably.
    QColor clrBackDtl = p.color( QPalette::Window );
    QColor clrFrntDtl = p.color( QPalette::WindowText );
    QColor clrBackWnd = p.color( QPalette::Base );
    QColor clrFrntWnd = p.color( QPalette::Text );
    QFont fixed = Config::self().defaultFixedFont();

    QString html = trUtf8(
        "<qt style=\"background: %5;\">"
        //  "<div style=\"margin: 2px;padding:0px;color: %6;background-color: %5\">"
                "<table style=\""
                            "border-style: solid;width:100%;"
                            "background-color: %1;"
                            "color: %2;"
                            "border-color: %4;\" border=\"1\" cellspacing=\"0\">"
                    "<tr>"
                        "<td width=\"100%\" style=\"font-size: x-large;color:%2;"
                                                    "padding: 3px;\" bgcolor=\"%1\">"
                            "%7"
                        "</td>"
                    "</tr>"
                    "<tr>"
                        "<td style=\"padding: 3px;color: %4;background-color:%3;\">"
                            "<table>"
                                "%8"
                            "</table>"
                        "</td>"
                    "</tr>"
                "</table>"
                "<pre style=\"font-family: '%10';font-size: %11pt\">"
                    "%9"
                "</pre>"
        //  "</div>"
        "</qt><!-- %6 --!>" );

    html = html
            .arg( clrBackSel.name() )  /* %1 */
            .arg( clrFrntSel.name() )  /* %2 */
            .arg( clrBackDtl.name() )  /* %3 */
            .arg( clrFrntDtl.name() )  /* %4 */
            .arg( clrBackWnd.name() )  /* %5 */
            .arg( clrFrntWnd.name() )  /* %6 */
            .arg( head )               /* %7 */
            .arg( detailRows )         /* %8 */
            .arg( body.join( QChar( L'\n' ) ) ) /* %9 */
            .arg( fixed.family() )     /* %10 */
            .arg( fixed.pointSize() ); /* %11 */

    //qDebug( "%s", qPrintable( html ) );

    setHtml( html );
}
