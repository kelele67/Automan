//
//  ISerialisable.hpp
//  Automan
//
//  Created by 刘启 on 2017/7/28.
//  Copyright © 2017年 kelele67. All rights reserved.
//

#ifndef ISerialisable_hpp
#define ISerialisable_hpp

namespace Messaging {
    class Socket;
    class ISerialisable {
    public:
        virtual int serialise(Socket &socket, const int flags = 0) const = 0;
    };
}

#endif /* ISerialisable_hpp */
