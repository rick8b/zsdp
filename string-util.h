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

#ifndef __ZSDP_STR_UTIL_H__
#define __ZSDP_STR_UTIL_H__

#include <string>
#include <vector>

namespace zsdp {

std::string strToLower(const std::string& s);
std::string strToUpper(const std::string& s);
std::vector<std::string> split(const std::string& haystack,
                               const std::string& needle,
                               size_t maxTokens = 0 /* no limit */);

uint16_t stou16(const std::string& s);
uint32_t stou32(const std::string& s);
int32_t stoi32(const std::string& s);
uint64_t stou64(const std::string& s);
int64_t stoi64(const std::string& s);

std::string printHex(const std::string& title,
                     const void* buf,
                     size_t size);

} // namespace zsdp

#endif /* __ZSDP_STR_UTIL_H__ */
