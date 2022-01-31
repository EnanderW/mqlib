#pragma once

#include <uv.h>
#include <string>
#include <vector>
#include <mutex>

namespace mq {
    class Instance;

    typedef void (*connection_listener)(Instance &instance, bool success);
    typedef void (*message_listener)(Instance &instance, std::string &channel, char *message, size_t length);

    class Instance {
    private:
        uv_tcp_t socket{};
        uv_connect_t connection{};
        uv_async_t async{};
        sockaddr_in addr{};

        std::mutex buffers_lock{};
        std::vector<uv_buf_t> buffers{};
    public:
        connection_listener on_connection_listener{};
        message_listener on_message_listener{};

    public:
        Instance();

        void connect(const std::string& address, int32_t port, connection_listener connection_listener = nullptr, message_listener message_listener = nullptr);
        void subscribe(const std::string& channel);
        void unsubscribe(const std::string& channel);
        void publish(const std::string& channel, char *payload, size_t payload_size);
    private:
        static void work_notification(uv_async_t *handle);
        static void do_connection(uv_async_t *handle);

        void push_buffer(char *data, size_t size);
        void notify();
    };

    void setup();
}