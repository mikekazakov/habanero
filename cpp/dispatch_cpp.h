// dispatch_cpp.h
//
// Copyright (c) 2014 Michael Kazakov <mike.kazakov@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include <dispatch/dispatch.h>
#include <functional>
#include <utility>

// synopsis

template <class T>
void dispatch_async( dispatch_queue_t queue, T f );

template <class T>
void dispatch_sync( dispatch_queue_t queue, T f );

template <class T>
void dispatch_apply( size_t iterations, dispatch_queue_t queue, T f );

template <class T>
void dispatch_after( std::chrono::nanoseconds when, dispatch_queue_t queue, T f );

template <class T>
void dispatch_barrier_async( dispatch_queue_t queue, T f );

template <class T>
void dispatch_barrier_sync( dispatch_queue_t queue, T f );



// implementation details

namespace __dispatch_cpp {
    typedef std::function<void()>       __lambda_exec;
    typedef std::function<void(size_t)> __lambda_apply;
    void __dispatch_cpp_exec_delete_lambda(void*);
    void __dispatch_cpp_appl_lambda(void*, size_t);
}

template <class T>
inline void dispatch_async( dispatch_queue_t queue, T f )
{
    dispatch_async_f(queue,
                     new __dispatch_cpp::__lambda_exec( std::move(f) ),
                     __dispatch_cpp::__dispatch_cpp_exec_delete_lambda);
}

template <class T>
inline void dispatch_sync( dispatch_queue_t queue, T f )
{
    dispatch_sync_f(queue,
                    new __dispatch_cpp::__lambda_exec( std::move(f) ),
                    __dispatch_cpp::__dispatch_cpp_exec_delete_lambda);
}

template <class T>
inline void dispatch_apply( size_t iterations, dispatch_queue_t queue, T f )
{
    __dispatch_cpp::__lambda_apply l( std::move(f) );
    dispatch_apply_f(iterations,
                     queue,
                     &l,
                     __dispatch_cpp::__dispatch_cpp_appl_lambda);
}

template <class T>
inline void dispatch_after( std::chrono::nanoseconds when, dispatch_queue_t queue, T f )
{
    dispatch_after_f(dispatch_time(DISPATCH_TIME_NOW, when.count()),
                     queue,
                     new __dispatch_cpp::__lambda_exec( std::move(f) ),
                     __dispatch_cpp::__dispatch_cpp_exec_delete_lambda);
}

template <class T>
inline void dispatch_barrier_async( dispatch_queue_t queue, T f )
{
    dispatch_barrier_async_f(queue,
                             new __dispatch_cpp::__lambda_exec( std::move(f) ),
                             __dispatch_cpp::__dispatch_cpp_exec_delete_lambda);
}

template <class T>
inline void dispatch_barrier_sync( dispatch_queue_t queue, T f )
{
    dispatch_barrier_sync_f(queue,
                            new __dispatch_cpp::__lambda_exec( std::move(f) ),
                            __dispatch_cpp::__dispatch_cpp_exec_delete_lambda);
}
