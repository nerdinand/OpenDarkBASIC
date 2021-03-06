#include <gmock/gmock.h>
#include "odbc/parsers/keywords/Driver.hpp"
#include "odbc/parsers/keywords/KeywordsDB.hpp"

#define NAME parse_keywords

using namespace testing;

class NAME : public Test
{
public:
    void SetUp() override
    {
        keywords = new odbc::KeywordDB;
        driver = new odbc::kw::Driver(keywords);
    }

    void TearDown() override
    {
        delete driver;
        delete keywords;
    }

    odbc::KeywordDB* keywords;
    odbc::kw::Driver* driver;
};

using namespace odbc;

TEST_F(NAME, no_parameters)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=*no parameters*"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(0));
}

TEST_F(NAME, no_parameters_ret)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=(*no parameters*)"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsTrue());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(0));
}

TEST_F(NAME, single_param)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=a"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));
}

TEST_F(NAME, single_param_ret)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=(a)"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsTrue());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));
}

TEST_F(NAME, two_params)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=a, b"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][1], Eq("b"));
}

TEST_F(NAME, two_params_ret)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=(a, b)"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsTrue());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][1], Eq("b"));
}

TEST_F(NAME, zero_or_one_param)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=[*no parameters*] [a]"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(0));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1][0], Eq("a"));
}

TEST_F(NAME, zero_or_one_param_ret)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=[(*no parameters*)] [(a)]"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsTrue());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(0));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1][0], Eq("a"));
}

TEST_F(NAME, one_or_two_params)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=[a] [a, b]"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1].size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1][1], Eq("b"));
}

TEST_F(NAME, one_or_two_params_ret)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=[(a)] [(a, b)]"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsTrue());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1].size(), Eq(2));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[1][1], Eq("b"));
}

TEST_F(NAME, cant_have_return_and_nonreturn_arg_types_1)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=[(a)] [a, b]"), IsFalse());
}

TEST_F(NAME, cant_have_return_and_nonreturn_arg_types_2)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND=help.htm=[a] [(a, b)]"), IsFalse());
}

TEST_F(NAME, arg_options)
{
    ASSERT_THAT(driver->parseString("TOOLBAR SET BUTTON STATE=main.htm=Toolbar, button, state (0, 1 or 2)"), IsTrue());

    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE"), NotNull());
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->name, StrEq("TOOLBAR SET BUTTON STATE"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->helpFile, StrEq("main.htm"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0].size(), Eq(3));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][0], Eq("Toolbar"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][1], Eq("button"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][2], Eq("state"));
}

TEST_F(NAME, arg_options_overload)
{
    ASSERT_THAT(driver->parseString("TOOLBAR SET BUTTON STATE=main.htm=[a, b, c (0, 1 or 2)][a, b (0, 1, 2)]"), IsTrue());

    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE"), NotNull());
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->name, StrEq("TOOLBAR SET BUTTON STATE"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->helpFile, StrEq("main.htm"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads.size(), Eq(2));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0].size(), Eq(3));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][1], Eq("b"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][2], Eq("c"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[1].size(), Eq(2));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[1][0], Eq("a"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[1][1], Eq("b"));
}

TEST_F(NAME, args_with_spaces)
{
    ASSERT_THAT(driver->parseString("TOOLBAR SET BUTTON STATE=main.htm=arg as integer, b as boolean"), IsTrue());

    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE"), NotNull());
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->name, StrEq("TOOLBAR SET BUTTON STATE"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->helpFile, StrEq("main.htm"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0].size(), Eq(2));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][0], Eq("arg as integer"));
    ASSERT_THAT(keywords->lookup("TOOLBAR SET BUTTON STATE")->overloads[0][1], Eq("b as boolean"));
}

TEST_F(NAME, command_name_with_ret_type)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND#=main.htm=(a)"), IsTrue());
    ASSERT_THAT(driver->parseString("SOME COMMAND$=main.htm=(a)"), IsTrue());
}

TEST_F(NAME, command_name_with_ret_type_but_no_retargs_fails)
{
    ASSERT_THAT(driver->parseString("SOME COMMAND#=main.htm=a"), IsFalse());
    ASSERT_THAT(driver->parseString("SOME COMMAND$=main.htm=a"), IsFalse());
}

TEST_F(NAME, parse_multiple_lines)
{
    ASSERT_THAT(driver->parseString(
        "\nSOME COMMAND=help.htm=a\n"
        "SOME OTHER COMMAND=main.htm=b\n"
        "SOME WOWEE=other.html=c\n"), IsTrue());

    ASSERT_THAT(keywords->lookup("SOME COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->name, StrEq("SOME COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->helpFile, StrEq("help.htm"));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME COMMAND")->overloads[0][0], Eq("a"));

    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND")->name, StrEq("SOME OTHER COMMAND"));
    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND")->helpFile, StrEq("main.htm"));
    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME OTHER COMMAND")->overloads[0][0], Eq("b"));

    ASSERT_THAT(keywords->lookup("SOME WOWEE"), NotNull());
    ASSERT_THAT(keywords->lookup("SOME WOWEE")->name, StrEq("SOME WOWEE"));
    ASSERT_THAT(keywords->lookup("SOME WOWEE")->helpFile, StrEq("other.html"));
    ASSERT_THAT(keywords->lookup("SOME WOWEE")->hasReturnType, IsFalse());
    ASSERT_THAT(keywords->lookup("SOME WOWEE")->overloads.size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME WOWEE")->overloads[0].size(), Eq(1));
    ASSERT_THAT(keywords->lookup("SOME WOWEE")->overloads[0][0], Eq("c"));
}
