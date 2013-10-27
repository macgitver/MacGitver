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

#ifndef TEST_MGV_CORE_FIXTURE_HPP
#define TEST_MGV_CORE_FIXTURE_HPP

#include "gtest/gtest.h"

#include "libMacGitverCore/App/MacGitver.hpp"

// All MacGitverCore tests MUST use this fixture in order to have libMacGitverCore intialized
// and deinitialized again.

class Fixture : public QObject, public ::testing::Test
{
    Q_OBJECT
public:
    Fixture();
    ~Fixture();

public:
    virtual void SetUp();
    virtual void TearDown();

public:
    QString prepareRepo(const char* name);

protected:
    QString dataDir() const;

private:
    MacGitver* mgv;
};

#endif
