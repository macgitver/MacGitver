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

#include "libMacGitverCore/Config/Config.h"

#include "libGitWrap/ObjectCommit.hpp"
#include "libGitWrap/Result.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/SHMParser/ShellExpand.hpp"

#include "HistoryDetails.h"

#include <QFile>

HistoryDetails::HistoryDetails( QWidget* parent )
    : QWebView( parent )
{
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

    QFile fStyle( QLatin1String(":/commit-detail.css") );
    QString styleTempl = fStyle.open(QFile::ReadOnly) ? QString::fromUtf8( fStyle.readAll().constData() ) : QString();

    mStyle = updateStyle( styleTempl );

    setCommit( mCurrentSHA1 );
}

QString HistoryDetails::updateStyle(const QString &templ) const
{
    // TODO: this hash shall be provided by the "Config" mechanism
    ShellExpand::Macros macros;
    macros[QLatin1String("MGV_FONT")] = Config::defaultFontCSS();
    macros[QLatin1String("MGV_BGCOLOR")] =
            Config::self().get(QLatin1String("mgv-bg")).toString();

    // replace constants in css (sample $MY_CONST)
    return ShellExpand(macros).expandText(templ);
}

void HistoryDetails::setRepository( Git::Repository repo )
{
    mRepo = repo;
    clear();
}

void HistoryDetails::clear()
{
    mCurrentSHA1 = Git::ObjectId();
    setHtml( QString() );
}

void HistoryDetails::setCommit( const Git::ObjectId& sha1 )
{
    if( mCurrentSHA1 != sha1 )
    {
        mCurrentSHA1 = sha1;
        updateText();
    }
}

static inline QString mkRow( const QString& lbl, const QString& content, bool fixed = false )
{
    QString s = QLatin1String(
                "<tr>"
                    "<td class=\"name\">%1:</td>"
                    "<td>%2</td>"
                "</tr>" );

    return s.arg( lbl ).arg( content );
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

    Git::Commit commit = mRepo.lookupCommit( r, mCurrentSHA1 );
    if( !r )
    {
        MacGitver::log( Log::Error, r, "Reading commit details" );
        return;
    }

    QString detailRows;

    foreach (HistoryHeaderDetails detailType, mViewDetailRows)
    {
        switch( detailType )
        {
        case HHD_Subject:
            detailRows += mkRow( trUtf8( "Subject" ),
                                 commit.shortMessage() );
            break;

        case HHD_Author:
            detailRows += mkRow( trUtf8( "Author" ),
                                 commit.author().fullName() );
            break;

        case HHD_AuthorDate:
            detailRows += mkRow( trUtf8( "Author date" ),
                                 commit.author().when().toString() );
            break;

        case HHD_AuthorMail:
            detailRows += mkRow( trUtf8( "Author mail" ),
                                 commit.author().email() );
            break;

        case HHD_AuthorName:
            detailRows += mkRow( trUtf8( "Author name" ),
                                 commit.author().name() );
            break;

        case HHD_Committer:
            detailRows += mkRow( trUtf8( "Committer" ),
                                 commit.committer().fullName() );
            break;

        case HHD_CommitterDate:
            detailRows += mkRow( trUtf8( "Committer date" ),
                                 commit.committer().when().toString() );
            break;

        case HHD_CommitterMail:
            detailRows += mkRow( trUtf8( "Committer mail" ),
                                 commit.committer().email() );
            break;

        case HHD_CommitterName:
            detailRows += mkRow( trUtf8( "Committer name" ),
                                 commit.committer().name() );
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
            MacGitver::log(Log::Error, r, "Reading commit details");
            r.clear();
        }
    }

    QStringList body = commit.message().split( QChar( L'\n' ) );
    QString head = body.takeFirst();

    if( body.count() && !body.first().isEmpty() )
    {
        // If someone typed a commit message without the second line being empty,
        // let's add an empty line after splitting the subject line away.
        body.prepend( QString() );
    }

    QString html = trUtf8(
        "<html>"
            "<head>"
                "<style type=\"text/css\">%1</style>"
            "</head>"
            "<body>"
                "<div class=\"heading\">%2</div>"
                "<table>%3</table>"
                "<pre>%4</pre>"
            "</body>"
        "<html>"
                );

    html = html
            .arg( mStyle )      // %1
            .arg( head )        // %2
            .arg( detailRows )  // %3
            .arg( body.join( QChar( L'\n' ) ) )  // %4
            ;

    setHtml( html );
}
