#include <gmock/gmock.h>
#include "odbc/Driver.hpp"
#include "odbc/ASTNode.hpp"
#include <fstream>

#define NAME ast_do

using namespace testing;

class NAME : public Test
{
public:
    void SetUp() override { driver = new odbc::Driver; }
    void TearDown() override { delete driver; }
    odbc::Driver* driver;
};

using namespace odbc;

TEST_F(NAME, infinite_loop)
{
    EXPECT_THAT(driver->parseString("do\nfoo()\nloop\n"), IsTrue());

    std::ofstream out("out.dot");
    ast::dumpToDOT(out, driver->getAST());
}