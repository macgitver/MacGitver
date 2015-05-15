#include "Test_SHMParser.hpp"

#include "libMacGitverCore/SHMParser/ShellExpand.hpp"

TEST(SHMParser, SimpleAssignVariables)
{
    ShellExpand::Macros macros;
    macros[QStringLiteral("VAR_TEST1_")] = QStringLiteral("TEST {1}");
    macros[QStringLiteral("VAR_TEST_2")] = QStringLiteral("TEST (2)");
    macros[QStringLiteral("VAR_TEST_3")] = QStringLiteral("TEST (3)");

    ShellExpand se(macros);
    QString source = QString::fromUtf8("_Testing 3 vars: $VAR_TEST1_, $VAR_TEST_2\n; $VAR_TEST_3!");
    QString result = se.expandText(source);

    ASSERT_STREQ("_Testing 3 vars: TEST {1}, TEST (2)\n; TEST (3)!", qPrintable(result));
}

TEST(SHMParser, SimpleAssignMacro)
{
    ShellExpand se;
    QString source = QString::fromUtf8("_${Macro:=Value}_");
    QString result = se.expandText(source);

    ASSERT_STREQ("_Value_", qPrintable(result));

    ShellExpand::Macros m = se.macros();
    ASSERT_EQ(1, m.count());
    ASSERT_TRUE(m.contains(QStringLiteral("Macro")));
    EXPECT_STREQ("Value", qPrintable(m[QStringLiteral("Macro")]));
}

TEST(SHMParser, AddMacroTest)
{
    ShellExpand::Macros macros;
    macros[QStringLiteral("VAR_TEST_1")] = QStringLiteral("TEST 1");

    ShellExpand se(macros);

    ASSERT_FALSE(se.overwriteMacroDeclarations());

    se.addMacro(QStringLiteral("VAR_TEST_2"), QStringLiteral("TEST 2"));
    se.addMacro(QStringLiteral("VAR_TEST_2"), QStringLiteral("TEST 3"));

    ShellExpand::Macros m = se.macros();
    ASSERT_EQ(2, m.count());
    ASSERT_TRUE(m.contains(QStringLiteral("VAR_TEST_2")));
    EXPECT_STREQ("TEST 2", qPrintable(m[QStringLiteral("VAR_TEST_2")]));
}

TEST(SHMParser, RecursiveAssignMacro_Test)
{
    ShellExpand se;
    QString source = QString::fromUtf8("_${MacroOuter:=${MacroInner:=Recurse}}_");
    QString result = se.expandText(source);

    ASSERT_STREQ("_Recurse_", qPrintable(result));

    ShellExpand::Macros m = se.macros();
    ASSERT_EQ(2, m.count());
    EXPECT_STREQ("Recurse", qPrintable(m[QStringLiteral("MacroInner")]));
    EXPECT_STREQ("Recurse", qPrintable(m[QStringLiteral("MacroOuter")]));
}
