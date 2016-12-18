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
#include <Habanero/CFStackAllocator.h>

CFStackAllocator::CFStackAllocator() noexcept:
    m_Left(m_Size),
    m_Alloc(__Construct())
{}

CFAllocatorRef CFStackAllocator::__Construct() noexcept
{
    CFAllocatorContext context = {
        0,
        this,
        nullptr,
        nullptr,
        nullptr,
        __DoAlloc,
        nullptr,
        __DoDealloc,
        nullptr
    };
    return CFAllocatorCreate(kCFAllocatorUseContext, &context);
}

void *CFStackAllocator::__DoAlloc(CFIndex allocSize, CFOptionFlags hint, void *info)
{
    CFStackAllocator *me = (CFStackAllocator *)info;
    if( allocSize <= me->m_Left ) {
        void *v = me->m_Buffer + m_Size - me->m_Left;
        me->m_Left -= allocSize;
        return v;
    }
    else
        return malloc(allocSize);
}

void CFStackAllocator::__DoDealloc(void *ptr, void *info)
{
    CFStackAllocator *me = (CFStackAllocator *)info;
    if( ptr < me->m_Buffer || ptr >= me->m_Buffer + m_Size )
        free(ptr);
}
