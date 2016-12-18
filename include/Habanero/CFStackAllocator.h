/* Copyright (c) 2016 Michael G. Kazakov
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
#pragma once

#include <CoreFoundation/CoreFoundation.h>

struct CFStackAllocator
{
    CFStackAllocator() noexcept;

    inline CFAllocatorRef Alloc() const noexcept { return m_Alloc; }
    
private:
    static const int        m_Size = 4096;
    char                    m_Buffer[m_Size];
    // these members should be last to keep cache happy:
    int                     m_Left;
    const CFAllocatorRef    m_Alloc;
    
    CFAllocatorRef __Construct() noexcept;
    static void *__DoAlloc(CFIndex allocSize, CFOptionFlags hint, void *info);
    static void  __DoDealloc(void *ptr, void *info);
};
