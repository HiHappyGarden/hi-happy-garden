/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#include "pico/pico-gpio.hpp"
#include "pico/pico-uart.hpp"
#include "pico/pico-i2c.hpp"
#include "pico/pico-rotary-encored.hpp"
using namespace os;

#include <hardware/gpio.h>

#include "osal/osal.hpp"

namespace hhg::driver
{
inline namespace v1
{

    thread t{"test", 1, 1024, [](auto i) -> void* {
        while (1)
        {
//            gpio_put(20, false);
//            osal_us_sleep(500_ms);
//            gpio_put(20, true);
//            osal_us_sleep(1000_ms);
        }
        return nullptr;
    }};

    bool open;

os::exit init_gpio()
{

    gpio_set_function(pico_uart::TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(pico_uart::RX_PIN, GPIO_FUNC_UART);


//    gpio_set_function(pico_i2c::SDA_PIN, GPIO_FUNC_I2C);
//    gpio_set_function(pico_i2c::SCL_PIN, GPIO_FUNC_I2C);
//    gpio_pull_up(pico_i2c::SDA_PIN);
//    gpio_pull_up(pico_i2c::SCL_PIN);


//    gpio_init(pico_rotary_encoder::ENCODER_A);
//    gpio_init(pico_rotary_encoder::ENCODER_B);
//    gpio_init(pico_rotary_encoder::ENCODER_BTN);
//    gpio_set_dir(pico_rotary_encoder::ENCODER_A, GPIO_IN);
//    gpio_set_dir(pico_rotary_encoder::ENCODER_B, GPIO_IN);
//    gpio_set_dir(pico_rotary_encoder::ENCODER_BTN, GPIO_IN);
//    gpio_disable_pulls(pico_rotary_encoder::ENCODER_A);
//    gpio_disable_pulls(pico_rotary_encoder::ENCODER_B);
//    gpio_disable_pulls(pico_rotary_encoder::ENCODER_BTN);
//    gpio_set_irq_enabled(pico_rotary_encoder::ENCODER_A, GPIO_IRQ_EDGE_FALL, true);
//    gpio_set_irq_enabled(pico_rotary_encoder::ENCODER_B, GPIO_IRQ_EDGE_FALL, true);
//    gpio_set_irq_enabled(pico_rotary_encoder::ENCODER_BTN, GPIO_IRQ_EDGE_FALL, true);
//    gpio_set_irq_enabled_with_callback(pico_rotary_encoder::ENCODER_BTN, GPIO_IRQ_EDGE_FALL, true, &pico_rotary_encoder_callback);

    gpio_init(pico_rotary_encoder::ENCODER_A);
    gpio_set_dir(pico_rotary_encoder::ENCODER_A, GPIO_IN);
    gpio_disable_pulls(pico_rotary_encoder::ENCODER_A);

    gpio_init(pico_rotary_encoder::ENCODER_B);
    gpio_set_dir(pico_rotary_encoder::ENCODER_B, GPIO_IN);
    gpio_disable_pulls(pico_rotary_encoder::ENCODER_B);

    gpio_init(pico_rotary_encoder::ENCODER_BTN);
    gpio_set_dir(pico_rotary_encoder::ENCODER_BTN, GPIO_IN);
    gpio_disable_pulls(pico_rotary_encoder::ENCODER_BTN);

    gpio_set_irq_enabled_with_callback(pico_rotary_encoder::ENCODER_BTN, GPIO_IRQ_EDGE_FALL, true, pico_rotary_encoder::encoder_callback);
    gpio_set_irq_enabled(pico_rotary_encoder::ENCODER_A, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(pico_rotary_encoder::ENCODER_B, GPIO_IRQ_EDGE_FALL, true);
   // t.create();

    return exit::OK;
}

}
}