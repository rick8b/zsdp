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

#ifndef __ZSDP_ATTRIBUTES_H__
#define __ZSDP_ATTRIBUTES_H__

#include <string>
#include <zsdp/defs.h>


namespace zsdp{

class Attribute {
public:
    virtual ~Attribute(){}

    virtual std::string key() const = 0;
    virtual std::string value() const = 0;

    /// everything except a=
    virtual std::string sdpLine() const{ return key() + ":" + value(); }
};



typedef sp<Attribute> (*AttributeParseFnc)(const std::string& key,
                                           const std::string& value);

void registerAttribute(const std::string& key,
                       AttributeParseFnc parseFnc);

sp<Attribute> parseAttribute(const std::string& line);





#define SINGLE_ATTR_CLASS_DEF(CLASS_NAME, PARAM_TYPE, PARAM_NAME)  \
class CLASS_NAME : public Attribute {                    \
public:                                                  \
    virtual ~CLASS_NAME();                               \
    PARAM_TYPE PARAM_NAME;                               \
                                                         \
    virtual std::string key() const override;            \
    virtual std::string value() const override;          \
};


SINGLE_ATTR_CLASS_DEF(AttrCategory, std::string, category)
SINGLE_ATTR_CLASS_DEF(AttrKeywords, std::string, keywords)
SINGLE_ATTR_CLASS_DEF(AttrTool,     std::string, tool)

SINGLE_ATTR_CLASS_DEF(AttrPTime, uint64_t, packetDuration)
SINGLE_ATTR_CLASS_DEF(AttrMaxPTime, uint64_t, maxPacketDuration)

enum class MediaDirection {
    SendRecv,
    SendOnly,
    RecvOnly,
    Inactive,
};

class AttrMediaDirection : public Attribute {
public:
    virtual ~AttrMediaDirection();

    MediaDirection direction;

    virtual std::string key() const override;
    virtual std::string value() const override;
    virtual std::string sdpLine() const override;
};


enum class Orientation {
    Unknown,
    Landscape,
    Portrait,
    Seascape,
};

SINGLE_ATTR_CLASS_DEF(AttrOrientation, Orientation, orientation)


constexpr const char *const kConferenceType_Broadcast = "broadcast";
constexpr const char *const kConferenceType_Meeting = "meeting";
constexpr const char *const kConferenceType_Moderated = "moderated";
constexpr const char *const kConferenceType_Test = "test";
constexpr const char *const kConferenceType_H332 = "H332";

SINGLE_ATTR_CLASS_DEF(AttrConferenceType, std::string, conferenceType)
SINGLE_ATTR_CLASS_DEF(AttrCharset, std::string, charset)
SINGLE_ATTR_CLASS_DEF(AttrSdpLanguage, std::string, language)
SINGLE_ATTR_CLASS_DEF(AttrMediaLanguage, std::string, language)
SINGLE_ATTR_CLASS_DEF(AttrFramerate, double, framerate)
SINGLE_ATTR_CLASS_DEF(AttrQuality, uint32_t, quality)




class AttrFormatParams : public Attribute {
public:
    virtual ~AttrFormatParams();

    uint8_t payloadType = kPayloadType_NotSet;
    std::string formatParams;

    virtual std::string key() const override;
    virtual std::string value() const override;
};


class AttrRtpMap : public Attribute {
public:
    virtual ~AttrRtpMap();

    uint8_t payloadType = kPayloadType_NotSet;
    std::string encodingName;
    uint32_t clockRate = 0;
    uint32_t audioChannelCount = 0;

    virtual std::string key() const override;
    virtual std::string value() const override;
};

} // namespace zsdp

#endif // __ZSDP_ATTRIBUTES_H__
