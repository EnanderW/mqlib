#include "instance.hpp"

#include "handler.hpp"
#include "util/buffer.hpp"

#include <thread>
#include <iostream>

namespace mq {

    static uv_loop_t event_loop{};
    static uv_idle_t idle_handle{};
    static std::thread *thread{};

    static void wait_for_instance(uv_idle_t *handle) {
        if (event_loop.active_handles > 1)
            uv_idle_stop(&idle_handle);
    }

    void setup() {
        uv_loop_init(&event_loop);

        uv_idle_init(&event_loop, &idle_handle);

        thread = new std::thread([] () {
            uv_idle_start(&idle_handle, wait_for_instance);
            uv_run(&event_loop, UV_RUN_DEFAULT);

            delete thread;
        });

        sleep(1);
    }

    Instance::Instance() {
        uv_async_init(&event_loop, &this->async, work_notification);
        this->async.data = this;
    }

    void Instance::connect(const std::string& address, int32_t port, connection_listener connection_listener, message_listener message_listener) {
        this->on_connection_listener = connection_listener == nullptr ? this->on_connection_listener : connection_listener;
        this->on_message_listener = message_listener == nullptr ? this->on_message_listener : message_listener;

        uv_ip4_addr(address.c_str(), port, &this->addr);

        auto *connect_async = new uv_async_t;
        uv_async_init(&event_loop, connect_async, do_connection);
        connect_async->data = this;

        uv_async_send(connect_async);
    }

    void Instance::subscribe(const std::string& channel) {
        size_t size = 3 + channel.size();
        char *data = new char[size];
        Buffer buf(data);

        buf.ubyte(SUBSCRIBE);
        buf.utfstring(channel.c_str(), channel.size());

        this->push_buffer(data, size);
        this->notify();
    }

    void Instance::unsubscribe(const std::string& channel) {
        size_t size = 3 + channel.size();
        char *data = new char[size];
        Buffer buf(data);

        buf.ubyte(UNSUBSCRIBE);
        buf.utfstring(channel.c_str(), channel.size());

        this->push_buffer(data, size);
        this->notify();
    }

    void Instance::publish(const std::string& channel, char *payload, size_t payload_size) {
        size_t size = 5 + channel.size() + payload_size;
        char *data = new char[size];
        Buffer buf(data);

        buf.ubyte(PUBLISH);
        buf.utfstring(channel.c_str(), channel.size());
        buf.utfstring(payload, payload_size);

        this->push_buffer(data, size);
        this->notify();
    }

    void Instance::push_buffer(char *data, size_t size) {
        buffers_lock.lock();
        buffers.push_back({ data, size });
        buffers_lock.unlock();
    }

    void Instance::notify() {
        uv_async_send(&this->async);
    }

    void Instance::work_notification(uv_async_t *handle) {
        auto *instance = static_cast<Instance*>(handle->data);

        instance->buffers_lock.lock();

        uv_try_write(instance->connection.handle, instance->buffers.data(), instance->buffers.size());
        for (auto &buf : instance->buffers)
            delete [] buf.base;

        instance->buffers.clear();

        instance->buffers_lock.unlock();
    }

    void Instance::do_connection(uv_async_t *handle) {
        auto *instance = static_cast<Instance*>(handle->data);

        uv_tcp_init(&event_loop, &instance->socket);
        uv_tcp_keepalive(&instance->socket, 1, 60);

        instance->connection.data = instance;

        uv_tcp_connect(&instance->connection, &instance->socket, (const struct sockaddr*) &instance->addr, on_connection);

        delete handle;
    }
}