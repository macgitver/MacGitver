/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#include "RefRenameDialog.hpp"

#include "RefItem.hpp"

#include "libGitWrap/Reference.hpp"

RefRenameDialog::RefRenameDialog()
    : BlueSky::Dialog()
    , mRefInfo( 0 )
{
    setupUi( this );
    setFixedSize( size() ); // Why?
}

RefRenameDialog::~RefRenameDialog()
{
}

void RefRenameDialog::init( RefBranch* refInfo )
{
    mRefInfo = refInfo;

    updateValues();
}

const Git::Result &RefRenameDialog::gitResult() const
{
    return mGitResult;
}

void RefRenameDialog::accept()
{
    if (!mRefInfo) {
        reject();
        return;
    }

    Git::Reference ref = mRefInfo->reference();
    const QString oldRefName = ref.name();
    const QString prefix = oldRefName.left( oldRefName.length() - ref.shorthand().length() );

    QString newRefName = prefix + textRefName->text();
    if ( !newRefName.isEmpty() && (oldRefName != newRefName) )
    {
        ref.rename( mGitResult, newRefName );
    }

    if ( mGitResult )
        QDialog::accept();
    else
        reject();
}

void RefRenameDialog::updateValues()
{
    if ( !mRefInfo ) return;

    const Git::Reference& ref = mRefInfo->reference();
    Q_ASSERT( ref.isValid() );
    textRefName->setText( ref.shorthand() );
}
