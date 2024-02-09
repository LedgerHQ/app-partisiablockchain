/*****************************************************************************
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "buffer.h"
#include "io.h"
#include "ledger_assert.h"

#include "dispatcher.h"
#include "../constants.h"
#include "../globals.h"
#include "../types.h"
#include "../status_words.h"
#include "../handler/get_version.h"
#include "../handler/get_app_name.h"
#include "../handler/get_address.h"
#include "../handler/sign_tx.h"

int apdu_dispatcher(const command_t *cmd) {
    LEDGER_ASSERT(cmd != NULL, "NULL cmd");

    if (cmd->cla != CLA) {
        return io_send_sw(SW_CLA_NOT_SUPPORTED);
    }

    buffer_t buf = {0};

    switch (cmd->ins) {
        case GET_VERSION:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_version();
        case GET_APP_NAME:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_app_name();
        case GET_ADDRESS:
            if (cmd->p1 > 1 || cmd->p2 > 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            return handler_get_address(&buf, (bool) cmd->p1);
        case SIGN_TX:
            if (cmd->p1 == P1_FIRST_CHUNK && cmd->p2 != P2_NOT_LAST_CHUNK) {
                return io_send_sw(SW_WRONG_P1P2);
            } else if (cmd->p1 != P1_FIRST_CHUNK && cmd->p1 != P1_NOT_FIRST_CHUNK) {
                return io_send_sw(SW_WRONG_P1P2);
            } else if (cmd->p2 != P2_LAST_CHUNK && cmd->p2 != P2_NOT_LAST_CHUNK) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            bool first_chunk = !((bool) (cmd->p1 & P1_NOT_FIRST_CHUNK));
            bool not_last_chunk = (bool) (cmd->p2 & P2_NOT_LAST_CHUNK);
            return handler_sign_tx(&buf, first_chunk, not_last_chunk);
        default:
            return io_send_sw(SW_INS_NOT_SUPPORTED);
    }
}
