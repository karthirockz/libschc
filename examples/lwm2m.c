/*
 * (c) 2018 - idlab - UGent - imec
 *
 * Bart Moons
 *
 * This file is part of the SCHC stack implementation
 *
 * This is a basic example on how to compress 
 * and decompress a packet
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "../schc.h"
#include "../compressor.h"

#define MAX_PACKET_LENGTH		256

// the ipv6/udp/coap packet
uint8_t msg[] = {
		// IPv6 header
		0x60, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x11, 0x40, 0xCC, 0xCC, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x02,
		// UDP header
		0x33, 0x16, 0x33, 0x16, 0x00, 0x1E, 0x27, 0x4E,
		// LwM2M registration header
		0x44, 0x02, 0x89, 0xC4, 0xC4, 0x89, 0x0A, 0x00, 0xB2, 0x72, 0x64, 0x11,
		0x28, 0x39, 0x6C, 0x77, 0x6D, 0x32, 0x6D, 0x3D, 0x31, 0x2E, 0x31, 0x0D,
		0x02, 0x65, 0x70, 0x3D, 0x6C, 0x77, 0x6D, 0x32, 0x6D, 0x2D, 0x63, 0x6C,
		0x69, 0x65, 0x6E, 0x74, 0x03, 0x62, 0x3D, 0x55, 0x06, 0x6C, 0x74, 0x3D,
		0x33, 0x30, 0x30, 0xFF,
		// LwM2M data
		0x3C, 0x2F, 0x3E, 0x3B, 0x72, 0x74, 0x3D, 0x22, 0x6F, 0x6D, 0x61, 0x2E,
		0x6C, 0x77, 0x6D, 0x32, 0x6D, 0x22, 0x2C, 0x3C, 0x2F, 0x31, 0x2F, 0x30,
		0x3E, 0x2C, 0x3C, 0x2F, 0x33, 0x2F, 0x30, 0x3E, 0x2C, 0x3C, 0x2F, 0x33,
		0x33, 0x30, 0x33, 0x2F, 0x30, 0x3E, 0x2C, 0x3C, 0x2F, 0x33, 0x33, 0x30,
		0x34, 0x2F, 0x30, 0x3E, 0x2C, 0x3C, 0x2F, 0x33, 0x33, 0x33, 0x36, 0x2F,
		0x30, 0x3E
};

int main() {
	// COMPRESSION
	// initialize the client compressor
	uint8_t src[16] = { 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
	schc_compressor_init(src);
	
	uint8_t compressed_buf[MAX_PACKET_LENGTH];
	uint32_t device_id = 0x01;

	// compress packet
	struct schc_rule_t* schc_rule;
	schc_bitarray_t bit_arr;
	bit_arr.ptr = (uint8_t*) (compressed_buf);

	int compressed_len = schc_compress(msg, sizeof(msg), &bit_arr, device_id,
			UP, &schc_rule);

	// DECOMPRESSION
	uint8_t new_packet_len = 0;

	// NOTE: DIRECTION remains UP as this packet is forwarded to the IPv6 network
	unsigned char decomp_packet[MAX_PACKET_LENGTH] = { 0 };
	new_packet_len = schc_decompress(&bit_arr, decomp_packet, device_id,
			compressed_len, UP);
	if(new_packet_len == 0) { // some error occured
		return 1;
	}

 	return 0;
 }
