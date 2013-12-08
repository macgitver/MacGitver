#include "Test_SHMParser.hpp"

#include "libMacGitverCore/SHMParser/ShellExpand.hpp"

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
