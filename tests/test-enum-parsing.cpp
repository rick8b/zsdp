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
#include "../parsing.h"

using namespace zsdp;

TEST_CASE("Parse SDP Version", "[SDP Version]"){
    REQUIRE( parseVersion("0") == 0 );
}

TEST_CASE("Parse Network Type", "[Network Type]"){
    REQUIRE( networkTypeForStr("IN") == NetworkType::IN );
    REQUIRE( networkTypeForStr("in") == NetworkType::IN );
}

TEST_CASE("Parse Address Type", "[Address Type]"){
    REQUIRE( addressTypeForStr("IP4") == AddressType::IP4 );
    REQUIRE( addressTypeForStr("ip4") == AddressType::IP4 );
    REQUIRE( addressTypeForStr("IP6") == AddressType::IP6 );
    REQUIRE( addressTypeForStr("ip6") == AddressType::IP6 );
}

TEST_CASE("Parse Bandwidth Type", "[Bandwidth Type]"){
    REQUIRE( bandwidthTypeForStr("CT") == BandwidthType::ConferenceTotal );
    REQUIRE( bandwidthTypeForStr("ct") == BandwidthType::ConferenceTotal );
    REQUIRE( bandwidthTypeForStr("AS") == BandwidthType::ApplicationSpecific );
    REQUIRE( bandwidthTypeForStr("as") == BandwidthType::ApplicationSpecific );
}


TEST_CASE("Parse Media Direction", "[Media Direction]"){
    REQUIRE( mediaDirectionForStr("sendrecv") == MediaDirection::SendRecv );
    REQUIRE( mediaDirectionForStr("SendRecv") == MediaDirection::SendRecv );
    REQUIRE( mediaDirectionForStr("sendonly") == MediaDirection::SendOnly );
    REQUIRE( mediaDirectionForStr("SendOnly") == MediaDirection::SendOnly );
    REQUIRE( mediaDirectionForStr("recvonly") == MediaDirection::RecvOnly );
    REQUIRE( mediaDirectionForStr("RecvOnly") == MediaDirection::RecvOnly );
    REQUIRE( mediaDirectionForStr("inactive") == MediaDirection::Inactive );
    REQUIRE( mediaDirectionForStr("Inactive") == MediaDirection::Inactive );
}

TEST_CASE("Parse Protocol", "[Protocol]"){
    REQUIRE( protocolForStr("RTP/AVP") == Protocol::RTP_AVP );
    REQUIRE( protocolForStr("rtp/avp") == Protocol::RTP_AVP );
    REQUIRE( protocolForStr("RTP/SAVP") == Protocol::RTP_SAVP );
    REQUIRE( protocolForStr("rtp/savp") == Protocol::RTP_SAVP );
    REQUIRE( protocolForStr("udp") == Protocol::UnknownUDP );
    REQUIRE( protocolForStr("UDP") == Protocol::UnknownUDP );
}

TEST_CASE("Parse Encryption Type", "[Encryption Type]"){
    REQUIRE( encryptionTypeForStr("prompt") == EncryptionType::PromptForKey );
    REQUIRE( encryptionTypeForStr("PROMPT") == EncryptionType::PromptForKey );
    REQUIRE( encryptionTypeForStr("clear") == EncryptionType::Clear );
    REQUIRE( encryptionTypeForStr("CLEAR") == EncryptionType::Clear );
    REQUIRE( encryptionTypeForStr("base64") == EncryptionType::Base64 );
    REQUIRE( encryptionTypeForStr("BASE64") == EncryptionType::Base64 );
    REQUIRE( encryptionTypeForStr("uri") == EncryptionType::URI );
    REQUIRE( encryptionTypeForStr("URI") == EncryptionType::URI );
}

TEST_CASE("Parse Orientation", "[Orientation]"){
    REQUIRE( orientationForStr("landscape") == Orientation::Landscape );
    REQUIRE( orientationForStr("LANDSCAPE") == Orientation::Landscape );
    REQUIRE( orientationForStr("portrait") == Orientation::Portrait );
    REQUIRE( orientationForStr("PORTRAIT") == Orientation::Portrait );
    REQUIRE( orientationForStr("seascape") == Orientation::Seascape );
    REQUIRE( orientationForStr("SEASCAPE") == Orientation::Seascape );
}

TEST_CASE("Payload Type", "[Payload Type]"){
    REQUIRE( parsePayloadType("0") == 0 );
    REQUIRE( parsePayloadType("127") == 127 );
    REQUIRE_THROWS( parsePayloadType("-1") );
    REQUIRE_THROWS( parsePayloadType("128") );
}
