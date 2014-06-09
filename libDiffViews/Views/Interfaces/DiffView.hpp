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

#ifndef MGV_DIFF_VIEW_HPP
#define MGV_DIFF_VIEW_HPP

#include <QWidget>

#include "libDiffViews/Model/Patch.hpp"

namespace DiffViews
{

    class DIFF_VIEWS_API DiffView : public QWidget
    {
        Q_OBJECT
    public:
        DiffView( QWidget* parent );
        ~DiffView();

    public:
        virtual void setPatch( Patch::Ptr patch ) = 0;
    };

    class DIFF_VIEWS_API DiffViewCreator : public QObject
    {
        Q_OBJECT
    public:
        virtual DiffView* create( QWidget* parent ) = 0;
        virtual QString name() const = 0;
    };

    #define MGVDV_DECLARE_DIFFVIEWCREATOR(Name) \
        void registerDiffView_##Name();

    #define MGVDV_IMPLEMENT_DIFFVIEWCREATOR(Name) \
        void registerDiffView_##Name() \
        { \
            class Local : public DiffViewCreator \
            { \
            public: \
                virtual DiffView* create( QWidget* parent ) \
                    { return new Name( parent ); } \
                virtual QString name() const \
                    { return QLatin1String( #Name ); } \
            }; \
            ::DiffViews::DiffViews::self().registerCreator( new Local ); \
        }

}

#endif
