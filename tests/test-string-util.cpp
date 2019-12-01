/*
 * Copyright 2019 Richard Kern <kernrj@gmail.com>
 *
 * This file is part of zsdp.
 *
 * Zsdp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zsdp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with zsdp.  If not, see <https://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../string-util.h"

using namespace zsdp;
using namespace std;

TEST_CASE("Lower Case", "[Lower Case]"){
    REQUIRE( strToLower("") == "" );
    REQUIRE( strToLower(".") == "." );
    REQUIRE( strToLower("a") == "a" );
    REQUIRE( strToLower("A") == "a" );
    REQUIRE( strToLower("hello") == "hello" );
    REQUIRE( strToLower("Hello") == "hello" );
    REQUIRE( strToLower("hellO") == "hello" );
    REQUIRE( strToLower("HELLO") == "hello" );
}

TEST_CASE("Upper Case", "[Upper Case]"){
    REQUIRE( strToUpper("") == "" );
    REQUIRE( strToUpper(".") == "." );
    REQUIRE( strToUpper("a") == "A" );
    REQUIRE( strToUpper("A") == "A" );
    REQUIRE( strToUpper("hello") == "HELLO" );
    REQUIRE( strToUpper("Hello") == "HELLO" );
    REQUIRE( strToUpper("hellO") == "HELLO" );
    REQUIRE( strToUpper("HELLO") == "HELLO" );
}

TEST_CASE("String splitting", "[String splitting]"){
    auto tokens = split("1 2 3 4 5", " ");
    REQUIRE( tokens.size() == 5 );
    for(int i = 0; i < tokens.size(); i++){
        REQUIRE( tokens[i] == to_string(i + 1) );
    }

    tokens = split("11 22 33 44 55", " ");
    REQUIRE( tokens.size() == 5 );
    for(int i = 0; i < tokens.size(); i++){
        REQUIRE( tokens[i] == to_string((i + 1) * 11) );
    }

    tokens = split("1 2 3 4 5 ", " ");
    REQUIRE( tokens.size() == 6 );
    for(int i = 0; i < tokens.size(); i++){
        if(i <= 4)
            REQUIRE( tokens[i] == to_string(i + 1) );
        else
            REQUIRE( tokens[i] == "" );
    }

    tokens = split(" 1 2 3 4 5 ", " ");
    REQUIRE( tokens.size() == 7 );
    for(int i = 0; i < tokens.size(); i++){
        if(i >= 1 && i <= 5)
            REQUIRE( tokens[i] == to_string(i) );
        else
            REQUIRE( tokens[i] == "" );
    }

    tokens = split("1 2 3 4 5", " ", 0);
    REQUIRE( tokens.size() == 5 );
    REQUIRE( tokens[0] == "1" );
    REQUIRE( tokens[1] == "2" );
    REQUIRE( tokens[2] == "3" );
    REQUIRE( tokens[3] == "4" );
    REQUIRE( tokens[4] == "5" );

    tokens = split("1 2 3 4 5", " ", 2);
    REQUIRE( tokens.size() == 2 );
    REQUIRE( tokens[0] == "1" );
    REQUIRE( tokens[1] == "2 3 4 5" );

    tokens = split("1 2 3 4 5", " ", 3);
    REQUIRE( tokens.size() == 3 );
    REQUIRE( tokens[0] == "1" );
    REQUIRE( tokens[1] == "2" );
    REQUIRE( tokens[2] == "3 4 5" );

    tokens = split("96 param1 param2 param3;param4/param5", " ", 2);
    REQUIRE( tokens.size() == 2 );
    REQUIRE( tokens[0] == "96" );
    REQUIRE( tokens[1] == "param1 param2 param3;param4/param5" );
}
