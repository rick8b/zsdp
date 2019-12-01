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

#include "string-util.h"
#include <sstream>

using namespace std;

namespace zsdp {

string strToLower(const string& s){
    string lc;
    for(size_t i = 0; i < s.size(); i++)
        lc += tolower(s[i]);

    return lc;
}

string strToUpper(const string& s){
    string uc;
    for(size_t i = 0; i < s.size(); i++)
        uc += toupper(s[i]);

    return uc;
}

vector<string> split(const string& haystack,
                     const string& needle,
                     size_t maxTokens)
{
    string s = haystack;
    vector<string> strings;

    if(needle.empty()){
        strings.resize(needle.size());
        for(size_t i = 0; i < needle.size(); i++)
            strings[i] = needle.substr(i, 1);

        return strings;
    }

    while(true){
        if(strings.size() + 1 == maxTokens){
            strings.push_back(s);
            break;
        }

        size_t nlLoc = s.find(needle);
        if(nlLoc == string::npos){
            strings.push_back(s);
            break;
        }
        else {
            strings.push_back(s.substr(0, nlLoc));
            s = s.substr(nlLoc + needle.size());
        }
    }

    return strings;
}

uint16_t stou16(const std::string& s){
    unsigned long ul = stoul(s);
    if(ul > UINT16_MAX)
        throw out_of_range(s + " is out-of-range for an unsigned 16-bit integer.");

    return (uint16_t)ul;
}

uint32_t stou32(const std::string& s) {
    unsigned long ul = stoul(s);
    if(ul > UINT32_MAX)
        throw out_of_range(s + " is out-of-range for an unsigned 32-bit integer.");

    return (uint32_t)ul;
}

int32_t stoi32(const std::string& s) {
    return stoi(s);
}

uint64_t stou64(const std::string& s) {
    return stoull(s);
}

int64_t stoi64(const std::string& s) {
    return stoll(s);
}

string printHex(const string& title,
                const void* buf,
                size_t size)
{

    ostringstream out;

    char byteChars[3];
    const uint8_t* buf8 = (const uint8_t*)buf;

    out << title << "\n";

    for(size_t i = 0; i < size; i++){
        snprintf(byteChars, 3, "%02hhx", buf8[i]);

        out << byteChars;

        if((i + 1) % 16 == 0)
            out << "\n";
        else
            out << " ";

    }

    return out.str();
}

} // namespace zsdp
