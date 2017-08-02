//
//  WithdrawlRequest.hpp
//  Automan
//
//  Created by 刘启 on 2017/8/2.
//  Copyright © 2017年 kelele67. All rights reserved.
//  ATM消息

#ifndef WithdrawlRequest_hpp
#define WithdrawlRequest_hpp

#include "Messaging.hpp"

#include <sstream>

namespace ATM {
    class WithdrawlRequest : public Messaging::Message, public Messaging::ISerialisable {
    public:
        enum class State {
            Hello,
            Account,
            Pin,
            Amount,
            Authorise
        };
        
        WithdrawlRequest():
        _account(""),
        _pin(""),
        _amount(0),
        _state(State::Hello)
        {}
        
        virtual int serialise(Messaging::Socket &socket, const int flags) const {
            auto sent = socket.send("WITHDRAWL", flags | ZMQ_SNDMORE);
            sent += socket.send((char)_state, flags | ZMQ_SNDMORE);
            sent += socket.send(_account, flags | ZMQ_SNDMORE);
            sent += socket.send(_pin, flags | ZMQ_SNDMORE);
            
            std::ostringstream os;
            os << _amount;
            sent += socket.send(os.str(), flags);
            
            return sent;
        }
        
        static std::shared_ptr<WithdrawlRequest> deserialise(Messaging::MessageIterator &iterator) {
            if (memcmp(iterator.data(), "WITHDRAWL", 9) == 0) {
                auto request = std::make_shared<WithdrawlRequest>();
                iterator.more();
                request->_state = (State) * ((char*)iterator.data());
                iterator.more();
                request->_account = std::string((char*)iterator.data(), iterator.size());
                iterator.more();
                request->_pin = std::string((char*)iterator.data(), iterator.size());
                iterator.more();
                std::string amount((char*)iterator.data(), iterator.size());
                
                std::istringstream is(amount);
                is >> request->_amount;
                
                return request;
            }
            return nullptr;
        }
        std::string _account;
        std::string _pin;
        unsigned _amount;
        State _state;
    };
}

#endif /* WithdrawRequest_hpp */
