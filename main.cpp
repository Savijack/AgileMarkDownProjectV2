#include <iostream>
#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_MAIN   
#include "catch.hpp"
#include "HTMLConverter.h"
#include <iostream>

using namespace std;

int main()
{
   HTMLConverter *test = new HTMLConverter("./test_documents/1.md");
   test->convert("output.html");
   delete test;
}

//--
TEST_CASE("File Conversion")  
{ 
	SECTION("")
	{
		REQUIRE(10 == 10);
	}

	SECTION("")
	{
		REQUIRE(10 == 10);
	}
}