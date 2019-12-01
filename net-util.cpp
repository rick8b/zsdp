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

#include "net-util.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

using namespace std;

namespace zsdp {

string sockaddrToString(sockaddr_storage addr){
    if(addr.ss_family == AF_INET){
        char addrCStr[INET_ADDRSTRLEN + 1];
        sockaddr_in* sa = (sockaddr_in*)&addr;
        const char* outStr = inet_ntop(AF_INET,
                                       &sa->sin_addr,
                                       addrCStr,
                                       sizeof(addrCStr));

        if(outStr == NULL)
            return "";

        return addrCStr;
    }
    else if(addr.ss_family == AF_INET6){
        char addrCStr[INET6_ADDRSTRLEN + 1];
        sockaddr_in6* sa = (sockaddr_in6*)&addr;
        const char* outStr = inet_ntop(AF_INET6,
                                       &sa->sin6_addr,
                                       addrCStr,
                                       sizeof(addrCStr));

        if(outStr == NULL)
            return "";

        return addrCStr;
    }
    else{
        return "";
    }
}

sockaddr_storage sockaddrParse(const string& addrStr){
    sockaddr_storage saddr;
    sockaddr_in* v4 = (sockaddr_in*)&saddr;
    sockaddr_in6* v6 = (sockaddr_in6*)&saddr;

    memset(&saddr, 0, sizeof(saddr));
    int status = inet_pton(AF_INET, addrStr.c_str(), &v4->sin_addr);
    if(status == 1){
        v4->sin_family = AF_INET;
        v4->sin_len = sizeof(*v4);
        return saddr;
    }

    memset(&saddr, 0, sizeof(saddr));
    status = inet_pton(AF_INET6, addrStr.c_str(), &v6->sin6_addr);
    if(status == 1){
        v6->sin6_family = AF_INET6;
        v6->sin6_len = sizeof(*v6);
        return saddr;
    }

    if(status < 0)
        throw runtime_error("System error " + to_string(errno) + " while parsing address '" + addrStr + "': " + strerror(errno));

    throw invalid_argument("Failed to parse address: " + addrStr);
}

} // namespace zsdp
