/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#include "libLogger/Internal.hpp"

#include <QHash>
#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QDateTime>

namespace Log
{

    /**
     * @class       Template
     * @brief       A transformation template for Log::Event objects
     *
     * In order to display Event objects in a QWebView, their content has to be wrapped nicely into
     * HTML. This wrapping is the task of instances of this class.
     *
     * Each template has a name, which must be unique in the logging system. Templates have to be
     * registered with the Log::Manager in order to be used.
     *
     * For every template, a transformation has to be defined. A transformation is a string with
     * replacement markers embedded. Replacement markers are any letters covered in dollar signs.
     *
     * The text of the marker is used to search for a parameter inside the event and the marker is
     * then replaced with the content of the parameter.
     *
     * If a template has no transformation assigned, then the event's unnamed parameter is used as
     * the transformation. If the template has a transformation, the replacement marker without text
     * (two consecutive dollar signs) is treated specially. It is replaced before any other
     * replacements are processed.
     *
     * This allows events to use templates and still have full control over the layout.
     *
     */

    /**
     * @brief       Copy constructor
     *
     * @param[in]   other   Tempalte to copy.
     *
     */
    Template::Template(const Template& other)
        : d(other.d)
    {
    }

    Template::Template(Template&& other)
        : d(std::move(other.d))
    {
    }

    /**
     * @internal
     * @brief       Create constructor
     *
     * @param[in]   d       Creates a template with private data
     *
     */
    Template::Template(const std::shared_ptr<Internal::TemplateData>& d)
        : d(d)
    {
    }

    /**
     * @brief       Default constructor
     *
     */
    Template::Template()
    {
    }

    /**
     * @brief       Destructor.
     *
     */
    Template::~Template()
    {
    }

    /**
     * @brief       Assignment operator
     *
     * @param[in]   other   Template to assign to this.
     *
     * @return      A reference to this
     *
     */
    Template& Template::operator=(const Template& other)
    {
        d = other.d;
        return *this;
    }

    Template& Template::operator=(Template&& other)
    {
        std::swap(d, other.d);
        return * this;
    }

    /**
     * @brief       Static creator
     *
     * @param[in]   name    The name of the template to create
     *
     * @return      A new template.
     *
     */
    Template Template::create(const QString& name)
    {
        std::shared_ptr<Internal::TemplateData> d(new Internal::TemplateData);

        d->name = name;
        return d;
    }

    /**
     * @brief       Static creator
     *
     * @param[in]   name    The name of the template to create
     *
     * @return      A new template.
     *
     */
    Template Template::create(QString&& name)
    {
        std::shared_ptr<Internal::TemplateData> d(new Internal::TemplateData);

        d->name = std::move(name);
        return d;
    }

    /**
     * @brief       Get the name of the template
     *
     * @return      Name of the template
     *
     */
    QString Template::name() const
    {
        return d ? d->name : QString();
    }

    /**
     * @brief       Set the transformation for this template
     *
     * @param[in]   transformText   The transformation to be used with this template.
     *
     */
    void Template::setTransformation(const QString& transformText)
    {
        Q_ASSERT(d);

        if (d) {
            d->transformation = transformText;
        }
    }

    /**
     * @brief       Set the transformation for this template
     *
     * @param[in]   transformText   The transformation to be used with this template.
     *
     */
    void Template::setTransformation(QString&& transformText)
    {
        Q_ASSERT(d);

        if (d) {
            d->transformation = std::move(transformText);
        }
    }

    /**
     * @brief       Apply this template to an event
     *
     * @param[in]   event   The event to apply to
     *
     * @return      The transformed template text filled with parameters from the event.
     *
     */
    QString Template::apply(const Event& event) const
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

        return s;
        /* We're adding these later:
            QStringLiteral("<a id=\"") %
            QString::number(event.uniqueId()) %
            QStringLiteral("\">") %
            s %
            QStringLiteral("</a>");
        */
    }

}
