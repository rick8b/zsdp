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
#include "../net-util.h"
#include <netinet/in.h>

using namespace zsdp;
using namespace std;

static void printHex(const void* buf, size_t size){
    char line[3 * 16 + 1];
    const uint8_t* buf8 = (const uint8_t*)buf;

    for(size_t i = 0; i < size; i++){
        snprintf(line + 3 * (i % 16), 4, "%02hhx ", buf8[i]);

        if((i + 1) % 16 == 0)
            WARN(line);
    }

    if(size % 16 != 0){
        WARN(line << "\n");
    }
}

TEST_CASE("IPv4 to String", "[IPv4 to String]"){
    sockaddr_storage saddr;

    sockaddr_in* v4 = (sockaddr_in*)&saddr;

    memset(&saddr, 0, sizeof(saddr));
    v4->sin_family = AF_INET;
    v4->sin_len = sizeof(sockaddr_in);
    v4->sin_addr.s_addr = htonl(0x01020304);
    REQUIRE( sockaddrToString(saddr) == "1.2.3.4" );
}

TEST_CASE("IPv6 to String", "[IPv6 to String]"){
    sockaddr_storage saddr;

    memset(&saddr, 0, sizeof(saddr));
    sockaddr_in6* v6 = (sockaddr_in6*)&saddr;
    v6->sin6_family = AF_INET6;
    v6->sin6_len = sizeof(sockaddr_in6);

    for(int i = 0; i < 8; i++)
        v6->sin6_addr.__u6_addr.__u6_addr16[i] = htons(i + 1);

    REQUIRE( sockaddrToString(saddr) == "1:2:3:4:5:6:7:8" );
}

TEST_CASE("Parse IPv4 String", "[Parse IPv4 String]"){
    sockaddr_storage expected;
    sockaddr_in* expectedV4 = (sockaddr_in*)&expected;

    sockaddr_storage actual = sockaddrParse("1.2.3.4");
    sockaddr_in* actualV4 = (sockaddr_in*)&actual;
    memset(&expected, 0, sizeof(expected));
    expectedV4->sin_family = AF_INET;
    expectedV4->sin_len = sizeof(sockaddr_in);
    expectedV4->sin_addr.s_addr = htonl(0x01020304);

    REQUIRE( memcmp(&actualV4->sin_addr, &expectedV4->sin_addr, sizeof(expectedV4->sin_addr)) == 0 );
}


TEST_CASE("Parse IPv6 String", "[Parse IPv6 String]"){
    sockaddr_storage expected;
    sockaddr_in6* expectedV6 = (sockaddr_in6*)&expected;

    sockaddr_storage actual = sockaddrParse("1:2:3:4:5:6:7:8");
    memset(&expected, 0, sizeof(expected));
    expectedV6->sin6_family = AF_INET6;
    expectedV6->sin6_len = sizeof(sockaddr_in6);

    for(int i = 0; i < 8; i++)
        expectedV6->sin6_addr.__u6_addr.__u6_addr16[i] = htons(i + 1);

    REQUIRE( memcmp(&actual, &expected, sizeof(actual)) == 0 );
}

TEST_CASE("Parse Bad IP String", "[Parse Bad IP String]"){
    REQUIRE_THROWS( sockaddrParse("1.1.1.256") );
    REQUIRE_THROWS( sockaddrParse("not an address") );
    REQUIRE_THROWS( sockaddrParse("") );
    REQUIRE_THROWS( sockaddrParse("1.2.3.4.5") );
    REQUIRE_THROWS( sockaddrParse("1:2:3:4") );
    REQUIRE_THROWS( sockaddrParse("1:2:3:4:5:6:7:8:9") );
    REQUIRE_THROWS( sockaddrParse("http://1.2.3.4") );
}
