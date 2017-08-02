//
//  Dispatcher.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef Dispatcher_hpp
#define Dispatcher_hpp

#include <memory>
#include <functional>

namespace Messaging {
    template <class T>
    class Dispatcher {
    
    public:
        Dispatcher(const std::shared_ptr<T> msg) : _msg(msg), _handled(false) {}
        
        template <class M>
        Dispatcher &handle(const std::function<bool(M&)> &f) {
            if (!_handled) {
                auto specific = std::dynamic_pointer_cast<M>(_msg);
                if (specific) {
                    _handled = f(*specific);
                }
            }
            return *this;
        }
        
        Dispatcher &handle(const std::function<bool()> &f) {
            if (!_handled) {
                _handled = f();
            }
            return *this;
        }
    private:
        std::shared_ptr<T> _msg;
        bool _handled;
    };
}

#endif /* Dispatcher_hpp */
