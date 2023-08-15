/***************************************************************************
 *
 * PROJECT
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
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

#pragma once
#include "osal/osal.hpp"

namespace hhg::app
{
inline namespace v1
{

using os::string;
using os::unique_ptr;

enum class status
{
    UNACTIVE,
    ACTIVE,
    RUN
};

/**
 * @brief pod who describe a irrigation station
 */
struct station final
{

    /**
    * @brief name of station
    */
    string<32> name;

    /**
    * @brief description of station
    */
    string<128> description;

    /**
    * @brief relay number association
    */
    uint8_t relay_number;

    /**
    * @brief watering time in minutes
    */
    uint watering_time;

    /**
    * @brief for manage order of execution lighter is first then weightier
    */
    uint16_t weight;

    /**
     * @brief status of station
     */
    enum status status;
};

struct zone
{
    static inline const constexpr uint8_t NOT_SET = 0xFF;

    /**
    * @brief minute, values allowed 0 - 59
    */
    uint8_t minute = 0; //0 - 59 or NOT_SET

    /**
    * @brief minute, values allowed 0 - 23 or NOT_SET
    */
    uint8_t hour = 0; //0 - 23 or NOT_SET

    /**
    * @brief days, values allowed 0x01 - 0x7F or NOT_SET
    */
    union
    {
        struct
        {

            uint8_t mon: 1;
            uint8_t tue: 1;
            uint8_t wen: 1;
            uint8_t thu: 1;
            uint8_t fri: 1;
            uint8_t sat: 1;
            uint8_t sun: 1;
        };
        uint8_t data;

    }days;

    /**
     * @brief brief description of aggregation
     */
    string<128> description;

    /**
     * @brief status of station
     */
    enum status status = status::ACTIVE;

    /**
     * @brief number of station
     */
    uint8_t stations_size = 0;

    /**
     * @brief number of station
     */
    unique_ptr<station> stations;
};

}
}
