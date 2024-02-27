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

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <string.h>  // memmove

#include "buffer.h"

#include "send_response.h"
#include "../constants.h"
#include "../globals.h"
#include "../status_words.h"

#include "io.h"

WARN_UNUSED_RESULT
int helper_send_response_address() {
    return io_send_response_pointer(G_context.pk_info.address.raw_bytes,
                                    sizeof(G_context.pk_info.address.raw_bytes),
                                    SW_OK);
}

WARN_UNUSED_RESULT
int helper_send_response_sig(void) {
    // Serialize signature
    uint8_t signature_bytes[32 + 32 + 1] = {0};
    signature_bytes[0] = G_context.tx_info.signature.recovery_id;
    memmove(&signature_bytes[1], G_context.tx_info.signature.r, 32);
    memmove(&signature_bytes[33], G_context.tx_info.signature.s, 32);

    // Send signature
    return io_send_response_pointer(signature_bytes, sizeof(signature_bytes), SW_OK);
}
