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

#ifndef __ZSDP_PARSING_H__
#define __ZSDP_PARSING_H__

#include <zsdp/sdp.h>
#include <string>
#include <vector>


namespace zsdp {

Sdp parseSdp(const std::string& sdpString);

uint32_t parseVersion(const std::string& line);

NetworkType networkTypeForStr(const std::string& s);

AddressType addressTypeForStr(const std::string& s);

Origin parseOrigin(const std::string& line);

ConnectionData parseConnectionData(const std::string& line);

BandwidthType bandwidthTypeForStr(const std::string& s);

Bandwidth parseBandwidth(const std::string& line);

Timing parseTiming(const std::string& s);

int64_t parseTimeStrSigned(const std::string& s);

uint64_t parseTimeStrUnsigned(const std::string& s);

RepeatingTime parseRepeatingTime(const std::string& s);

std::vector<TimeZoneAdjustment> parseTimeZone(const std::string& s);

MediaType mediaTypeForStr(const std::string& s);

MediaDirection mediaDirectionForStr(const std::string& s);

Protocol protocolForStr(const std::string& s);

Stream parseMediaDesc(const std::string& line);

EncryptionType encryptionTypeForStr(const std::string& s);

Encryption parseEncryption(const std::string& s);

uint8_t parsePayloadType(const std::string& s);

Orientation orientationForStr(const std::string& s);

} // namespace zsdp

#endif /* __ZSDP_PARSING_H__ */
