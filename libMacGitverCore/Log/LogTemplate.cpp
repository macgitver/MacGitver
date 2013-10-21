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
#include <QStringList>
#include <QStringBuilder>
#include <QDateTime>

#include "libMacGitverCore/Log/LogTemplate.hpp"
#include "libMacGitverCore/Log/LogEvent.hpp"

namespace Log
{

    class Template::Data : public QSharedData
    {
    public:
        Data();

    public:
        QString         name;
        QString         transformation;
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

    Template Template::create(const QString& name)
    {
        Data* d = new Data;
        d->name = name;
        return d;
    }

    QString Template::name() const
    {
        return d ? d->name : QString();
    }

    void Template::setTransformation(const QString& transformText)
    {
        Q_ASSERT(d);

        if (d) {
            d->transformation = transformText;
        }
    }

    QString Template::apply(Event event) const
    {
        if (!d) {
            return QString();
        }

        QString s = d->transformation;

        if (s.isEmpty()) {
            s = event.param(QString());
        }
        else {
            s.replace(QLatin1String("$$"), event.param(QString()));
        }

        foreach (QString pname, event.paramNames()) {
            if (!pname.isEmpty()) {
                QString key = QChar(L'$') % pname % QChar(L'$');
                QString value = event.param(pname);

                value.replace(QChar(L'&'), QLatin1String("&amp;"));
                value.replace(QChar(L'<'), QLatin1String("&lt;"));

                s.replace(key, value);
            }
        }

        return
            QLatin1Literal("<a name=\"") %
            QString::number(event.uniqueId()) %
            QLatin1Literal("\">") %
            s %
            QLatin1Literal("</a>");
    }

    //-- Template::Data ------------------------------------------------------------------------- >8

    Template::Data::Data()
    {
    }

}
