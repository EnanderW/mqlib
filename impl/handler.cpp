#include "handler.hpp"

#include "instance.hpp"
#include "util/buffer.hpp"

#include <iostream>

namespace mq {
    void allocate_buffers(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* uv_buf);
    void on_close(uv_handle_t* handle) {}

    char BUFFER_IN[20000000];

    void on_connection(uv_connect_t* req, int status) {
        auto *instance = static_cast<Instance*>(req->data);
        if (status < 0) {
            instance->on_connection_listener(*instance, false);
            return;
        }

        instance->on_connection_listener(*instance, true);

        auto *stream = req->handle;
        stream->data = instance;

        uv_read_start(stream, allocate_buffers, on_read);
    }

    void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* uv_buf) {
        auto *instance = static_cast<Instance*>(stream->data);
        if (nread < 0) {
            if (nread == UV_EOF) {
                std::cout << "End of file yo 2\n";
            }

            std::cout << "Error on_read: " << uv_strerror(nread) << "\n";

            uv_close(reinterpret_cast<uv_handle_t *>(stream), on_close);
            return;
        }

        Buffer buf(uv_buf->base);
        char *end = buf.data + nread;

        while (buf.data < end) {
            uint16_t channel_length = buf.ushort();
            std::string channel(buf.data, channel_length);
            buf.data += channel_length;

            uint16_t message_length = buf.ushort();
            char *message = buf.data;
            instance->on_message_listener(*instance, channel, message, message_length);
            buf.data += message_length;
        }
    }

    void allocate_buffers(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
        buf->base = BUFFER_IN;
        buf->len = 20000000;
    }
}