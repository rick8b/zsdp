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

#ifndef __ZSDP_NET_UTIL_H__
#define __ZSDP_NET_UTIL_H__

#include <string>
#include <sys/socket.h>

namespace zsdp{

std::string sockaddrToString(sockaddr_storage addr);

sockaddr_storage sockaddrParse(const std::string& addrStr);

} // namespace zsdp

#endif /* __ZSDP_NET_UTIL_H__ */
