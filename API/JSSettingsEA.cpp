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

#include "config.h"
#include "JSSettingsEA.h"
#include "Profiler.h"

struct JSSettingsEAPrivate
{
    size_t    mJavaScriptStackSize;	    // In bytes.  This is the approximate stack size in bytes and not the capacity of the stack array.
    size_t	  mJavaScriptHeapWatermark;	
	bool      mPrintExceptions;
    const char8_t*  mdefaultLocale;

	JSCallstackCallback mCallstackCallback;
    JSLogCallback mLogCallback;

    JSSettingsEAPrivate(void)
    : mJavaScriptStackSize(128 * 1024)
	, mJavaScriptHeapWatermark(1 * 1024 * 1024)
    , mPrintExceptions(false)
	, mdefaultLocale(NULL)
    , mCallstackCallback(NULL)
    , mLogCallback(NULL) 
	{
        // Do nothing.
    }
};

static JSSettingsEAPrivate sSettingsJS;


void JSSetStackSize(size_t  size)
{
    sSettingsJS.mJavaScriptStackSize = size;
}

size_t JSGetStackSize(void) 
{
    return sSettingsJS.mJavaScriptStackSize;
}

void JSSetHeapWatermark(size_t  size)
{
	sSettingsJS.mJavaScriptHeapWatermark = size;
}

size_t JSGetHeapWatermark(void) 
{
	return sSettingsJS.mJavaScriptHeapWatermark;
}

void JSSetPrintExceptions(bool active)
{
    sSettingsJS.mPrintExceptions = active;            
}

bool JSPrintExceptionsEnabled(void)
{
    return sSettingsJS.mPrintExceptions;            
}

void JSSetDefaultLocale(const char8_t* pLocale)
{
	sSettingsJS.mdefaultLocale = pLocale;
}

const char8_t* JSGetDefaultLocale()
{
	return sSettingsJS.mdefaultLocale;
}

void JSSetCallstackCallback(JSCallstackCallback callback)
{
    sSettingsJS.mCallstackCallback = callback;
}

JSCallstackCallback JSGetCallstackCallback(void)
{
    return sSettingsJS.mCallstackCallback;
}

// Not exactly a setting so might need to move this to a different location but is EA platform specific.  
void JSFinalize(void)
{
}

void JSSetLogCallback(JSLogCallback callback)
{
    sSettingsJS.mLogCallback = callback;
}

JSLogCallback JSGetLogCallback(void)
{
    return sSettingsJS.mLogCallback;
}
