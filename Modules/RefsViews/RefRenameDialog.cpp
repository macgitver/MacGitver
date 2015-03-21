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

#include "libGitWrap/RefName.hpp"

#include "libMacGitverCore/RepoMan/Branch.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

RefRenameDialog::RefRenameDialog(const Git::Reference &ref, RM::Repo* repo)
    : BlueSky::Dialog()
    , mRef(ref)
    , mRepo(repo)
{
    if (!mRef.isValid()) {
        reject();
        return;
    }

    setupUi( this );

    Git::RefName nameParser = ref.nameAnalyzer();
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
    const QString oldRefName = mRef.name();
    const QString prefix = oldRefName.left(oldRefName.length() - mRef.shorthand().length());

    QString newRefName = prefix % textRefName->text();

    if (!newRefName.isEmpty() && oldRefName != newRefName) {

        mRef.rename(mGitResult, newRefName);

        if (mGitResult) {
            mRepo->refresh();
            QDialog::accept();
            return;
        }
    }

    reject();
}
