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

#include <zsdp/sdp.h>
#include "string-util.h"
#include "net-util.h"
#include <sstream>


using namespace std;

namespace zsdp{


static const string CRLF = "\r\n";




string sdpToString(const Sdp* sdp) {
    ostringstream out;
    out << "v=" << sdp->version << CRLF;

    out << "o=" << sdp->origin.toString() << CRLF;

    if(sdp->sessionName.empty())
        out << "s= " << CRLF;
    else
        out << "s=" << sdp->sessionName << CRLF;

    if(!sdp->sessionInformation.empty())
        out << "i=" << sdp->sessionInformation << CRLF;

    if(!sdp->uri.empty())
        out << "u=" << sdp->uri << CRLF;

    if(!sdp->email.empty())
        out << "e=" << sdp->email << CRLF;

    if(!sdp->phoneNumber.empty())
        out << "p=" << sdp->phoneNumber << CRLF;

    if(sdp->connectionData.addressType != AddressType::NotSet)
        out << "c=" << sdp->connectionData.toString() << CRLF;

    if(sdp->bandwidth.type != BandwidthType::NotSet)
        out << "b=" << sdp->bandwidth.toString() << CRLF;

    if(!sdp->timeZoneAdjustments.empty()){
        out << "z=";

        for(size_t i = 0; i < sdp->timeZoneAdjustments.size(); i++){
            if(i > 0)
                out << " ";

            out << to_string(sdp->timeZoneAdjustments[i].adjustAtTime)
                << " "
                << to_string(sdp->timeZoneAdjustments[i].adjustment);
        }

        out << CRLF;
    }

    if(sdp->encryption.type != EncryptionType::NotSet)
        out << "k=" + sdp->encryption.toString() << CRLF;

    for(size_t i = 0; i < sdp->attributes.size(); i++)
        out << "a=" + sdp->attributes[i]->sdpLine() << CRLF;

    if(sdp->times.empty())
        out << "t=0 0" << CRLF;
    else{
        for(size_t i = 0; i < sdp->times.size(); i++){
            out << "t=" << sdp->times[i].toString() << CRLF;

            const vector<RepeatingTime>& repeatingTimes = sdp->times[i].repeatingTimes;
            for(size_t j = 0; j < repeatingTimes.size(); j++)
                out << "r=" << repeatingTimes[j].toString() << CRLF;
        }
    }

    for(size_t i = 0; i < sdp->streams.size(); i++)
        out << sdp->streams[i].sdpLines();

    return out.str();
}

static string dashIfEmpty(string s){
    if(s.empty())
        return "-";
    else
        return s;
}

static string nullAddrIfEmpty(string s, AddressType t){
    if(s.empty()){
        if(t == AddressType::IP4)
            return "0.0.0.0";
        else if(t == AddressType::IP6)
            return "::";
        else
            throw invalid_argument("Unknown Address Type " + to_string((uint32_t)t));
    }

    return s;
}

string Origin::toString() const{

    ostringstream out;
    out << dashIfEmpty(username)
        << " " << dashIfEmpty(sessionID)
        << " " << dashIfEmpty(sessionVersion)
        << " " << networkTypeToString(networkType)
        << " " << addressTypeToString(addressType)
        << " " << nullAddrIfEmpty(host, addressType);

    return out.str();
}
string ConnectionData::toString() const {
    ostringstream out;

    out << networkTypeToString(networkType)
        << " " << addressTypeToString(addressType)
        << " " << nullAddrIfEmpty(host, addressType);

    return out.str();
}

static string bandwidthTypeToStr(BandwidthType type){
    switch(type){
        case BandwidthType::ConferenceTotal: return "CT";
        case BandwidthType::ApplicationSpecific: return "AS";
        default: return "";
    }
}

string Bandwidth::toString() const {
    return bandwidthTypeToStr(type) + ":" + to_string(kbps);
}

static string encTypeToStr(EncryptionType type){
    switch(type){
        case EncryptionType::Base64: return "base64";
        case EncryptionType::Clear: return "clear";
        case EncryptionType::PromptForKey: return "prompt";
        case EncryptionType::URI: return "uri";
        default: return "";
    }
}

string Encryption::toString() const {
    if(type == EncryptionType::PromptForKey)
        return encTypeToStr(type);

    return encTypeToStr(type) + ":" + key;
}

string Stream::sdpLines() const {
    ostringstream out;

    out << "m=" << mediaDescription.toString() << CRLF;

    if(!title.empty())
        out << "i=" << title << CRLF;

    if(connectionData.addressType != AddressType::NotSet)
        out << "c=" << connectionData.toString() << CRLF;

    if(bandwidth.type != BandwidthType::NotSet)
        out << "b=" << bandwidth.toString() << CRLF;

    if(encryption.type != EncryptionType::NotSet)
        out << "k=" << encryption.toString() << CRLF;

    for(auto& attr : attributes)
        out << "a=" << attr->sdpLine() << CRLF;

    return out.str();
}

string Timing::toString() const {
    ostringstream out;
    out << start << " " << end;

    return out.str();
}

string RepeatingTime::toString() const {
    ostringstream out;
    out << interval << " " << duration;

    for(size_t i = 0; i < offsetsFromStartTime.size(); i++)
        out << " " << offsetsFromStartTime[i];

    return out.str();
}

string MediaDescription::toString() const {
    ostringstream out;

    out << mediaTypeToString(mediaType)
        << " "
        << port;

    if(portCount > 1)
        out << "/" << portCount;

    out << " "
        << protocolToString(protocol);


    if(protocol == Protocol::RTP_AVP || protocol == Protocol::RTP_SAVP){
        if(payloadTypes.empty())
            throw invalid_argument("At least one payload type must be specified when using RTP/AVP or RTP/SAVP.");

        for(auto pt : payloadTypes){
            out << " " << pt;
        }
    }
    else if(protocol == Protocol::UnknownUDP){
        if(codec.empty())
            throw invalid_argument("The codec must be specified when using raw UDP.");

        out << " " << codec;
    }
    else
        throw invalid_argument("Unknown protocol.");

    return out.str();
}


std::string mediaTypeToString(MediaType t){
    switch(t){
        case MediaType::Video: return "video";
        case MediaType::Audio: return "audio";
        case MediaType::Application: return "application";
        case MediaType::Message: return "message";
        case MediaType::Text: return "text";
        default:
            throw invalid_argument("Unknown media type " + to_string((uint32_t)t));
    }
}

std::string networkTypeToString(NetworkType t){
    if(t != NetworkType::IN)
        throw invalid_argument("Unknown Network Type " + to_string((uint32_t)t));

    return "IN";
}

std::string addressTypeToString(AddressType t){
    switch(t){
        case AddressType::IP4: return "IP4";
        case AddressType::IP6: return "IP6";
        default:
            throw invalid_argument("Unknown Address Type " + to_string((uint32_t)t));
    }
}

std::string protocolToString(Protocol p){
    switch(p){
        case Protocol::RTP_AVP: return "RTP/AVP";
        case Protocol::RTP_SAVP: return "RTP/SAVP";
        case Protocol::UnknownUDP: return "udp";
        default:
            throw invalid_argument("Unknown protocol " + to_string((uint32_t)p));
    }
}

} // namespace zsdp
