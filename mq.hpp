#pragma once

#include "instance.hpp"

/*
 *
 * mq::setup();
 *
 * Instance instance;
 *
 * instance.on_message_listener = [] (Instance &instance, std::string &channel, char *payload, size_t payload_size) {};
 *
 * instance->connect("address", port);
 *
 * instance->subscribe("category");
 * instance->publish("category", "message");
 *
 */