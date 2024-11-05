/****************************************************************************
 *
 *   Copyright (c) 2012-2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file mavlink_bridge_header
 * MAVLink bridge header for UART access.
 *
 * @author Lorenz Meier <lm@inf.ethz.ch>
 */

/* MAVLink adapter header */
#ifndef MAVLINK_BRIDGE_HEADER_H
#define MAVLINK_BRIDGE_HEADER_H

#define MAVLINK_NO_CONVERSION_HELPERS

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

/* use efficient approach, see mavlink_helpers.h */
#define MAVLINK_SEND_UART_BYTES mavlink_send_uart_bytes

#define MAVLINK_START_UART_SEND mavlink_start_uart_send
#define MAVLINK_END_UART_SEND mavlink_end_uart_send

#define MAVLINK_GET_CHANNEL_BUFFER mavlink_get_channel_buffer
#define MAVLINK_GET_CHANNEL_STATUS mavlink_get_channel_status

#if !defined(CONSTRAINED_MEMORY)
# define MAVLINK_COMM_NUM_BUFFERS 6
# define MAVLINK_COMM_4 static_cast<mavlink_channel_t>(4)
# define MAVLINK_COMM_5 static_cast<mavlink_channel_t>(5)
#endif



#include "mesl_crypto_macros.h"

// -------------------------------------------------------
// Setting dependencies.
// -------------------------------------------------------

#ifdef MAVLINK_USE_MESSAGE_INFO
// require 'offsetof'
#include <stddef.h> 
#endif



#include <mavlink_types.h>



#ifdef MESL_MAV_TEBUG

void mav_tebug_fi_start(mavlink_status_t* status);
void mav_tebug_send_end(mavlink_status_t* status);
void mav_tebug_parse_start(mavlink_status_t* status);
void mav_tebug_parse_end(mavlink_status_t* status);
#define MESL_MAV_TEBUG_FI_SEND_START(status) mav_tebug_fi_start(status);
#define MESL_MAV_TEBUG_SEND_END(status) mav_tebug_send_end(status);
#define MESL_MAV_TEBUG_PARSE_START(status) mav_tebug_parse_start(status);
#define MESL_MAV_TEBUG_PARSE_END(rxmsg, status) mav_tebug_parse_end(status);

#endif // #ifdef MESL_MAV_TEBUG



#include <unistd.h>

__BEGIN_DECLS

/* Struct that stores the communication settings of this system.
   you can also define / alter these settings elsewhere, as long
   as they're included BEFORE mavlink.h.
   So you can set the

   mavlink_system.sysid = 100; // System ID, 1-255
   mavlink_system.compid = 50; // Component/Subsystem ID, 1-255

   Lines also in your main.c, e.g. by reading these parameter from EEPROM.
 */
extern mavlink_system_t mavlink_system;

/**
 * @brief Send multiple chars (uint8_t) over a comm channel
 *
 * @param chan MAVLink channel to use, usually MAVLINK_COMM_0 = UART0
 * @param ch Character to send
 */
void mavlink_send_uart_bytes(mavlink_channel_t chan, const uint8_t *ch, int length);

void mavlink_start_uart_send(mavlink_channel_t chan, int length);
void mavlink_end_uart_send(mavlink_channel_t chan, int length);

extern mavlink_status_t *mavlink_get_channel_status(uint8_t chan);
extern mavlink_message_t *mavlink_get_channel_buffer(uint8_t chan);

#include <mavlink.h>
#if !MAVLINK_FTP_UNIT_TEST
#include <uAvionix.h>
#endif



// -------------------------------------------------------
// Declare or implement, for MESL_CRYPTO related things.
//   NOTE: 'MAVLINK_HELPER' is 'static inline' by default.
// -------------------------------------------------------

#ifdef MAVLINK_USE_CXX_NAMESPACE
namespace mavlink {
#endif

#ifdef MESL_CRYPTO

#ifdef MESL_MAVLINK_DEBUG

// @brief  Print debug information,
//           when 'mavlink_mesl_crypto_condition' called.
//         This function is for non-static non-inline function.
void mavlink_mesl_crypto_condition_debuginfo(
		const mavlink_status_t* status,
		uint32_t msgid,
		uint8_t system_id,
		uint8_t component_id,
		const char *payload,
		uint8_t len
		);
#endif // #ifdef MESL_MAVLINK_DEBUG

// @brief  Function to decide if MAVLink payload should be encrypted.
//         Program that use MAVLink should implement this function.
//         This function is for non-static non-inline function.
// @param  'len': payload length (can be 0).
// @return 'MESL_CRYPTO_METHOD_XXX' (true),
//           if MAVLink payload should be encrypted.
//         Zero otherwise.
uint8_t mavlink_mesl_crypto_condition_f(
		mavlink_status_t* status,
		uint32_t msgid,
		uint8_t system_id,
		uint8_t component_id,
		const char *payload,
		uint8_t len
		);

// @brief  Function to decide if MAVLink payload should be encrypted.
//         Program that use MAVLink should implement this function.
// @param  'len': payload length (can be 0).
// @return 'MESL_CRYPTO_METHOD_XXX' (true),
//           if MAVLink payload should be encrypted.
//         Zero otherwise.
MAVLINK_HELPER uint8_t mavlink_mesl_crypto_condition(
		mavlink_status_t* status,
		uint32_t msgid,
		uint8_t system_id,
		uint8_t component_id,
		const char *payload,
		uint8_t len
		)
{
#ifdef MESL_MAVLINK_DEBUG
	mavlink_mesl_crypto_condition_debuginfo(
			status,
			msgid,
			system_id,
			component_id,
			payload,
			len);
#endif // #ifdef MESL_MAVLINK_DEBUG
	return mavlink_mesl_crypto_condition_f(
			status,
			msgid,
			system_id,
			component_id,
			payload,
			len);
}

// @brief  Function to encrypt MAVLink payload,
//           with method 'MESL_CRYPTO_METHOD_AES128'.
// @param  'len': payload length (can be 0).
// @return Payload length after encryption.
// @note   "input_len == 0 && output_len == 0",
//           will be considered as non-encryption.
//         But, "input len != 0 && output_len == 0",
//           or "output_len < 0 || output_len > maxlen",
//           will be considered as error,
//           MAVLink frame will be sent with zero payload length,
//           and receiving side can report error,
//           because the length is zero but encryption iflag is set.
int32_t mavlink_mesl_encrypt_aes128(
		const char *src,
		char *dst,
		uint8_t len,
		uint8_t maxlen
		);

// @brief  Function to decrypt MAVLink payload,
//           with method 'MESL_CRYPTO_METHOD_AES128'.
// @param  'len': payload length.
//         If 'len' is zero,
//           this function should return '-1'.
// @return Payload length after decryption (zero is valid result).
//         If "output_len < 0 || output_len > maxlen",
//           will be considered as error,
//           one example is for invalue 'crypto_method'.
int32_t mavlink_mesl_decrypt_aes128(
		const char *src,
		char *dst,
		uint8_t len,
		uint8_t maxlen
		);


// PX4_ERR("%s", str)
void mavlink_mesl_printerror(const char *str);

// @brief  Function to encrypt MAVLink payload.
//         Program that use MAVLink should implement this function.
// @param  'crypto_method': method for encryption,
//           value should be 'MESL_CRYPTO_METHOD_XXX'.
// @param  'len': payload length (can be 0).
// @return Payload length after encryption.
// @note   "input_len == 0 && output_len == 0",
//           will be considered as non-encryption.
//         But, "input len != 0 && output_len == 0",
//           or "output_len < 0 || output_len > maxlen",
//           will be considered as error,
//           MAVLink frame will be sent with zero payload length,
//           and receiving side can report error,
//           because the length is zero but encryption iflag is set.
MAVLINK_HELPER int32_t mavlink_mesl_encrypt(
		uint8_t crypto_method,
		const char *src,
		char *dst,
		uint8_t len,
		uint8_t maxlen
		)
{
	if (crypto_method == MESL_CRYPTO_METHOD_AES128) {
		return mavlink_mesl_encrypt_aes128(src, dst, len, maxlen);
	}
	if (crypto_method == MESL_CRYPTO_METHOD_USER7) {
		// User defined encryption:
		//   XOR with key '0xab', for test.
		memcpy(
				dst,
				src,
				len);
		for (uint8_t i = (uint8_t)0; i < len; i++) {
			dst[i] ^= 0xab;
		}
		return (int32_t)len;
	}

	mavlink_mesl_printerror("MESL_CRYPTO: invalid crypto method requested");
	// PX4_ERR("MESL_CRYPTO: invalid crypto method requested");
	return (int32_t)0;
}

// @brief  Function to decrypt MAVLink payload.
//         Program that use MAVLink should implement this function.
// @param  'crypto_method': method for decryption,
//           value can be 'MESL_CRYPTO_METHOD_XXX'.
//         For invalid 'crypto_method',
//           this function should return '-1'.
// @param  'len': payload length.
//         If 'len' is zero,
//           this function should return '-1'.
// @return Payload length after decryption (zero is valid result).
//         If "output_len < 0 || output_len > maxlen",
//           will be considered as error,
//           one example is for invalue 'crypto_method'.
MAVLINK_HELPER int32_t mavlink_mesl_decrypt(
		uint8_t crypto_method,
		const char *src,
		char *dst,
		uint8_t len,
		uint8_t maxlen
		)
{
	if (crypto_method == MESL_CRYPTO_METHOD_AES128) {
		return mavlink_mesl_decrypt_aes128(src, dst, len, maxlen);
	}
	if (crypto_method == MESL_CRYPTO_METHOD_USER7) {
		// User defined encryption:
		//   XOR with key '0xab', for test.
		memcpy(
				dst,
				src,
				len);
		for (uint8_t i = (uint8_t)0; i < len; i++) {
			dst[i] ^= 0xab;
		}
		return (int32_t)len;
	}

		return (int32_t)-1;
}

#endif // #ifdef MESL_CRYPTO

#ifdef MESL_MAVLINK_DEBUG

// @brief  Function for debug MAVLink frame parsing result.'
//           This function is for non-static non-inline function.
void mavlink_mesl_parse_result_f(
		const mavlink_message_t* rxmsg,
		const mavlink_status_t* status
		);

// @brief  Function for debug MAVLink frame parsing result.'
MAVLINK_HELPER void mavlink_mesl_parse_result(
		const mavlink_message_t* rxmsg,
		const mavlink_status_t* status
		)
{
	mavlink_mesl_parse_result_f(rxmsg, status);
}

#endif // #ifdef MESL_MAVLINK_DEBUG

#ifdef MAVLINK_USE_CXX_NAMESPACE
} // namespace mavlink
#endif



__END_DECLS

#endif /* MAVLINK_BRIDGE_HEADER_H */
