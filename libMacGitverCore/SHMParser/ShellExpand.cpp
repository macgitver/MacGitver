/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Nils Fenner <nilsfenner@web.de>
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

#include "ShellExpand.hpp"

#include <QStringList>
#include <QRegExp>


ShellExpand::ShellExpand(const ShellExpand::Macros &macros)
    : mMacros(macros)
{
}

/**
 * @brief Apply macro expansion to a given input string.
 * @param input the template string
 * @return The resulting string with the replaced macros.
 */
QString ShellExpand::apply(const QString &input)
{
    static QString cmdChars = QLatin1String(":#/%+-=");

    State s(input);

    QString output, partParameter, partCommand, partArgument;

    forever
    {
        switch (s.mode)
        {
        case PlainText:
            if (s.cur() != L'$')
            {
                s.pos++;
            }
            else
            {
                output += s.get();
                s.pos++;
                s.doSave();
                if (s.cur() == L'{')
                {
                    s.mode = ParamPart;
                    s.pos++;
                    s.doSave();
                }
                else
                {
                    s.mode = SimpleVarRef;
                }
            }
            break;

        case SimpleVarRef:
            if (isVarChar(s.cur()))
            {
                s.pos++;
            }
            else
            {
                output += replacementLogic(s.get());
                s.doSave();
                s.mode = PlainText;
            }
            break;

        case ParamPart:
            if (s.cur() == L'}')
            {
                output += replacementLogic(s.get());
                s.pos++;
                s.doSave();
                s.mode = PlainText;
            }
            else if (isVarChar(s.cur()))
            {
                s.pos++;
            }
            else
            {
                partParameter = s.get();
                s.doSave();
                s.mode = CommandPart;
            }
            break;

        case CommandPart:
            if (cmdChars.indexOf(s.cur()) != -1)
            {
                s.pos++;
            }
            else
            {
                partCommand = s.get();
                s.doSave();
                s.recur = 0;
                s.mode = ArgumentPart;
            }
            break;

        case ArgumentPart:
            if (s.cur() == L'}')
            {
                if (s.recur)
                {
                    s.recur--;
                    s.pos++;
                }
                else
                {
                    partArgument = s.get();
                    output += replacementLogic(partParameter, partCommand, partArgument);
                    s.pos++;
                    s.doSave();
                    s.mode = PlainText;
                }
            }
            else if (s.cur() == L'{')
            {
                s.recur++;
                s.pos++;
            }
            else
            {
                s.pos++;
            }
            break;
        }

        if (s.pos == s.input.length())
        {
            if (s.mode == PlainText)
            {
                output += s.get();
                return output;
            }
            // fucked up!
            return QString();
        }
    }
}


QString ShellExpand::replacementLogic(QString parameter, QString command, QString arg)
{
    QString value = mMacros.value(parameter, QString());

    if (!command.isEmpty())
    {
        if (command == QLatin1String(":-"))
        {
            if (value.isEmpty()) {
                value = apply(arg);
            }
        }
        else if (command == QLatin1String(":="))
        {
            if (value.isEmpty()) {
                value = apply(arg);
                mMacros[parameter] = value;
            }
        }
        else if (command == QLatin1String(":+"))
        {
            if (!value.isEmpty())
            {
                value = apply(arg);
            }
        }
        else if (command == QLatin1String(":"))
        {
            QStringList sl = arg.split(QLatin1String(":"));
            if (sl.count() == 1)
            {
                value = value.mid(sl[0].toInt());
            }
            else if (sl.count() == 2)
            {
                value = value.mid(sl[0].toInt(), sl[1].toInt());
            }
            else
            {
                value = QLatin1String(">Error: Bar arg count<");
            }
        }
        else {
            value = QLatin1String(">Error: Unknonwn command<");
        }
    }

    return value;
}

QString ShellExpand::State::get()
{
    return input.mid(save, pos - save);
}

void ShellExpand::State::doSave()
{
    save = pos;
}

QChar ShellExpand::State::cur() const
{
    if (pos < input.length())
        return input.at(pos);
    else
        return QChar();
}

bool ShellExpand::isVarChar(QChar ch)
{
    // verify this is right
    return ch.isLetterOrNumber() || ch == L'_';
}
