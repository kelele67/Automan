//
//  Exception.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef Exception_hpp
#define Exception_hpp

#include <stdexcept>
#include <string>

#include <errno.h>

#include <zmq.h>

namespace Messaging {
    class Exception : public std::runtime_error {
    public:
        Exception(const std::string &what, const errno_t errnum = errno) :
        std::runtime_error(what + std::string(" - ") + std::string(zmq_strerror(errnum)))
        {}
    };
}
#endif /* Exception_hpp */
