//
//  MessageIterator.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef MessageIterator_hpp
#define MessageIterator_hpp

#include "Exception.hpp"

#include <zmq.h>
#include <memory>

namespace Messaging {
    class MessageIterator {
    public:
        MessageIterator(const std::shared_ptr<void> &socket, const int flags = 0)
        :
        _socket(socket),
        _flags(flags),
        _valid(false),
        _close_msg(false) {
            _valid = receive();
        }
        
        MessageIterator(const MessageIterator&) = delete;
        ~MessageIterator() {
            if (_valid) {
                while (more()) {
                    // dump 剩余消息
                }
            }
            if (_close_msg) {
                zmq_msg_close(&_msg);
            }
        }
        
        const void *data() {
            if (_valid) {
                return zmq_msg_data(&_msg);
            }
            return nullptr;
        }
        
        const size_t size() {
            if (_valid) {
                return zmq_msg_size(&_msg);
            }
            return 0;
        }
        
        operator bool() const {
            return _valid;
        }
        
        bool more() {
            _valid = receive();
            return _valid;
        }
        
    protected:
        bool receive() {
            if(_close_msg) {
                auto more = zmq_msg_more(&_msg);
                zmq_msg_close(&_msg);
                _close_msg = false;
                if (!more) {
                    return more;
                }
            }
            
            zmq_msg_init(&_msg);
            _close_msg = true;
            
            auto len = zmq_msg_recv(&_msg, _socket.get(), _flags);
            
            if (len < 0) {
                if (len == EAGAIN) {
                    return false;
                }
                throw Exception("Receiving message part");
            }
            return true;
        }
        
    private:
        std::shared_ptr<void> _socket;
        zmq_msg_t _msg;
        int _flags;
        bool _valid;
        bool _close_msg;
    };
}

#endif /* MessageIterator_hpp */
