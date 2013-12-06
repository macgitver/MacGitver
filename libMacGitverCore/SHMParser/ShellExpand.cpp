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
#include <QTextStream>
#include <QFile>

class ShellExpand::State
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

    static inline bool isVarChar(QChar ch)
    {
        // verify this is right
        return ch.isLetterOrNumber() || ch == L'_';
    }

public:
    State(const QString& in)
        : mMode(PlainText)
        , mInput(in)
        , mSave(0)
        , mPos(0)
        , mRecur(0)
    {}

private:
    Mode        mMode;
    QString     mInput;
    int         mSave;
    int         mPos;
    int         mRecur;

public:
    inline bool atEnd() const
    {
        return (mPos >= mInput.length());
    }

    inline int next()
    {
        if (atEnd())
            return mPos;

        return ++mPos;
    }

    inline void changeMode(Mode mode, bool increment = true)
    {
        mMode = mode;
        if (increment)
            next();
    }

    QString get()
    {
        return mInput.mid(mSave, mPos - mSave);
    }

    void doSave()
    {
        mSave = mPos;
    }

    QChar cur() const
    {
        return atEnd() ? QChar() : mInput.at(mPos);
    }

    inline Mode mode() const
    {
        return mMode;
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

        changeMode( nextMode, increment );

        if (savePos)
            doSave();        
    }

    inline int recurseIn(bool increment = true)
    {
        if (increment)
            next();
        return ++mRecur;
    }

    inline int recurseOut(bool increment = true)
    {
        if (increment)
            next();
        return mRecur ? --mRecur : mRecur;
    }

    inline void initRecursion()
    {
        mRecur = 0;
    }
};


ShellExpand::ShellExpand()
    : mOverwriteMacroDecls(false)
{
}

bool ShellExpand::overwriteMacroDeclarations() const
{
    return mOverwriteMacroDecls;
}

void ShellExpand::setOverwriteMacroDeclarations(bool enabled)
{
    mOverwriteMacroDecls = enabled;
}

bool ShellExpand::addMacro(const QString& name, const QString& value)
{
    if (!mOverwriteMacroDecls && mMacros.contains(name))
        return false;

    mMacros[name] = value;

    return true;
}

bool ShellExpand::addMacros(const ShellExpand::Macros &macros)
{
    if (mMacros.count())
    {
        bool error = false;
        foreach (const QString& key, macros.keys())
        {
            error = addMacro(key, macros.value(key)) || error;
        }
        return error;
    }
    mMacros = macros;
    return true;
}

/**
 * @brief Apply macro expansion to a given input string.
 * @param input the template string
 * @return The resulting string with the replaced macros.
 */
QString ShellExpand::expandText(const QString &input)
{
    static QString cmdChars = QLatin1String(":#/%+-=&");

    State s(input);

    QString output, partParameter, partCommand, partArgument;

    while (!s.atEnd())
    {
        switch (s.mode())
        {
        case State::PlainText:
            if (s.cur() == L'$')
            {
                s.flush( output, s.get(), true, true, State::SimpleVarRef );
                if (s.cur() == L'{')
                {
                    s.changeMode(State::ParamPart);
                    s.doSave();
                }
            }
            else
            {
                s.next();
            }
            break;

        case State::SimpleVarRef:
            if (State::isVarChar(s.cur()))
            {
                s.next();
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
            else if (State::isVarChar(s.cur()))
            {
                s.next();
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
                s.next();
            }
            else
            {
                partCommand = s.get();
                s.flush(output, partCommand, false, true, State::ArgumentPart);
                s.initRecursion();
            }
            break;

        case State::ArgumentPart:
            if (s.cur() == L'}')
            {
                if (!s.recurseOut())
                {
                    partArgument = s.get();
                    s.flush(output, replacementLogic(partParameter, partCommand, partArgument),
                            false, true, State::PlainText);
                }
            }
            else if (s.cur() == L'{')
            {
                s.recurseIn();
            }
            else
            {
                s.next();
            }
            break;
        }
    }

    // end of text
    if (s.mode() == State::PlainText)
    {
        s.flush( output, s.get(), false, false, State::PlainText );
        return output;
    }

    // fucked up!
    return QString();
}

QString ShellExpand::expandFile(const QString& fileName)
{
    QFile input(fileName);
    if (!input.open(QIODevice::ReadOnly))
        return QString();

    return expandText( QString::fromUtf8(input.readAll().constData()));
}

QString ShellExpand::replacementLogic(QString parameter, QString command, QString arg)
{
    QString value = mMacros.value(parameter, QString());
    QStringList simpleApplyRules;
    simpleApplyRules << QLatin1String(":-") << QLatin1String(":+");

    if (!command.isEmpty())
    {
        if (simpleApplyRules.contains(command))
        {
            if (value.isEmpty())
                value = expandText(arg);
        }
        else if (command == QLatin1String(":="))
        {
            if (value.isEmpty())
            {
                value = expandText(arg);
                mMacros[parameter] = value;
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
        else if (!processExternal(value, command, arg))
        {
            value = QLatin1String(">Error: Unknonwn command<");
        }
    }

    return value;
}

bool ShellExpand::processExternal(QString &value, const QString &command, const QString &arg)
{
    bool result = true;

    if (command == QLatin1String("&-"))
    {
        expandFile( arg );
    }
    else if (command == QLatin1String("&"))
    {
        if (value.isEmpty())
            value = expandFile( arg );
    }
    else
    {
        result = false;
    }

    return result;
}
