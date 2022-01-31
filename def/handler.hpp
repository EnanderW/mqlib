#pragma once

#include <uv.h>

namespace mq {
    extern char BUFFER_IN[20000000];

    void on_connection(uv_connect_t* req, int status);

    enum Action {
        SUBSCRIBE = 0,
        UNSUBSCRIBE = 1,
        PUBLISH = 2,
    };
}