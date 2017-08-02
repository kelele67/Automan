//
//  ATM.hpp
//  Automan
//
//  Created by 刘启 on 2017/8/2.
//  Copyright © 2017年 kelele67. All rights reserved.
//  ATM状态机

#ifndef ATM_hpp
#define ATM_hpp

#include "Messaging.hpp"
#include "WithdrawlRequest.hpp"

#include <sstream>
#include <iostream>
#include <thread>

namespace ATM {
    class MessagFactory {
    public:
        using value_type = Messaging::Message;
        using pointer_type = std::shared_ptr<value_type>;
        
        static pointer_type create(Messaging::MessageIterator &iterator) {
            pointer_type msg;
            
            if ((msg = WithdrawlRequest::deserialise(iterator))) {
                return msg;
            }
            return nullptr;
        }
    };
    
    class Machine {
    public:
        using state = void(Machine::*)();
        
        Machine(const Messaging::Context &context):
        _customer(context, ZMQ_REP)
        {}
        
        void operator()() {
            _customer.connect("inproc://atm");
            
            state state = &Machine::wait_message;
            
            while (state) {
                (this->*state)();
            }
        }
        
        void wait_message() {
            _customer.receive<MessagFactory>()
            .handle<WithdrawlRequest>(
                                      [&](const WithdrawlRequest &request) {
                                          auto response = request;
                                          switch (request._state) {
                                              case WithdrawlRequest::State::Hello:
                                                  std::cout << std::this_thread::get_id() << ": Please Insert Your Card" << std::endl;
                                                  response._state = WithdrawlRequest::State::Account;
                                                  break;
                                                  
                                              case WithdrawlRequest::State::Account:
                                                  if (request._account.size() == 10) {
                                                      std::cout << std::this_thread::get_id() << ": Please Input Your PIN" << std::endl;
                                                      response._state = WithdrawlRequest::State::Pin;
                                                  }
                                                  
                                              case WithdrawlRequest::State::Pin:
                                                  if (request._pin.size() == 4) {
                                                      std::cout << std::this_thread::get_id() << ": Please Input the Amount to Withdraw" << std::endl;
                                                      response._state = WithdrawlRequest::State::Amount;
                                                  }
                                              case WithdrawlRequest::State::Amount:
                                                  if (request._amount > 0) {
                                                      std::cout << std::this_thread::get_id() << ": Please Wait, Authorising..." << std::endl;
                                                      response._state = WithdrawlRequest::State::Authorise;
                                                  }
                                              default:
                                                  break;
                                          }
                                          
                                          _customer.send(response);
                                          
                                          return true;
                                      })
            .handle<Messaging::Message>(
                                        [&](const Messaging::Message &msg) {
                                            std::cout << std::this_thread::get_id() << ": Invalid!" << std::endl;
                                            _customer.send("FAIL");
                                            
                                            return true;
                                        })
            .handle(
                    [&]() {
                        _customer.send("");
                        
                        return true;
                    });
        }
        
    private:
        Messaging::Socket _customer;
    };
}
#endif /* ATM_hpp */
