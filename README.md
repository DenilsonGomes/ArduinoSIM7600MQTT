# Arduino for SIM7600 MQTT

This library provides simple AT commands for publish messaging with
a SIM7600 using MQTT.

## Examples

The library is a simple implementation of AT commands for SIM7500 and SIM7600 by Arduino SoftwareSerial.

## Limitations

 - It can only publish QoS 0 messages. It can subscribe at QoS 0 or QoS 1.
 - The maximum message size, including header, is **256 bytes** by default. This
   is configurable via `MQTT_MAX_PACKET_SIZE` in `PubSubClient.h` or can be changed
   by calling `PubSubClient::setBufferSize(size)`.
 - The keepalive interval is set to 15 seconds by default. This is configurable
   via `MQTT_KEEPALIVE` in `PubSubClient.h` or can be changed by calling
   `PubSubClient::setKeepAlive(keepAlive)`.
 - The client uses MQTT 3.1.1 by default. It can be changed to use MQTT 3.1 by
   changing value of `MQTT_VERSION` in `PubSubClient.h`.


## Compatible Hardware

The library uses the Arduino, SIMCOM7500 & SIMCOM7600

## License

This code is released under the MIT License.
