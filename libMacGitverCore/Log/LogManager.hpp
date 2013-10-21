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

#pragma once
#ifndef MGV_LOG_LOG_MANAGER_HPP
#define MGV_LOG_LOG_MANAGER_HPP

#include <QSharedData>

class QString;

namespace Log
{

    enum Type
    {
        Information,
        Normal,
        Warning,
        Error
    };


    class Manager
    {
    public:
        Manager(const Manager& other);
        Manager();
        ~Manager();
        Manager& operator=(const Manager& other);
        bool isValid() const;

    public:
        static Manager create();

    public:
        void addMessage(Type t, const QString& message);

    private:
        class Data;
        Manager(Data* _d);
        QExplicitlySharedDataPointer<Data> d;
    };

}

#endif
