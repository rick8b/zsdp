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
#include <zsdp/attributes.h>
#include "attributes-priv.h"
#include <map>
#include <mutex>
#include "string-util.h"

using namespace std;

namespace zsdp{


#define AMETHODS(CLASS_NAME, KEY)                                \
CLASS_NAME::~CLASS_NAME(){}                                      \
string CLASS_NAME::key() const{ return KEY; }                    \

#define SINGLE_STRING_PARAM_METHODS(CLASS_NAME, PARAM_NAME, KEY) \
AMETHODS(CLASS_NAME, KEY)                                        \
string CLASS_NAME::value() const{ return PARAM_NAME; }


#define SINGLE_NUM_PARAM_METHODS(CLASS_NAME, PARAM_NAME, KEY)    \
AMETHODS(CLASS_NAME, KEY)                                        \
string CLASS_NAME::value() const{ return to_string(PARAM_NAME); }


SINGLE_STRING_PARAM_METHODS(AttrCategory, category, "cat")
SINGLE_STRING_PARAM_METHODS(AttrKeywords, keywords, "keywds")
SINGLE_STRING_PARAM_METHODS(AttrTool, tool, "tool")
SINGLE_STRING_PARAM_METHODS(AttrConferenceType, conferenceType, "type")
SINGLE_STRING_PARAM_METHODS(AttrCharset, charset, "charset")
SINGLE_STRING_PARAM_METHODS(AttrSdpLanguage, language, "sdplang")
SINGLE_STRING_PARAM_METHODS(AttrMediaLanguage, language, "lang")

SINGLE_NUM_PARAM_METHODS(AttrPTime, packetDuration, "ptime")
SINGLE_NUM_PARAM_METHODS(AttrMaxPTime, maxPacketDuration, "maxptime")
AMETHODS(AttrFramerate, "framerate")
string AttrFramerate::value() const {
    char framerateStr[128];
    snprintf(framerateStr, sizeof(framerateStr), "%.3f", framerate);
    if(strchr(framerateStr, '.')){
        size_t offset = strlen(framerateStr) - 1;
        while(framerateStr[offset] == '0')
            framerateStr[offset--] = 0;

        if(framerateStr[offset] == '.')
            framerateStr[offset] = 0;
    }

    return framerateStr;
}

SINGLE_NUM_PARAM_METHODS(AttrQuality, quality, "quality")

static once_flag initAttributesOnceFlag;
static map<string, AttributeParseFnc> attributeParseFncs;


static void initAttributes(){
    attributeParseFncs["sendrecv"] = parseMediaDirection;
    attributeParseFncs["sendonly"] = parseMediaDirection;
    attributeParseFncs["recvonly"] = parseMediaDirection;
    attributeParseFncs["inactive"] = parseMediaDirection;

    attributeParseFncs["cat"] = parseCategory;
    attributeParseFncs["keywds"] = parseKeywords;
    attributeParseFncs["tool"] = parseTool;
    attributeParseFncs["ptime"] = parsePTime;
    attributeParseFncs["maxptime"] = parseMaxPTime;
    attributeParseFncs["rtpmap"] = parseRtpMap;
    attributeParseFncs["orient"] = parseOrientation;
    attributeParseFncs["type"] = parseConferenceType;
    attributeParseFncs["charset"] = parseCharset;
    attributeParseFncs["sdplang"] = parseSdpLanguage;
    attributeParseFncs["lang"] = parseLanguage;
    attributeParseFncs["framerate"] = parseFramerate;
    attributeParseFncs["quality"] = parseQuality;
    attributeParseFncs["fmtp"] = parseFormatParams;
}

void registerAttribute(const string& key,
                       AttributeParseFnc parseFnc)
{
    call_once(initAttributesOnceFlag, initAttributes);

    attributeParseFncs[key] = parseFnc;
}

sp<Attribute> parseAttribute(const string& attrStr){
    call_once(initAttributesOnceFlag, initAttributes);

    vector<string> tokens = split(attrStr, ":", 2);
    string key = tokens[0];
    string val = tokens.size() > 1 ? tokens[1] : "";

    auto it = attributeParseFncs.find(key);
    if(it == attributeParseFncs.end()){
        fprintf(stderr, "No attribute parser for key '%s'.\n", key.c_str());
        return NULL;
    }

    auto parseAttr = it->second;

    return parseAttr(key, val);
}

AttrMediaDirection::~AttrMediaDirection(){
    
}

std::string AttrMediaDirection::sdpLine() const {
    return key();
}

string AttrMediaDirection::key() const{
    switch(direction){
        case MediaDirection::SendRecv: return "sendrecv";
        case MediaDirection::RecvOnly: return "recvonly";
        case MediaDirection::SendOnly: return "sendonly";
        case MediaDirection::Inactive: return "inactive";
        default: return "sendrecv";
    }
}

string AttrMediaDirection::value() const{
    return "";
}



AMETHODS(AttrFormatParams, "fmtp")
string AttrFormatParams::value() const {
    return to_string(payloadType) + " " + formatParams;
}






AttrOrientation asdf;
AMETHODS(AttrOrientation, "orient")
string AttrOrientation::value() const{
    switch(orientation){
        case Orientation::Landscape: return "landscape";
        case Orientation::Portrait:  return "portrait";
        case Orientation::Seascape:  return "seascape";
        default: return "unknown";
    }
}



AMETHODS(AttrRtpMap, "rtpmap")

string AttrRtpMap::value() const {
    std::string val = std::to_string(payloadType)
                      + " "
                      + encodingName
                      + "/"
                      + std::to_string(clockRate);

    if(audioChannelCount > 1)
        val += "/" + std::to_string(audioChannelCount);

    return val;
}

} // namespace zsdp
