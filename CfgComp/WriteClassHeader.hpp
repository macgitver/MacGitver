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

#ifndef CFGCOMP_WRITE_CLASS_HEADER_HPP
#define CFGCOMP_WRITE_CLASS_HEADER_HPP

#include <QTextStream>
#include <QFile>

#include "Model.hpp"

class WriteClassHeader
{
public:
    WriteClassHeader( const QString& outFile, const ConfigSection& section );

public:
    void generate();

private:
    QString                 mOutFileName;
    QFile                   mOutFile;
    const ConfigSection&    mSection;
    QTextStream             mOutStream;
};

#endif
