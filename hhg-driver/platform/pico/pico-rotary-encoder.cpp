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

//ref https://www.reddit.com/r/raspberrypipico/comments/pacarb/sharing_some_c_code_to_read_a_rotary_encoder/

#include "pico/pico-rotary-encored.hpp"
using namespace os;

#include <hardware/gpio.h>

namespace hhg::driver
{
inline namespace v1
{

    pico_rotary_encoder::pico_rotary_encoder() OS_NOEXCEPT = default;

    pico_rotary_encoder::~pico_rotary_encoder() OS_NOEXCEPT = default;


    os::exit pico_rotary_encoder::init(os::error **error) OS_NOEXCEPT
    {
        if(singleton)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_rotary_encoder::init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
        singleton = this;

        polling.create();

        return exit::OK;
    }

    void* pico_rotary_encoder::encoder_handle(void* arg)
    {

        bool last_a = false;
        bool last_b = false;
        bool last_btn = false;
        while(singleton->run)
        {
            bool a = gpio_get(ENCODER_A);
            bool b = gpio_get(ENCODER_B);
            bool btn = gpio_get(ENCODER_BTN);


            bool a_tmp = gpio_get(ENCODER_A);
            bool b_tmp = gpio_get(ENCODER_B);

            if(a && !b && last_a != a)
            {
                (singleton->obj->*singleton->callback)(true, false, !btn && last_btn != btn);
            }
            else if(!a && b && last_b != b)
            {
                (singleton->obj->*singleton->callback)(false, true, !btn && last_btn != btn);
            }

            if(!btn && last_btn != btn)
            {
                (singleton->obj->*singleton->callback)(a && !b && last_a != a, !a && b && last_b != b, true);
            }

            last_a = a;
            last_b = b;
            last_btn = btn;

            osal_us_sleep(10_ms);
        }

        return nullptr;
    }

}
}

