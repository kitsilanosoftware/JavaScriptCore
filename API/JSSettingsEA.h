/*
Copyright (C) 2011, 2012 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef JSSettingsEA_h
#define JSSettingsEA_h

#include "JSBase.h"
#include <eastl/string.h>
#include <eastl/vector.h>

/* Javascript configurable settings */

// For setting the stack size used by the RegisterFile
void JSSetStackSize(size_t  size);
size_t JSGetStackSize(void); 

void JSSetHeapWatermark(size_t  size);
size_t JSGetHeapWatermark(void);

// For exception printing
void JSSetPrintExceptions(bool active);
bool JSPrintExceptionsEnabled(void);

// For setting default locale
void JSSetDefaultLocale(const char8_t* pLocale);
const char8_t* JSGetDefaultLocale();

// For callstack capturing.
typedef void (*JSCallstackCallback)(const eastl::vector<eastl::string8> &names, const eastl::vector<eastl::string8> &args, const eastl::vector<int> &lines, const eastl::vector<eastl::string8> &urls);
void JSSetCallstackCallback(JSCallstackCallback callback);
JSCallstackCallback JSGetCallstackCallback(void);

// For leak fixes on general shutdown.
void JSFinalize(void);

// Assertion/Logging
typedef void (*JSLogCallback)(const eastl::string8 &message, bool shouldAssert);
void JSSetLogCallback(JSLogCallback callback);
JSLogCallback JSGetLogCallback(void);

#endif /* JSSettingsEA_h */
