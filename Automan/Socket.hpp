//
//  Socket.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef Socket_hpp
#define Socket_hpp

#include "Context.hpp"
#include "Exception.hpp"
#include "ISerialisable.hpp"
#include "MessageIterator.hpp"
#include "Dispatcher.hpp"
#include "EmptyFactory.hpp"

#include <string>
#include <functional>

namespace Messaging {
    class Socket {
    public:
        Socket(const Context &context, const int type) :
        _socket(std::shared_ptr<void>(zmq_socket(context.get(), type), &zmq_close))
        {}
        
        void bind(const std::string &addr) const {
            auto rc = zmq_bind(_socket.get(), addr.c_str());
            
            if (rc != 0) {
                throw Exception("Binding to '" + addr + "'");
            }
        }
        
        void connect(const std::string &addr) const {
            auto rc = zmq_connect(_socket.get(), addr.c_str());
            
            if (rc != 0) {
                throw Exception("Connecting to '" + addr + "'");
            }
        }
        
        int send(const char ch, const int flags = 0) const {
            auto len = zmq_send(_socket.get(), &ch, sizeof(char), flags);
            
            if (len < 0) {
                throw Exception("Sending byte");
            }
            return len;
        }
        
        int send(const std::string &data, const int flags = 0) const {
            auto len = zmq_send(_socket.get(), data.c_str(), data.size(), flags);
            
            if (len < 0) {
                throw Exception("Sending string");
            }
            return len;
        }
        
        int send(const ISerialisable &message) {
            auto len = message.serialise(*this);
            
            if (len < 0) {
                throw Exception("Sending serialisable");
            }
            return len;
        }
        
        template <class Factory = EmptyFactory>
        Dispatcher<typename Factory::value_type> receive(const int flags = 0) const {
            MessageIterator iterator(_socket, flags);
                if (iterator) {
                    return Factory::create(iterator);
                }
                return Dispatcher<typename Factory::value_type>(nullptr);
        }
    private:
        std::shared_ptr<void> _socket;
    };
}
#endif /* Socket_hpp */
