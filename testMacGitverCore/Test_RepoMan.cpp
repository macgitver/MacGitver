/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#include "Test_RepoMan.hpp"

#include "libRepoMan/RepoMan.hpp"

void RepoManFixture::expectSignal(QObject* sender, const char * const signature)
{
    connect(sender, signature, this, SLOT(expectedSignal()));

    expectedSender = sender;
    expectedSignature = signature;
    gotSignal = false;
}

bool RepoManFixture::receivedSignal()
{
    bool got = gotSignal;

    disconnect(expectedSender, expectedSignature, this, SLOT(expectedSignal()));
    expectedSender = NULL;
    expectedSignature = NULL;

    return got;
}

void RepoManFixture::expectedSignal()
{
    if (sender() == expectedSender)
    {
        gotSignal = true;
    }
}


TEST_F(RepoManFixture, Trivial)
{
#if 0
    // This test is stupid. If RepoMan was implemented correctly, there would be no signal for
    // opening an "invalid" repository...
    RM::RepoMan* rm = &MacGitver::repoMan();
    expectSignal(rm, SIGNAL(firstRepositoryOpened()));
    rm->open(Git::Repository());
    ASSERT_TRUE(receivedSignal());
#endif
}

