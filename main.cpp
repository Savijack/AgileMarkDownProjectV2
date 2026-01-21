#include <iostream>
#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_MAIN   
#include "catch.hpp"
using namespace std;

//--
TEST_CASE("Test Template")  
{ 
	REQUIRE(100 == 100); 
}