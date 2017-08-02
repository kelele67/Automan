//
//  Customer.hpp
//  Automan
//
//  Created by 刘启 on 2017/8/2.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef Customer_hpp
#define Customer_hpp

#include "Messaging.hpp"

#include <memory>
#include <functional>
#include <map>

namespace Messaging {
    class MapMessage : public Message, public ISerialisable {
    public:
        virtual int serialise(Socket &socket, const int flags = 0) const {
            auto iter = _map.begin();
            
            while (iter != _map.end()) {
                socket.send(iter->first, flags | ZMQ_SNDMORE);
                socket.send(iter->second, flags | ZMQ_SNDMORE);
                
                iter++;
            }
            
            socket.send("", flags);
            
            return 0;
        }
    private:
        std::map<std::string, std::string> _map;
    };
}

namespace ATM {
    class WithdrawalRequest : public Messaging::MapMessage {
    public:
        virtual int serialise(Messaging::Socket &socket, const int flags = 0) const {
            auto size = Messaging::MapMessage::serialise(socket, flags);
            
            return size;
        }
    };
    
    class Customer {
    public:
        using State = void(Customer::*)();
        
        Customer(const Messaging::Context &context):
        _atm(context, ZMQ_REQ),
        _cash(0)
        {}
        
        unsigned getCash(const unsigned &amount) {
            _nextState = &Customer::connect;
            
            while (_nextState) {
                (this->*_nextState)();
            }
            
            return _cash;
        }
        
    protected:
        void connect() {
            _atm.bind("inproc://atm");
            _nextState = &Customer::sendRequest;
        }
        
        void sendRequest() {
            _atm.send(ATM::WithdrawalRequest());
        }
        
    private:
        Messaging::Socket _atm;
        State _nextState;
        unsigned _cash;
    };
}
#endif /* Customer_hpp */
