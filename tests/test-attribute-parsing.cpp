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
#include "../attributes-priv.h"
#include "../string-util.h"


using namespace zsdp;
using namespace std;

TEST_CASE("Parse Media Direction Attribute", "[Media Direction Attribute]"){
    sp<Attribute> attr = parseMediaDirection("sendrecv", "");
    REQUIRE( attr->key() == "sendrecv" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendrecv" );

    attr = parseMediaDirection("sendrecv", "asdf");
    REQUIRE( attr->key() == "sendrecv" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendrecv" );

    attr = parseMediaDirection("SendRecv", "");
    REQUIRE( attr->key() == "sendrecv" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendrecv" );

    attr = parseMediaDirection("sendonly", "");
    REQUIRE( attr->key() == "sendonly" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendonly" );

    attr = parseMediaDirection("sendonly", "1234");
    REQUIRE( attr->key() == "sendonly" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendonly" );

    attr = parseMediaDirection("SendOnly", "");
    REQUIRE( attr->key() == "sendonly" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendonly" );

    attr = parseMediaDirection("recvonly", "");
    REQUIRE( attr->key() == "recvonly" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "recvonly" );

    attr = parseMediaDirection("recvonly", "asdf");
    REQUIRE( attr->key() == "recvonly" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "recvonly" );

    attr = parseMediaDirection("RecvOnly", "");
    REQUIRE( attr->key() == "recvonly" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "recvonly" );

    attr = parseMediaDirection("inactive", "");
    REQUIRE( attr->key() == "inactive" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "inactive" );

    attr = parseMediaDirection("inactive", "asdf");
    REQUIRE( attr->key() == "inactive" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "inactive" );

    attr = parseMediaDirection("Inactive", "");
    REQUIRE( attr->key() == "inactive" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "inactive" );

    REQUIRE_THROWS( parseMediaDirection("not valid", "") );
    REQUIRE_THROWS( parseMediaDirection("not valid", "asdf") );
}

TEST_CASE("Parse Orientation Attribute", "[Orientation Attribute]"){
    sp<Attribute> attr = parseOrientation("orient", "landscape");
    AttrOrientation* orientation = (AttrOrientation*)attr.get();
    REQUIRE( orientation->orientation == Orientation::Landscape );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "landscape" );
    REQUIRE( attr->sdpLine() == "orient:landscape" );

    attr = parseOrientation("Orient", "Landscape");
    orientation = (AttrOrientation*)attr.get();
    REQUIRE( orientation->orientation == Orientation::Landscape );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "landscape" );
    REQUIRE( attr->sdpLine() == "orient:landscape" );

    attr = parseOrientation("orient", "portrait");
    orientation = (AttrOrientation*)attr.get();
    REQUIRE( orientation->orientation == Orientation::Portrait );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "portrait" );
    REQUIRE( attr->sdpLine() == "orient:portrait" );

    attr = parseOrientation("Orient", "Portrait");
    orientation = (AttrOrientation*)attr.get();
    REQUIRE( orientation->orientation == Orientation::Portrait );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "portrait" );
    REQUIRE( attr->sdpLine() == "orient:portrait" );

    attr = parseOrientation("orient", "seascape");
    orientation = (AttrOrientation*)attr.get();
    REQUIRE( orientation->orientation == Orientation::Seascape );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "seascape" );
    REQUIRE( attr->sdpLine() == "orient:seascape" );

    attr = parseOrientation("Orient", "Seascape");
    orientation = (AttrOrientation*)attr.get();
    REQUIRE( orientation->orientation == Orientation::Seascape );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "seascape" );
    REQUIRE( attr->sdpLine() == "orient:seascape" );

    REQUIRE_THROWS( parseOrientation("orient", "invalid") );
    REQUIRE_THROWS( parseOrientation("orient", "") );
}

TEST_CASE("Parse Category Attribute", "[Category Attribute]"){
    sp<Attribute> attr = parseCategory("cat", "some category");
    REQUIRE( attr->key() == "cat" );
    REQUIRE( attr->value() == "some category" );
    REQUIRE( attr->sdpLine() == "cat:some category" );

    attr = parseCategory("Cat", "Another Category");
    REQUIRE( attr->key() == "cat" );
    REQUIRE( attr->value() == "Another Category" );
    REQUIRE( attr->sdpLine() == "cat:Another Category" );

    attr = parseCategory("cat", "");
    REQUIRE( attr->key() == "cat" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "cat:" );
}

TEST_CASE("Parse Keywords Attribute", "[Keywords Attribute]"){
    sp<Attribute> attr = parseKeywords("keywds", "a few keywords");
    REQUIRE( attr->key() == "keywds" );
    REQUIRE( attr->value() == "a few keywords" );
    REQUIRE( attr->sdpLine() == "keywds:a few keywords" );

    attr = parseKeywords("Keywds", "Some Other Keywords");
    REQUIRE( attr->key() == "keywds" );
    REQUIRE( attr->value() == "Some Other Keywords" );
    REQUIRE( attr->sdpLine() == "keywds:Some Other Keywords" );

    attr = parseKeywords("keywds", "");
    REQUIRE( attr->key() == "keywds" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "keywds:" );
}

TEST_CASE("Parse Tool Attribute", "[Tool Attribute]"){
    sp<Attribute> attr = parseTool("tool", "tool name");
    REQUIRE( attr->key() == "tool" );
    REQUIRE( attr->value() == "tool name" );
    REQUIRE( attr->sdpLine() == "tool:tool name" );

    attr = parseTool("Tool", "Another Tool Name");
    REQUIRE( attr->key() == "tool" );
    REQUIRE( attr->value() == "Another Tool Name" );
    REQUIRE( attr->sdpLine() == "tool:Another Tool Name" );

    attr = parseTool("tool", "");
    REQUIRE( attr->key() == "tool" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "tool:" );
}

TEST_CASE("Parse Conference Type Attribute", "[Converence Type Attribute]"){
    sp<Attribute> attr = parseConferenceType("type", kConferenceType_Broadcast);
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == kConferenceType_Broadcast );
    REQUIRE( attr->sdpLine() == string("type:") + kConferenceType_Broadcast );

    attr = parseConferenceType("type", kConferenceType_Meeting );
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == kConferenceType_Meeting );
    REQUIRE( attr->sdpLine() == string("type:") + kConferenceType_Meeting );

    attr = parseConferenceType("type", kConferenceType_Moderated );
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == kConferenceType_Moderated );
    REQUIRE( attr->sdpLine() == string("type:") + kConferenceType_Moderated );

    attr = parseConferenceType("type", kConferenceType_Test );
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == kConferenceType_Test );
    REQUIRE( attr->sdpLine() == string("type:") + kConferenceType_Test );

    attr = parseConferenceType("type", kConferenceType_H332 );
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == kConferenceType_H332 );
    REQUIRE( attr->sdpLine() == string("type:") + kConferenceType_H332 );

    attr = parseConferenceType("Type", "something Else");
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == "something Else" );
    REQUIRE( attr->sdpLine() == "type:something Else" );

    attr = parseConferenceType("Type", "");
    REQUIRE( attr->key() == "type" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "type:" );
}

TEST_CASE("Parse Charset Attribute", "[Charset Attribute]"){
    sp<Attribute> attr = parseCharset("charset", "iso-10646");
    REQUIRE( attr->key() == "charset" );
    REQUIRE( attr->value() == "iso-10646" );
    REQUIRE( attr->sdpLine() == "charset:iso-10646" );

    attr = parseCharset("Charset", "iso-8859-1");
    REQUIRE( attr->key() == "charset" );
    REQUIRE( attr->value() == "iso-8859-1" );
    REQUIRE( attr->sdpLine() == "charset:iso-8859-1" );
}

TEST_CASE("Parse SDP Language Attribute", "[SDP Language Attribute]"){
    sp<Attribute> attr = parseSdpLanguage("sdplang", "en-us");
    REQUIRE( attr->key() == "sdplang" );
    REQUIRE( attr->value() == "en-us" );
    REQUIRE( attr->sdpLine() == "sdplang:en-us" );

    attr = parseSdpLanguage("sdpLang", "ga-gb");
    REQUIRE( attr->key() == "sdplang" );
    REQUIRE( attr->value() == "ga-gb" );
    REQUIRE( attr->sdpLine() == "sdplang:ga-gb" );
}

TEST_CASE("Parse Language Attribute", "[Language Attribute]"){
    sp<Attribute> attr = parseLanguage("lang", "en-us");
    REQUIRE( attr->key() == "lang" );
    REQUIRE( attr->value() == "en-us" );
    REQUIRE( attr->sdpLine() == "lang:en-us" );

    attr = parseLanguage("Lang", "ga-gb");
    REQUIRE( attr->key() == "lang" );
    REQUIRE( attr->value() == "ga-gb" );
    REQUIRE( attr->sdpLine() == "lang:ga-gb" );
}

TEST_CASE("Parse Framerate Attribute", "[Framerate Attribute]"){
    sp<Attribute> attr = parseFramerate("framerate", "29.97");
    AttrFramerate* frAttr = (AttrFramerate*)attr.get();

    REQUIRE( fabs(frAttr->framerate - 29.97) < 0.001 );
    REQUIRE( attr->key() == "framerate" );
    REQUIRE( attr->value() == "29.97" );
    REQUIRE( attr->sdpLine() == "framerate:29.97" );


    attr = parseFramerate("framerate", "24");
    frAttr = (AttrFramerate*)attr.get();

    REQUIRE( fabs(frAttr->framerate - 24) < 0.001 );
    REQUIRE( attr->key() == "framerate" );
    REQUIRE( attr->value() == "24" );
    REQUIRE( attr->sdpLine() == "framerate:24" );


    attr = parseFramerate("framerate", "23.976");
    frAttr = (AttrFramerate*)attr.get();

    REQUIRE( fabs(frAttr->framerate - 23.976) < 0.001 );
    REQUIRE( attr->key() == "framerate" );
    REQUIRE( attr->value() == "23.976" );
    REQUIRE( attr->sdpLine() == "framerate:23.976" );
}

TEST_CASE("Parse Quality Attribute", "[Quality Attribute]"){
    sp<Attribute> attr = parseQuality("quality", "10");
    AttrQuality* qAttr = (AttrQuality*)attr.get();

    REQUIRE( qAttr->quality == 10 );
    REQUIRE( attr->key() == "quality" );
    REQUIRE( attr->value() == "10" );
    REQUIRE( attr->sdpLine() == "quality:10" );

    attr = parseQuality("quality", "0");
    qAttr = (AttrQuality*)attr.get();

    REQUIRE( qAttr->quality == 0 );
    REQUIRE( attr->key() == "quality" );
    REQUIRE( attr->value() == "0" );
    REQUIRE( attr->sdpLine() == "quality:0" );

    attr = parseQuality("Quality", "5.0");
    qAttr = (AttrQuality*)attr.get();

    REQUIRE( qAttr->quality == 5 );
    REQUIRE( attr->key() == "quality" );
    REQUIRE( attr->value() == "5" );
    REQUIRE( attr->sdpLine() == "quality:5" );
}

TEST_CASE("Parse Format Parameters Attribute", "[Format Parameters Attribute]"){
    const string paramStr = "param1 param2 param3;param4/param5";
    const uint8_t payloadType = 96;
    const string fullStr = to_string(payloadType) + " " + paramStr;

    sp<Attribute> attr = parseFormatParams("fmtp", fullStr);

    AttrFormatParams* fmtAttr = (AttrFormatParams*)attr.get();

    REQUIRE( fmtAttr->formatParams == paramStr );
    REQUIRE( fmtAttr->payloadType == payloadType );
    REQUIRE( attr->key() == "fmtp" );
    REQUIRE( attr->value() == fullStr );
    REQUIRE( attr->sdpLine() == "fmtp:" + fullStr );
}

TEST_CASE("Parse RTP Map Attribute", "[RTP Map Attribute]"){
    uint8_t payloadType = 98;
    string format = "L16";
    int clockRate = 16000;
    int channelCount = 2;

    string attrStr = to_string(payloadType)
                        + " "
                        + format
                        + "/"
                        + to_string(clockRate)
                        + "/"
                        + to_string(channelCount);

    sp<Attribute> attr = parseRtpMap("rtpmap", attrStr);
    AttrRtpMap* rtpmap = (AttrRtpMap*)attr.get();

    REQUIRE( rtpmap->encodingName == format );
    REQUIRE( rtpmap->audioChannelCount == channelCount );
    REQUIRE( rtpmap->clockRate == clockRate );
    REQUIRE( attr->key() == "rtpmap" );
    REQUIRE( attr->value() == attrStr );
    REQUIRE( attr->sdpLine() == "rtpmap:" + attrStr );

    channelCount = 1;
    attrStr = to_string(payloadType)
                + " "
                + format
                + "/"
                + to_string(clockRate);

    attr = parseRtpMap("rtpmap", attrStr);
    rtpmap = (AttrRtpMap*)attr.get();

    REQUIRE( rtpmap->encodingName == format );
    REQUIRE( rtpmap->audioChannelCount == channelCount );
    REQUIRE( rtpmap->clockRate == clockRate );
    REQUIRE( attr->key() == "rtpmap" );
    REQUIRE( attr->value() == attrStr );
    REQUIRE( attr->sdpLine() == "rtpmap:" + attrStr );
}

TEST_CASE("Test PTime Attribute", "[PTime Attribute]"){
    sp<Attribute> attr = parsePTime("ptime", "256");
    AttrPTime* ptime = (AttrPTime*)attr.get();
    REQUIRE( ptime->packetDuration == 256 );
    REQUIRE( attr->key() == "ptime" );
    REQUIRE( attr->value() == "256" );
    REQUIRE( attr->sdpLine() == "ptime:256" );
}

TEST_CASE("Test Max PTime Attribute", "[Max PTime Attribute]"){
    sp<Attribute> attr = parseMaxPTime("maxptime", "256");
    AttrMaxPTime* maxPTime = (AttrMaxPTime*)attr.get();
    REQUIRE( maxPTime->maxPacketDuration == 256 );
    REQUIRE( attr->key() == "maxptime" );
    REQUIRE( attr->value() == "256" );
    REQUIRE( attr->sdpLine() == "maxptime:256" );
}

TEST_CASE("Test Attribute Detection", "[Attribute Detection]"){
    sp<Attribute> attr = parseAttribute("orient:landscape");
    AttrOrientation* orientation = (AttrOrientation*)attr.get();
    REQUIRE( attr.get() != NULL );
    REQUIRE( orientation->orientation == Orientation::Landscape );
    REQUIRE( attr->key() == "orient" );
    REQUIRE( attr->value() == "landscape" );
    REQUIRE( attr->sdpLine() == "orient:landscape" );

    attr = parseAttribute("sendrecv");
    REQUIRE( attr.get() != NULL );
    REQUIRE( attr->key() == "sendrecv" );
    REQUIRE( attr->value() == "" );
    REQUIRE( attr->sdpLine() == "sendrecv" );
}


