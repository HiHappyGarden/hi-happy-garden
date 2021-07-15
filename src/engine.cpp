// MIT License
//
// Copyright (c) 2021 Happy GardenPI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "engine.hpp"

#include <wiringPi.h>
#include <mosquitto.h>
#include <csignal>
#include <syslog.h>

#include <thread>
using namespace std;

#include "services/lockservice.hpp"
using std::make_unique;

#include "services/logservice.hpp"
#include "services/configservice.hpp"
#include "constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        //enable loop
        static volatile bool run = true;

        //exit signal handler
        static __sighandler_t handleSignal = [](int)
        {
            run = false;
        };

        Engine::~Engine() noexcept
        {
            delete factory;
        }

        void initialize()
        {
            Engine::getInstance()->factory = new (nothrow) FactoryConcrete;
            if (!Engine::getInstance()->factory) {
                throw runtime_error("no memory for Engine::getInstance()->factory");
            }

            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());

            system->initialize();

            device->setLogService(system->getLogService());

            device->initialize();

            // //check if already run an instance of Happy GardenPI
            // if (Globals::getInstance()->lockService->lock())
            // {
            //     string error("another instance already run pid:");
            //     error += to_string(Globals::getInstance()->lockService->getPidInExecution());
            //     LogService::getInstance()->write(LOG_ERR, "%s", error.c_str());
            //     throw runtime_error(error);
            // }

            // //print init info on log
            // LogService::getInstance()->write(LOG_INFO, "version: %s", HGARDENPI_VER);

            // //get device info
            // Globals::getInstance()->deviceInfo = getDeviceInfo();

            // //print device info to log
            // LogService::getInstance()->write(LOG_INFO, "hardware: %s", Globals::getInstance()->deviceInfo->hardhare.c_str());
            // LogService::getInstance()->write(LOG_INFO, "revision: %s", Globals::getInstance()->deviceInfo->revision.c_str());
            // LogService::getInstance()->write(LOG_INFO, "serial: %s", Globals::getInstance()->deviceInfo->serial.c_str());
            // LogService::getInstance()->write(LOG_INFO, "model: %s", Globals::getInstance()->deviceInfo->model.c_str());
            // LogService::getInstance()->write(LOG_INFO, "cpu: %d", Globals::getInstance()->deviceInfo->cpu);

            // //HW check
            // if (Globals::getInstance()->deviceInfo->hardhare != HW_V1)
            // {
            //     HGARDENPI_ERROR_LOG_AMD_THROW("hardware not supporrted, you need a Raspberry Pi Zero W")
            // }

            // ConfigService config(HGARDENPI_FILE_CONFIG);

            // config.read();

            // //initialize WiringPI
            // wiringPiSetupGpio();

            // //initialize mosquittopp
            // if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS)
            // {
            //     HGARDENPI_ERROR_LOG_AMD_THROW("mosquitto_lib_init() error")
            // }

            // //Globals::getInstance()->mqttClient = make_shared<MQTTClientMosquitto>(Globals::getInstance()->deviceInfo->serial, HGARDENPI_MQTT_BROKER_HOST, HGARDENPI_MQTT_BROKER_USER, HGARDENPI_MQTT_BROKER_PASSWD, HGARDENPI_MQTT_BROKER_PORT);
        }

        void start()
        {

            signal(SIGINT, handleSignal);
            signal(SIGTERM, handleSignal);

            while (run)
            {
                //Globals::getInstance()->mqttClient->loop(run);
                // while (run)
                // {
                //     this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(Time::TICK)));
                // }
                // this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(Time::TICK)));
            }
        }
    }
}