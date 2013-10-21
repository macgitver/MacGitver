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

#include <QHash>
#include <QString>
#include <QDateTime>

#include "libMacGitverCore/Log/LogTemplate.hpp"

namespace Log
{

    class Template::Data : public QSharedData
    {
    public:
        Data();

    public:
    };

    Template::Template(const Template& other)
        : d(other.d)
    {
    }

    Template::Template(Data *_d)
        : d(_d)
    {
    }

    Template::Template()
    {
    }

    Template::~Template()
    {
    }

    Template& Template::operator=(const Template& other)
    {
        d = other.d;
        return *this;
    }

    bool Template::isValid() const
    {
        return d;
    }

    //-- Template::Data ---------------------------------------------------------------------------- >8

    Template::Data::Data()
    {
    }

}
