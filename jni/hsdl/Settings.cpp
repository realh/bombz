/*
 * Copyright (c) 2012, Tony Houghton <h@realh.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// HGame - a simple cross-platform game framework

#include "hsdl/Settings.h"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "hgame/Platform.h"

namespace hsdl {

using namespace std;

Settings::Settings(hgame::Platform *plat,
        const char *owner, const char *domain, const char *appname) :
        mFilename(strdup(plat->getProfileFilename(
                owner, domain, appname, "settings.txt")))
{
    ifstream f(mFilename);
    while (f.good())
    {
        string line;
        getline(f, line);
        if (line.size() && line[0] != '#')
        {
            size_t colon = line.find(':');
            if (colon)
            {
                string k = line.substr(0, colon);
                string v = line.substr(colon + 1);
                mHash[k] = v;
            }
        }
    }
}

Settings::~Settings()
{
    free(mFilename);
}

template<class T>
void Settings::set(const char *k, T v)
{
    ostringstream ss;
    ss << v;
    mHash[string(k)] = ss.str();
}

template<class T>
T Settings::get(const char *k, T d)
{
    map<string, string>::iterator it = mHash.find(string(k));
    if (it == mHash.end())
    {
        return d;
    }
    istringstream ss(it->second);
    T result;
    ss >> result;
    return result;
}

void Settings::set(const char *k, const char *v)
{
    set<const char *>(k, v);
}

const char *Settings::get(const char *k, const char *d)
{
    return get<string>(k, d).c_str();
}

void Settings::set(const char *k, int v)
{
    set<int>(k, v);
}

int Settings::get(const char *k, int d)
{
    return get<int>(k, d);
}

void Settings::set(const char *k, float v)
{
    set<float>(k, v);
}

float Settings::get(const char *k, float d)
{
    return get<float>(k, d);
}

void Settings::enableEditing()
{
}

void Settings::commit()
{
    ofstream f(mFilename);
    if (f.bad())
    {
        THROW(hgame::ErrnoException, "Unable to commit settings");
    }
    map<string, string>::iterator it;
    for (it = mHash.begin(); it != mHash.end(); ++it)
    {
        f << it->first << ':' << it->second << endl;
    }
}

}