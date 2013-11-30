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

class State
{
public:
    enum Mode
    {
        // "Normal"
        PlainText,

        // $foo
        SimpleVarRef,

        // ${ ParamPart CommandPart ArgumentPart }
        ParamPart,
        CommandPart,
        ArgumentPart
    };

public:
    State(const QString& in)
        : mode(PlainText)
        , input(in)
        , recur(0)
        , save(0)
        , pos(0)
    {}

public:
    Mode        mode;
    QString     input;
    int         recur;
    int         save;
    int         pos;

public:
    QString get()
    {
        return input.mid(save, pos - save);
    }

    void doSave()
    {
        save = pos;
    }

    QChar cur() const
    {
        if (pos < input.length())
            return input.at(pos);
        else
            return QChar();
    }

    /**
     * @brief Appends the currently parsed text part to the output stream and updates the state.
     *
     * @param output the output stream
     * @param part the current parsed part to append
     * @param increment when true, parser goes to the next character
     * @param savePos remember the current (incremented) reading position
     * @param nextMode the mode to switch the state into
     */
    inline void flush(QString &output, const QString &part, bool increment, bool savePos, State::Mode nextMode)
    {
        output += part;

        if (increment)
            pos++;

        if (savePos)
            doSave();

        mode = nextMode;
    }
};


bool ShellExpand::isVarChar(QChar ch)
{
    // verify this is right
    return ch.isLetterOrNumber() || ch == L'_';
}


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
        case State::PlainText:
            if (s.cur() != L'$')
            {
                s.pos++;
            }
            else
            {
                s.flush( output, s.get(), true, true, s.mode );
                if (s.cur() == L'{')
                {
                    s.mode = State::ParamPart;
                    s.pos++;
                    s.doSave();
                }
                else
                {
                    s.mode = State::SimpleVarRef;
                }
            }
            break;

        case State::SimpleVarRef:
            if (isVarChar(s.cur()))
            {
                s.pos++;
            }
            else
            {
                s.flush( output, replacementLogic(s.get()), false, true, State::PlainText );
            }
            break;

        case State::ParamPart:
            if (s.cur() == L'}')
            {
                s.flush(output, replacementLogic(s.get()), true, true, State::PlainText);
            }
            else if (isVarChar(s.cur()))
            {
                s.pos++;
            }
            else
            {
                partParameter = s.get();
                s.flush( output, partParameter, false, true, State::CommandPart );
            }
            break;

        case State::CommandPart:
            if (cmdChars.indexOf(s.cur()) != -1)
            {
                s.pos++;
            }
            else
            {
                partCommand = s.get();
                s.flush(output, partCommand, false, true, State::ArgumentPart);
                s.recur = 0;
            }
            break;

        case State::ArgumentPart:
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
                    s.flush(output, replacementLogic(partParameter, partCommand, partArgument),
                            true, true, State::PlainText);
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
            if (s.mode == State::PlainText)
            {
                s.flush( output, s.get(), false, false, State::PlainText );
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
