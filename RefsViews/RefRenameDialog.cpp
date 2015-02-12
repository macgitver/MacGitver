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

#include <QStringBuilder>

#include "RefRenameDialog.hpp"

#include "Branches/RefItem.hpp"

#include "libGitWrap/BranchRef.hpp"
#include "libGitWrap/RefName.hpp"

#include "libMacGitverCore/RepoMan/Branch.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

RefRenameDialog::RefRenameDialog(RefBranch* ref)
    : BlueSky::Dialog()
    , mRefInfo(ref)
{
    if (!mRefInfo) {
        reject();
        return;
    }

    setupUi( this );
    setFixedSize( size() ); // Why?

    Git::RefName nameParser(ref->object()->fullName());
    QString name = nameParser.shorthand();
    QString localName = nameParser.name();

    textRefName->setText(name);
    textRefName->setSelection(name.length() - localName.length(), localName.length());
}

const Git::Result &RefRenameDialog::gitResult() const
{
    return mGitResult;
}

void RefRenameDialog::accept()
{
    Git::Reference ref = mRefInfo->object()->load(mGitResult);
    if (!mGitResult) {
        reject();
        return;
    }

    const QString oldRefName = ref.name();
    const QString prefix = oldRefName.left(oldRefName.length() - ref.shorthand().length());

    QString newRefName = prefix % textRefName->text();

    if (!newRefName.isEmpty() && oldRefName != newRefName) {

        ref.rename(mGitResult, newRefName);

        if (mGitResult) {
            mRefInfo->object()->repository()->refresh();
            QDialog::accept();
            return;
        }
    }

    reject();
}
