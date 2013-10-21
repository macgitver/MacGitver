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

#include "libMacGitverCore/Log/LogManager.hpp"

namespace Log
{

    class Manager::Data : public QSharedData
    {
    public:
        Data();
    };

    Manager::Manager(const Manager& other)
        : d(other.d)
    {
    }

    Manager::Manager(Data* _d)
        : d(_d)
    {
    }

    Manager::Manager()
    {
    }

    Manager::~Manager()
    {
    }

    Manager& Manager::operator=(const Manager& other)
    {
        d = other.d;
        return *this;
    }

    bool Manager::isValid() const
    {
        return d;
    }

    Manager Manager::create()
    {
        Data* d = new Data;
        return d;
    }

    void Manager::addMessage(Type t, const QString& message)
    {
    }

    //-- Manager::Data -------------------------------------------------------------------------- >8

    Manager::Data::Data()
    {
    }

}
