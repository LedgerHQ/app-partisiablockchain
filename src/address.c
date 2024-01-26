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

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "os.h"
#include "cx.h"
#include "ledger_assert.h"

#include "address.h"

#include "transaction/types.h"

bool blockchain_address_from_pubkey(const uint8_t public_key[static 65],
                                    blockchain_address_s *out) {
    LEDGER_ASSERT(out != NULL, "NULL out");

    out->raw_bytes[0] = BLOCKCHAIN_ADDRESS_ACCOUNT;

    cx_sha256_t digest;

    if (cx_hash_init((cx_hash_t *) &digest, CX_SHA256) != CX_OK) {
        return false;
    }

    return cx_hash_no_throw((cx_hash_t *) &digest,
                            CX_LAST,
                            public_key + 1,
                            64,
                            out->raw_bytes+1,
                            IDENTIFIER_LEN) != CX_OK;
}
