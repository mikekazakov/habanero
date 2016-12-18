//
//  DispatchQueue.mm
//  Files
//
//  Created by Michael G. Kazakov on 20.12.13.
//  Copyright (c) 2013 Michael G. Kazakov. All rights reserved.
//

#include <Habanero/DispatchQueue.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// SerialQueueT implementation
////////////////////////////////////////////////////////////////////////////////
/*std::shared_ptr<SerialQueue> SerialQueue::Make(const char *_label)
{
    return std::make_shared<SerialQueue>(_label);
};

std::shared_ptr<SerialQueueT> SerialQueueT::Make(const std::string &_label)
{
    return Make(_label.c_str());
};
*/
//m_Queue

SerialQueue::SerialQueue(const char *_label):
    m_Queue( dispatch_queue_create(_label, DISPATCH_QUEUE_SERIAL) )
{
}

SerialQueue::~SerialQueue()
{
    Stop();
    Wait();
}

void SerialQueue::OnDry( function<void()> _on_dry )
{
    lock_guard<spinlock> lock(m_CallbackLock);
    m_OnDry = _on_dry;
}

void SerialQueue::OnWet( function<void()> _on_wet )
{
    lock_guard<spinlock> lock(m_CallbackLock);
    m_OnWet = _on_wet;
}

void SerialQueue::OnChange( function<void()> _on_change )
{
    lock_guard<spinlock> lock(m_CallbackLock);
    m_OnChange = _on_change;
}

void SerialQueue::Stop()
{
    if(m_Length > 0)
        m_Stopped = true;
}

bool SerialQueue::IsStopped() const
{
    return m_Stopped;
}

void SerialQueue::Increment() const
{
    if( ++m_Length == 1 )
        FireWet();
    FireChanged();
}

void SerialQueue::Decrement() const
{
    if( --m_Length == 0 )
        FireDry();
    FireChanged();
}

/*
void SerialQueueT::Run( function<void()> _block )
{
    Run( [_block = move(_block)](const shared_ptr<SerialQueueT> &_unused) { _block(); } );
}

void SerialQueueT::Run( function<void(const shared_ptr<SerialQueueT> &_que)> _block )
{
    if(m_Stopped) // won't push any the tasks until we're stopped
        return;
    
    if((++m_Length) == 1)
        BecameWet();
    Changed();
    
    __block auto block = move(_block);
    __block auto me = shared_from_this();
    
    m_Queue.async(^{
        
        if(me->m_Stopped == false)
            block(me);
        
        if(--(me->m_Length) == 0)
            BecameDry();
        Changed();
    });
}

void SerialQueueT::RunSync( function<void(const shared_ptr<SerialQueueT> &_que)> _block )
{
    if(m_Stopped) // won't push any the tasks until we're stopped
        return;
    
    __block auto block = move(_block);
    __block auto me = shared_from_this();
    
    m_Queue.sync(^{
        block(me);
    });
}

void SerialQueueT::RunSyncHere( function<void(const shared_ptr<SerialQueueT> &_que)> _block )
{
    if(m_Stopped) // won't push any the tasks until we're stopped
        return;
    _block(shared_from_this());
}
*/
void SerialQueue::Wait()
{
    if( m_Length == 0 )
        return;
    
    //m_Queue.sync(^{});
    //dispatch_group_wait(m_Group, DISPATCH_TIME_FOREVER);
    dispatch_sync_f(m_Queue, nullptr, [](void* _p){});
}

int SerialQueue::Length() const noexcept
{
    return m_Length;
}

bool SerialQueue::Empty() const noexcept
{
    return m_Length == 0;
}

void SerialQueue::FireDry() const
{
    m_Stopped = false;

    lock_guard<spinlock> lock(m_CallbackLock);
    if(m_OnDry)
        m_OnDry();
}

void SerialQueue::FireWet() const
{
    lock_guard<spinlock> lock(m_CallbackLock);
    if(m_OnWet)
        m_OnWet();
}

void SerialQueue::FireChanged() const
{
    lock_guard<spinlock> lock(m_CallbackLock);
    if(m_OnChange)
        m_OnChange();
}
