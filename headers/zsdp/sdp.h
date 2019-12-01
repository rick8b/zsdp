/*
 * Copyright 2019 Richard Kern <kernrj@gmail.com>
 *
 * Copyright 2019 Richard J Kern II
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

#ifndef __ZSDP_SDP_H__
#define __ZSDP_SDP_H__

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <zsdp/attributes.h>
#include <zsdp/defs.h>


namespace zsdp {

enum class NetworkType {
    NotSet,
    IN
};

std::string networkTypeToString(NetworkType t);


enum class AddressType {
    NotSet,
    IP4,
    IP6,
};

std::string addressTypeToString(AddressType t);


enum class EncryptionType {
    NotSet,
    Clear,
    Base64,
    URI,
    PromptForKey,
};

std::string encryptionTypeToString(EncryptionType t);


enum class MediaType {
    NotSet,
    Video,
    Audio,
    Text,
    Application,
    Message,
};

std::string mediaTypeToString(MediaType t);


enum class Protocol {
    NotSet,
    RTP_AVP,
    RTP_SAVP,
    UnknownUDP,
};

std::string protocolToString(Protocol p);


constexpr uint32_t VersionNumber = 0;
constexpr const char* MimeType = "application/sdp";

struct Origin {
    std::string username;
    std::string sessionID;
    std::string sessionVersion;
    NetworkType networkType = NetworkType::IN;
    AddressType addressType = AddressType::IP4;
    std::string host;

    std::string toString() const;
};

struct ConnectionData {
    NetworkType networkType = NetworkType::IN;
    AddressType addressType = AddressType::IP4;
    std::string host;
    uint8_t multicastIPv4TTL = 0; // Only used for IPv4 Multicast addresses
    uint32_t multicastAddressCount = 0; // Only used for Multicast addresses

    std::string toString() const;
};

struct MediaDescription {
    MediaType mediaType = MediaType::NotSet;
    uint16_t port = 0;
    uint16_t portCount = 0;
    Protocol protocol = Protocol::NotSet;
    std::vector<uint8_t> payloadTypes; /// For RTP/AVP and RTP/SAVP only, typically only one payload type
    std::string codec; /// For udp only (Not used with RTP/AVP or RTP/SAVP).

    std::string toString() const;
};

struct RepeatingTime {
    uint64_t interval = 0; // e.g. 7 * 24 * 3600 means every week
    uint64_t duration = 0; // duration of the media playback in seconds
    std::vector<uint64_t> offsetsFromStartTime; //one or more offsets from the start of this time period

    std::string toString() const;
};

struct Timing {
    uint64_t start = 0; // Times are NTP: seconds since January 1, 1900.
    uint64_t end = 0; // set to 0 for a session without a defined end-time.
    std::vector<RepeatingTime> repeatingTimes;

    std::string toString() const;
};




/**
 * Generic way to handle daylight savings. At adjustAtTime, add adjustment to the current clock.
 */
struct TimeZoneAdjustment {
    uint64_t adjustAtTime = 0;
    int64_t adjustment = 0;

    std::string toString() const;
};

struct Encryption {
    EncryptionType type = EncryptionType::NotSet;
    std::string key; // ignored for some encryption types (e.g. "prompt")

    std::string toString() const;
};


enum class BandwidthType {
    NotSet,
    ConferenceTotal,
    ApplicationSpecific,
};

struct Bandwidth {
    BandwidthType type = BandwidthType::NotSet;
    uint64_t kbps = 0;

    std::string toString() const;
};

struct Stream {
    MediaDescription mediaDescription;
    std::string title;
    std::vector<sp<Attribute>> attributes;
    ConnectionData connectionData;
    Bandwidth bandwidth;
    Encryption encryption;

    std::string sdpLines() const;
};


struct Sdp {
    uint32_t version = VersionNumber; // v=
    Origin origin; // o=
    std::string sessionName; // s=
    std::string sessionInformation; // i=
    std::string uri; // u=
    std::string email; // e=
    std::string phoneNumber; // p=

    ConnectionData connectionData; // c=
    Bandwidth bandwidth; // b=
    std::vector<Timing> times; // t=, start and end time(s), use repeatTimes for regularly repeating times. Timing also contains repeating times (r=)

    std::vector<TimeZoneAdjustment> timeZoneAdjustments;
    Encryption encryption;

    std::vector<sp<Attribute>> attributes;

    std::vector<Stream> streams; // m=
};

std::string sdpToString(const Sdp* sdp);
Sdp parseSdp(const std::string& sdpStr);

} // namespace zsdp

#endif // __ZSDP_SDP_H__
