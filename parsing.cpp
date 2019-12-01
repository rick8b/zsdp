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

#include "parsing.h"
#include "string-util.h"
#include "net-util.h"


using namespace std;


namespace zsdp {

uint32_t parseVersion(const string& line){
    uint64_t version = stou32(line);

    return (uint32_t)version;
}

NetworkType networkTypeForStr(const string& s){
    if(strToUpper(s) == "IN")
        return NetworkType::IN;

    throw invalid_argument("Unrecognized network type '" + s + "'.");
}

AddressType addressTypeForStr(const string& s){
    string uc = strToUpper(s);
    if(uc == "IP4")
        return AddressType::IP4;
    else if(uc == "IP6")
        return AddressType::IP6;
    else
        throw invalid_argument("Unrecognized address type '" + s + "'.");
}

Origin parseOrigin(const string& line){
    Origin origin;
    vector<string> tokens = split(line, " ");
    if(tokens.size() < 6)
        throw invalid_argument("Failed to parse origin.");

    origin.username = tokens[0];
    origin.sessionID = tokens[1];
    origin.sessionVersion = tokens[2];
    origin.networkType = networkTypeForStr(tokens[3]);
    origin.addressType = addressTypeForStr(tokens[4]);
    origin.host = tokens[5];

    return origin;
}

ConnectionData parseConnectionData(const string& line)
{
    ConnectionData cd;
    vector<string> tokens = split(line, " ");
    if(tokens.size() < 3){
        throw invalid_argument("Error parrsing connection data: " + line);
    }

    cd.networkType = networkTypeForStr(tokens[0]);
    cd.addressType = addressTypeForStr(tokens[1]);
    cd.host = tokens[2];

    return cd;
}

BandwidthType bandwidthTypeForStr(const string& s){
    string uc = strToUpper(s);

    if(uc == "CT")
        return BandwidthType::ConferenceTotal;
    else if(uc == "AS")
        return BandwidthType::ApplicationSpecific;

    throw invalid_argument("Unrecognized bandwidth type '" + s + "'.");
}

Bandwidth parseBandwidth(const string& line){
    Bandwidth bandwidth;

    vector<string> tokens = split(line, ":");
    if(tokens.size() != 2)
        throw invalid_argument("Error parsing bandwidth '" + line + "'.");

    bandwidth.type = bandwidthTypeForStr(tokens[0]);
    bandwidth.kbps = stoull(tokens[1]);

    return bandwidth;
}

Timing parseTiming(const string& s){
    vector<string> timeStrs = split(s, " ");
    if(timeStrs.size() != 2)
        throw invalid_argument("Error parsing timing '" + s + "'.");

    Timing t;
    t.start = stoull(timeStrs[0]);
    t.end = stoull(timeStrs[1]);

    return t;
}

int64_t parseTimeStrSigned(const string& s){
    char lastCh = *s.rend();
    int64_t multiplier = 1;

    string numStr = s;
    if(lastCh < '0' || lastCh > '9')
        numStr = numStr.substr(0, numStr.size() - 1);

    if(lastCh == 'd')
        multiplier = 3600 * 24;
    else if(lastCh == 'h')
        multiplier = 3600;
    else if(lastCh == 'm')
        multiplier = 60;
    else if(lastCh == 's')
        multiplier = 1;

    return stoll(numStr) * multiplier;
}

uint64_t parseTimeStrUnsigned(const string& s){
    char lastCh = *s.rend();
    uint64_t multiplier = 1;

    string numStr = s;
    if(lastCh < '0' || lastCh > '9')
        numStr = numStr.substr(0, numStr.size() - 1);

    if(lastCh == 'd')
        multiplier = 3600 * 24;
    else if(lastCh == 'h')
        multiplier = 3600;
    else if(lastCh == 'm')
        multiplier = 60;
    else if(lastCh == 's')
        multiplier = 1;

    return stoull(numStr) * multiplier;
}

RepeatingTime parseRepeatingTime(const string& s){
    auto timeStrs = split(s, " ");
    if(timeStrs.size() < 3)
        throw invalid_argument("Error parsing repeating times '" + s + "'.");

    RepeatingTime rt;
    rt.interval = parseTimeStrUnsigned(timeStrs[0]);
    rt.duration = parseTimeStrUnsigned(timeStrs[1]);
    for(size_t i = 2; i < timeStrs.size(); i++)
        rt.offsetsFromStartTime.push_back(parseTimeStrUnsigned(timeStrs[i]));

    return rt;
}

vector<TimeZoneAdjustment> parseTimeZone(const string& s){
    vector<string> timeStrs = split(s, " ");
    if(timeStrs.size() % 2 != 0)
        throw invalid_argument("Error parsing time zone information '" + s + "'.");


    vector<TimeZoneAdjustment> adjustments(timeStrs.size() / 2);

    for(size_t i = 0; i < timeStrs.size(); i += 2){
        TimeZoneAdjustment adj;
        adj.adjustAtTime = stoull(timeStrs[i]);
        adj.adjustment = parseTimeStrSigned(timeStrs[i + 1]);
        adjustments[i / 2] = adj;
    }

    return adjustments;
}

MediaType mediaTypeForStr(const string& s){
    string mtStr = strToLower(s);
    if(mtStr == "video")
        return MediaType::Video;
    else if(mtStr == "audio")
        return MediaType::Audio;
    else if(mtStr == "text")
        return MediaType::Text;
    else if(mtStr == "application")
        return MediaType::Application;
    else if(mtStr == "message")
        return MediaType::Message;

    throw invalid_argument("Unrecognized media type '" + s + "'.");
}

Protocol protocolForStr(const string& s){
    string pStr = strToUpper(s);
    if(pStr == "RTP/AVP")
        return Protocol::RTP_AVP;
    else if(pStr == "RTP/SAVP")
        return Protocol::RTP_SAVP;
    else if(pStr == "UDP")
        return Protocol::UnknownUDP;

    throw invalid_argument("Unknown protocol '" + s + "'.");
}

Stream parseMediaDesc(const string& line){
    auto tokens = split(line, " ");
    if(tokens.size() < 4)
        throw invalid_argument("Error parsing media description '" + line + "'.");

    Stream stream;
    stream.mediaDescription.mediaType = mediaTypeForStr(tokens[0]);
    stream.mediaDescription.port = stou16(tokens[1]);

    Protocol protocol = protocolForStr(tokens[2]);
    stream.mediaDescription.protocol = protocol;

    if(protocol == Protocol::RTP_AVP || protocol == Protocol::RTP_SAVP){
        for(size_t i = 3; i < tokens.size(); i++)
            stream.mediaDescription.payloadTypes.push_back(parsePayloadType(tokens[i]));
    }
    else if(protocol == Protocol::UnknownUDP){
        stream.mediaDescription.codec = tokens[3];
    }

    return stream;
}

EncryptionType encryptionTypeForStr(const string& s){
    string encType = strToLower(s);
    if(encType == "prompt")
        return EncryptionType::PromptForKey;
    else if(encType == "clear")
        return EncryptionType::Clear;
    else if(encType == "base64")
        return EncryptionType::Base64;
    else if(encType == "uri")
        return EncryptionType::URI;

    throw invalid_argument("Unrecognized encryption type '" + s + "'.");
}

Encryption parseEncryption(const string& line){
    Encryption enc;
    auto tokens = split(line, ":", 2);
    string methodStr = tokens[0];
    string keyStr = tokens.size() > 1 ? tokens[1] : "";

    enc.type = encryptionTypeForStr(methodStr);
    enc.key = keyStr;

    return enc;
}

Sdp parseSdp(const string& sdpString){
    Sdp sdp;
    Stream* currStream = NULL;

    if(sdpString.size() < 2 || sdpString.compare(0, 2, "v=") != 0)
        throw runtime_error("Not an SDP.");

    vector<string> lines = split(sdpString, "\r\n");

    for(size_t i = 0; i < lines.size(); i++){
        const string& line = lines[i];
        if(line.empty())
            continue;

        if(line.size() < 2){
            fprintf(stderr, "Failed to parse SDP line '%s'. Ignoring.\n", line.c_str());
            continue;
        }

        if(line[1] != '='){
            fprintf(stderr, "SDP syntax incorrect for line '%s'. Ignoring.\n", line.c_str());
            continue;
        }

        char code = tolower(line[0]);
        string restOfLine = line.substr(2);

        if(code == 'm'){
            sdp.streams.push_back(parseMediaDesc(restOfLine));
            currStream = &*sdp.streams.rbegin();
        }
        else if(currStream == NULL){ // session-level parameters
            switch(code){
                case 'v': sdp.version = parseVersion(restOfLine); break;
                case 'o': sdp.origin = parseOrigin(restOfLine); break;
                case 's': sdp.sessionName = restOfLine; break;
                case 'i': sdp.sessionInformation = restOfLine; break;
                case 'u': sdp.uri = restOfLine; break;
                case 'e': sdp.email = restOfLine; break;
                case 'p': sdp.phoneNumber = restOfLine; break;
                case 'c': sdp.connectionData = parseConnectionData(restOfLine); break;
                case 'b': sdp.bandwidth = parseBandwidth(restOfLine); break;
                case 't': sdp.times.push_back(parseTiming(restOfLine)); break;
                case 'z':
                {
                    vector<TimeZoneAdjustment> adjustments = parseTimeZone(restOfLine);
                    for(size_t i = 0; i < adjustments.size(); i++)
                        sdp.timeZoneAdjustments.push_back(adjustments[i]);

                    break;
                }
                case 'r':
                {
                    if(sdp.times.empty())
                        throw invalid_argument("Invalid SDP: r= without t=.");

                    Timing& timing = *sdp.times.end();
                    RepeatingTime rt = parseRepeatingTime(restOfLine);

                    timing.repeatingTimes.push_back(rt);
                    break;
                }
                case 'a':
                    sdp.attributes.push_back(parseAttribute(restOfLine));
                    break;

                default:
                    fprintf(stderr, "Unexpected session-level code '%c': Ignoring.\n", code);
            }
        }
        else{
            switch(code){
                case 'i': currStream->title = restOfLine;
                case 'c': currStream->connectionData = parseConnectionData(restOfLine);
                case 'b': currStream->bandwidth = parseBandwidth(restOfLine);
                case 'k': currStream->encryption = parseEncryption(restOfLine);
                case 'a':
                    currStream->attributes.push_back(parseAttribute(restOfLine));
                default:
                    fprintf(stderr, "Unexpected media-level code '%c': Ignoring.\n", code);
            }
        }
    }

    return sdp;
}

MediaDirection mediaDirectionForStr(const std::string& s){
    string lc = strToLower(s);
    if(lc == "sendrecv")
        return MediaDirection::SendRecv;
    else if(lc == "sendonly")
        return MediaDirection::SendOnly;
    else if(lc == "recvonly")
        return MediaDirection::RecvOnly;
    else if(lc == "inactive")
        return MediaDirection::Inactive;

    throw invalid_argument("Unrecognized media direction '" + s + "'.");
}

uint8_t parsePayloadType(const std::string& s){
    int payloadType = stoi(s);

    if(payloadType < 0 || payloadType > kMaxPayloadType)
        throw out_of_range("Payload type " + s + " is out-of-range.");

    return (uint8_t)payloadType;
}

Orientation orientationForStr(const string& s){
    string lc = strToLower(s);

    if(lc == "landscape")
        return Orientation::Landscape;
    else if(lc == "portrait")
        return Orientation::Portrait;
    else if(lc == "seascape")
        return Orientation::Seascape;

    throw invalid_argument("Unknown video orientation '" + s + "'.");
}

} // namespace zsdp
