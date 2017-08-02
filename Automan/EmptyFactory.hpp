//
//  EmptyFactory.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef EmptyFactory_hpp
#define EmptyFactory_hpp

#include "MessageIterator.hpp"

#include <memory>

namespace Messaging {
    class EmptyFactory {
    public:
        using value_type = void;
        using pointer_type = std::shared_ptr<void>;
        
        static pointer_type create(MessageIterator &) {
            return pointer_type(nullptr);
        }
    };
}

#endif /* EmptyFactory_hpp */
