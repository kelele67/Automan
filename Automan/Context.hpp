//
//  Context.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef Context_hpp
#define Context_hpp

#include <zmq.h>
#include <memory>

namespace Messaging {
    class Context {
    public:
        Context() : _context(std::shared_ptr<void>(zmq_ctx_new(), &zmq_ctx_destroy)) {}
        
        void *get() const {
            return _context.get();
        }
    private:
        std::shared_ptr<void> _context;
    };
}


#endif /* Context_hpp */
