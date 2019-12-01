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
 s*/

#ifndef __ZSDP_SDP_ATTRIBS_PRIV_H__
#define __ZSDP_SDP_ATTRIBS_PRIV_H__

#include <zsdp/defs.h>
#include <zsdp/attributes.h>


namespace zsdp {

sp<Attribute> parseMediaDirection(const std::string& key, const std::string& val);
sp<Attribute> parseOrientation(const std::string& key, const std::string& val);
sp<Attribute> parseConferenceType(const std::string& key, const std::string& val);
sp<Attribute> parseCharset(const std::string& key, const std::string& val);
sp<Attribute> parseSdpLanguage(const std::string& key, const std::string& val);
sp<Attribute> parseLanguage(const std::string& key, const std::string& val);
sp<Attribute> parseFramerate(const std::string& key, const std::string& val);
sp<Attribute> parseQuality(const std::string& key, const std::string& val);
sp<Attribute> parseFormatParams(const std::string& key, const std::string& val);
sp<Attribute> parseRtpMap(const std::string& key, const std::string& val);
sp<Attribute> parseCategory(const std::string& key, const std::string& val);
sp<Attribute> parseKeywords(const std::string& key, const std::string& val);
sp<Attribute> parseTool(const std::string& key, const std::string& val);
sp<Attribute> parsePTime(const std::string& key, const std::string& val);
sp<Attribute> parseMaxPTime(const std::string& key, const std::string& val);

} // namespace zsdp

#endif /* __ZSDP_SDP_ATTRIBS_PRIV_H__ */
