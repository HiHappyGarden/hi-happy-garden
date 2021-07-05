/*
MIT License

Copyright (c) 2021 Happy GardenPI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mqttclient.hpp"

#include <mosquitto.h>
#include <unistd.h>

#include <cstring>
#include <sstream>

#include "../services/logservice.hpp"
#include "../constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        MQTTClient::MQTTClient(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port, uint16_t keepAlive) : topic("/HappyGardenPI/" + serial),
                                                                                                                                                        user(user),
                                                                                                                                                        passwd(passwd)
        {

            ostringstream ss;
            ss << getpid();

            /* 
            * new instance client
            */
            mosq = mosquitto_new(ss.str().c_str(), true, 0);

            if (mosq)
            {
                /* 
                * Connection callback
                * call when yhe broker send CONNACK
                */
                mosquitto_connect_callback_set(mosq, [](mosquitto *mosq, void *obj, int result)
                                               { cout << to_string(result) << endl; });

                /* 
                * Subscription callback
                * call when the message will senf succesfully to broker
                */
                mosquitto_message_callback_set(mosq, [](mosquitto *mosq, void *obj, const mosquitto_message *message)
                                               {
                                                   bool match = 0;
                                                   printf("received message '%.*s' for topic '%s'\n", message->payloadlen, (char *)message->payload, message->topic);

                                                   //    mosquitto_topic_matches_sub(, message->topic, &match);
                                                   //    if (match)
                                                   //    {
                                                   //        printf("received message for Telemetry topic\n");
                                                   //    }
                                               });
                int rc = mosquitto_connect(mosq, host.c_str(), port, keepAlive);

                LogService::getInstance()->write(LOG_INFO, "%s: %s", "topic", topic.c_str());
                mosquitto_subscribe(mosq, nullptr, topic.c_str(), 0);
            }
        }

        void MQTTClient::loop(volatile bool &run)
        {

            int rc = mosquitto_loop(mosq, static_cast<int>(Time::TICK), 1);

            if (run && rc)
            {
                LogService::getInstance()->write(LOG_WARNING, "%s", "connection error! Try to reconnect");
                sleep(5);
                mosquitto_reconnect(mosq);
            }
        }

    }

}
