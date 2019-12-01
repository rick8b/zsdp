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
#include <zsdp/sdp.h>
#include "../net-util.h"

using namespace zsdp;
using namespace std;

TEST_CASE("SDP Enums to Strings", "[Enums to Strings]"){
    REQUIRE( mediaTypeToString(MediaType::Video) == "video" );
    REQUIRE( mediaTypeToString(MediaType::Audio) == "audio" );
    REQUIRE( mediaTypeToString(MediaType::Application) == "application" );
    REQUIRE( mediaTypeToString(MediaType::Message) == "message" );
    REQUIRE( mediaTypeToString(MediaType::Text) == "text" );

    REQUIRE_THROWS( mediaTypeToString(MediaType::NotSet) );
    REQUIRE_THROWS( mediaTypeToString((MediaType)-1) );
}

TEST_CASE("Origin", "[Origin]"){
    Origin origin;

    string expected = "- - - IN IP4 0.0.0.0";
    string actual = origin.toString();
    REQUIRE( expected == actual );

    origin = Origin();
    origin.username = "username";
    origin.sessionID = "sessionID";
    origin.sessionVersion = "sessionVersion";
    origin.networkType = NetworkType::IN;
    origin.addressType = AddressType::IP4;
    origin.host = "example.com";
    expected = "username sessionID sessionVersion IN IP4 example.com";
    actual = origin.toString();
    REQUIRE( expected == actual );

    origin = Origin();
    origin.addressType = AddressType::IP6;
    expected = "- - - IN IP6 ::";
    actual = origin.toString();
    REQUIRE( expected == actual );

    origin = Origin();
    origin.networkType = (NetworkType)-1;
    REQUIRE_THROWS( origin.toString() );

    origin = Origin();
    origin.addressType = (AddressType)-1;
    REQUIRE_THROWS( origin.toString() );

    origin = Origin();
    origin.host = "";
    expected = "- - - IN IP4 0.0.0.0";
    actual = origin.toString();
    REQUIRE( expected == actual );
}

TEST_CASE("SDP", "[SDP]"){
    Sdp sdp;

    string actual = sdpToString(&sdp);
    string expected =
        "v=0\r\n"
        "o=- - - IN IP4 0.0.0.0\r\n"
        "s= \r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "t=0 0\r\n";
    REQUIRE( actual == expected );

    sdp = Sdp();
    sdp.origin.username = "username";
    sdp.origin.sessionID = "sessionID";
    sdp.origin.sessionVersion = "sessionVersion";
    sdp.origin.networkType = NetworkType::IN;
    sdp.origin.addressType = AddressType::IP4;
    sdp.origin.host = "example.com";

    sdp.sessionName = "sessionName";

    sdp.sessionInformation = "sessionInformation";

    sdp.uri = "http://example.com/video";
    sdp.email = "email@example.com";
    sdp.phoneNumber = "+1 (555) 555-5555";
    sdp.connectionData.networkType = NetworkType::IN;
    sdp.connectionData.addressType = AddressType::IP4;
    sdp.connectionData.host = "1.2.3.4";

    sdp.bandwidth.type = BandwidthType::ConferenceTotal;
    sdp.bandwidth.kbps = 1024;

    TimeZoneAdjustment tz;
    tz.adjustAtTime = 1000;
    tz.adjustment = -3600;
    sdp.timeZoneAdjustments.push_back(tz);

    tz.adjustAtTime = 2000;
    tz.adjustment = 3600;
    sdp.timeZoneAdjustments.push_back(tz);

    sdp.encryption.type = EncryptionType::Clear;
    sdp.encryption.key = "12345";

    Timing time;
    time.start = 1000;
    time.end = 2000;
    sdp.times.push_back(time);

    actual = sdpToString(&sdp);
    expected =
        "v=0\r\n"
        "o=username sessionID sessionVersion IN IP4 example.com\r\n"
        "s=sessionName\r\n"
        "i=sessionInformation\r\n"
        "u=http://example.com/video\r\n"
        "e=email@example.com\r\n"
        "p=+1 (555) 555-5555\r\n"
        "c=IN IP4 1.2.3.4\r\n"
        "b=CT:1024\r\n"
        "z=1000 -3600 2000 3600\r\n"
        "k=clear:12345\r\n"
        "t=1000 2000\r\n";
    REQUIRE( actual == expected );
}

