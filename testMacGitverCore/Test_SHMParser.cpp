#include "Test_SHMParser.hpp"

#include "libMacGitverCore/SHMParser/ShellExpand.hpp"

TEST(SHMParser, SimpleAssignVariables)
{
    ShellExpand::Macros macros;
    macros[QLatin1String("VAR_TEST1_")] = QLatin1String("TEST {1}");
    macros[QLatin1String("VAR_TEST_2")] = QLatin1String("TEST (2)");
    macros[QLatin1String("VAR_TEST_3")] = QLatin1String("TEST (3)");

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
    ASSERT_TRUE(m.contains(QLatin1String("Macro")));
    EXPECT_STREQ("Value", qPrintable(m[QLatin1String("Macro")]));
}

TEST(SHMParser, AddMacroTest)
{
    ShellExpand::Macros macros;
    macros[QLatin1String("VAR_TEST_1")] = QLatin1String("TEST 1");

    ShellExpand se(macros);

    ASSERT_FALSE(se.overwriteMacroDeclarations());

    se.addMacro(QLatin1String("VAR_TEST_2"), QLatin1String("TEST 2"));
    se.addMacro(QLatin1String("VAR_TEST_2"), QLatin1String("TEST 3"));

    ShellExpand::Macros m = se.macros();
    ASSERT_EQ(2, m.count());
    ASSERT_TRUE(m.contains(QLatin1String("VAR_TEST_2")));
    EXPECT_STREQ("TEST 2", qPrintable(m[QLatin1String("VAR_TEST_2")]));
}

TEST(SHMParser, RecursiveAssignMacro_Test)
{
    ShellExpand se;
    QString source = QString::fromUtf8("_${MacroOuter:=${MacroInner:=Recurse}}_");
    QString result = se.expandText(source);

    ASSERT_STREQ("_Recurse_", qPrintable(result));

    ShellExpand::Macros m = se.macros();
    ASSERT_EQ(2, m.count());
    EXPECT_STREQ("Recurse", qPrintable(m[QLatin1String("MacroInner")]));
    EXPECT_STREQ("Recurse", qPrintable(m[QLatin1String("MacroOuter")]));
}
