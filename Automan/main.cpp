//
//  main.cpp
//  Automan
//
//  Created by 刘启 on 2017/8/2.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#include "ATM.hpp"
#include "Customer.hpp"

#include <thread>
#include <vector>
#include <stdio.h>

int main() {
    std::cout << "Staring" << std::endl;
    
    Messaging::Context context;
    
    std::vector<std::thread> threads;
    
    threads.emplace_back(ATM::Machine(context));
    
    ATM::Customer customer(context);
    
    auto cash = customer.getCash(25);
    
    std::cout << "Cash obtained: " << cash << std::endl;
    
    Messaging::Socket atm(context, ZMQ_REQ);
    
    atm.bind("inproc://atm");
    
    atm.send(ATM::WithdrawlRequest());
    
    atm.receive<ATM::MessagFactory>()
    .handle<ATM::WithdrawlRequest>(
                                   [&] (const ATM::WithdrawlRequest &request) {
                                       auto response = request;
                                       response._account = "0123456789";
                                       atm.send(response);
                                       
                                       return true;
                                   });
    
    atm.receive<ATM::MessagFactory>()
    .handle<ATM::WithdrawlRequest>(
                                   [&](const ATM::WithdrawlRequest &request) {
                                       auto response = request;
                                       response._pin = "1234";
                                       atm.send(response);
                                       
                                       return true;
                                   });
    
    atm.receive<ATM::MessagFactory>()
    .handle<ATM::WithdrawlRequest>(
                                   [&](const ATM::WithdrawlRequest &request) {
                                       auto response = request;
                                       response._amount = 25;
                                       atm.send(response);
                                       return true;
                                   });
    
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    
    return 0;
}
