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

#include "attributes-priv.h"
#include "parsing.h"
#include "string-util.h"

using namespace std;


namespace zsdp {


sp<Attribute> parseCategory(const string& key, const string& val){
    auto attr = make_shared<AttrCategory>();
    attr->category = val;

    return attr;
}

sp<Attribute> parseKeywords(const string& key, const string& val){
    auto attr = make_shared<AttrKeywords>();
    attr->keywords = val;

    return attr;
}

sp<Attribute> parseTool(const string& key, const string& val){
    auto attr = make_shared<AttrTool>();
    attr->tool = val;

    return attr;
}

sp<Attribute> parseOrientation(const string& key, const string& val){
    auto attr = make_shared<AttrOrientation>();
    attr->orientation = orientationForStr(val);

    return attr;
}

sp<Attribute> parsePTime(const string& key, const string& val){
    auto attr = make_shared<AttrPTime>();
    try {
        attr->packetDuration = stoull(val);
    }
    catch(...) {
        fprintf(stderr, "Error parsing PTime '%s'.\n", val.c_str());
        return NULL;
    }

    return attr;
}

sp<Attribute> parseMaxPTime(const string& key, const string& val){
    auto attr = make_shared<AttrMaxPTime>();

    
    attr->maxPacketDuration = stoull(val);

    return attr;
}

sp<Attribute> parseMediaDirection(const string& key, const string& val){
    auto attr = make_shared<AttrMediaDirection>();
    attr->direction = mediaDirectionForStr(key);

    return attr;
}

sp<Attribute> parseCharset(const string& key, const string& val){
    auto attr = make_shared<AttrCharset>();
    attr->charset = val;

    return attr;
}

sp<Attribute> parseSdpLanguage(const string& key, const string& val){
    auto attr = make_shared<AttrSdpLanguage>();
    attr->language = val;

    return attr;
}

sp<Attribute> parseLanguage(const string& key, const string& val){
    auto attr = make_shared<AttrMediaLanguage>();
    attr->language = val;

    return attr;
}

sp<Attribute> parseFramerate(const string& key, const string& val){
    auto attr = make_shared<AttrFramerate>();

    attr->framerate = stod(val);

    return attr;
}

sp<Attribute> parseQuality(const string& key, const string& val){
    auto attr = make_shared<AttrQuality>();

    double quality = stod(val);
    attr->quality = (uint32_t)(quality + 0.5);

    return attr;
}

sp<Attribute> parseConferenceType(const std::string& key,
                                  const std::string& val)
{
    auto attr = make_shared<AttrConferenceType>();
    attr->conferenceType = val;

    return attr;
}

sp<Attribute> parseFormatParams(const string& key, const string& val){
    auto attr = make_shared<AttrFormatParams>();

    auto tokens = split(val, " ", 2);

    string payloadTypeStr = tokens[0];
    string params = tokens.size() > 1 ? tokens[1] : "";

    try{
        attr->payloadType = parsePayloadType(payloadTypeStr);
    } catch(exception e){
        throw runtime_error("Failed to parse payload type '" + payloadTypeStr + "'.");
    }
    attr->formatParams = params;

    return attr;
}

sp<Attribute> parseRtpMap(const string& key, const string& val){
    auto attr = make_shared<AttrRtpMap>();
    auto tokens = split(val, " ", 2);

    string payloadTypeStr = tokens[0];
    string encParams = tokens.size() > 1 ? tokens[1] : "";

    auto encTokens = split(encParams, "/", 3);
    if(encTokens.size() < 2)
        throw invalid_argument("Unable to parse rtpmap '" + val + "'.");

    attr->payloadType = parsePayloadType(payloadTypeStr);
    attr->encodingName = encTokens[0];
    attr->clockRate = stou32(encTokens[1]);
    attr->audioChannelCount = encTokens.size() > 2 ? stou32(encTokens[2]) : 1;

    return attr;
}

} // namespace zsdp
