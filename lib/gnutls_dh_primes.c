/*
 * Copyright (C) 2000,2001 Nikos Mavroyanopoulos
 *
 * This file is part of GNUTLS.
 *
 *  The GNUTLS library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public   
 *  License as published by the Free Software Foundation; either 
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */

#include <gnutls_int.h>
#include <gnutls_errors.h>
#include <gnutls_datum.h>
#include "debug.h"

static uint8 DH_G_1024[] = { 0x05 };
static uint8 DH_G_4096[] = { 0x05 };
static uint8 DH_G_2048[] = { 0x05 };
static uint8 DH_G_3072[] = { 0x0D };

static uint8 diffie_hellman_prime_1024[128] = {
	0xe3, 0x79, 0xb5, 0xa7, 0x47, 0x4c, 0xfd,
	0x9c, 0x78, 0xfe, 0x17, 0x87, 0x44, 0xc4,
	0x86, 0x2b, 0x92, 0x13, 0x43, 0xf5, 0xac,
	0x72, 0xd2, 0xf1, 0x2a, 0xf5, 0x39, 0xa2,
	0x79, 0x01, 0xdd, 0x4c, 0x7e, 0x5b, 0xa0,
	0x19, 0x11, 0xd4, 0x2f, 0x0a, 0x92, 0x8d,
	0xfd, 0xde, 0x85, 0x93, 0x99, 0xad, 0xe0,
	0xd4, 0x0b, 0x62, 0xaa, 0x86, 0xa7, 0xd7,
	0x63, 0x2e, 0x35, 0x96, 0x88, 0xbe, 0x52,
	0x2e, 0x8c, 0x27, 0xf0, 0xe0, 0xa1, 0x0e,
	0xb7, 0xb9, 0xc8, 0xbd, 0x5d, 0xe8, 0xdb,
	0x63, 0xd8, 0xb4, 0xe7, 0x0d, 0xff, 0x0f,
	0x55, 0xe7, 0x27, 0x0d, 0xb7, 0x57, 0x33,
	0x30, 0xd6, 0xeb, 0x51, 0x99, 0x86, 0x17,
	0x5b, 0x48, 0xb3, 0x0c, 0xae, 0xbd, 0xa1,
	0x83, 0x6b, 0xbd, 0x9f, 0x83, 0x83, 0x2b,
	0x46, 0x3e, 0x18, 0xa4, 0x4d, 0x82, 0x95,
	0xa4, 0x08, 0xdd, 0x28, 0x0c, 0x4f, 0x93,
	0xfd, 0xd7
};

/* prime - 4096 bits */
static uint8 diffie_hellman_prime_4096[] = { 0x00,
	0x98, 0xb7, 0x3d, 0x66, 0xf1, 0x18, 0x61,
	0xa9, 0x36, 0xd9, 0xf1, 0xbf, 0x65, 0xbb,
	0x7c, 0x06, 0x10, 0x15, 0xe5, 0x24, 0x47,
	0xb5, 0x45, 0x7e, 0xbb, 0xdf, 0x59, 0xf4,
	0xf2, 0x59, 0x7d, 0xea, 0xe0, 0x0f, 0x06,
	0x42, 0xd8, 0xb1, 0x9b, 0x62, 0xf9, 0x81,
	0x05, 0xd7, 0xd5, 0x74, 0x7c, 0x39, 0x3b,
	0x6d, 0x57, 0xb7, 0xe9, 0x51, 0x0d, 0xb6,
	0xe5, 0x03, 0xf7, 0xf3, 0xac, 0x1b, 0x66,
	0x96, 0xb3, 0xf8, 0xa1, 0xe1, 0xc7, 0x9c,
	0xc7, 0x52, 0x19, 0x2a, 0x90, 0xe6, 0x1d,
	0xba, 0xf5, 0x15, 0xcb, 0x8b, 0x52, 0x88,
	0xcd, 0xf5, 0x50, 0x33, 0x04, 0xb8, 0x2f,
	0x2c, 0x01, 0x57, 0x82, 0x7c, 0x8a, 0xf0,
	0xa3, 0x73, 0x7e, 0x0c, 0x2d, 0x69, 0xd4,
	0x17, 0xf6, 0xd0, 0x6a, 0x32, 0x95, 0x6a,
	0x69, 0x40, 0xb0, 0x55, 0x4f, 0xf0, 0x1d,
	0xae, 0x3d, 0x5f, 0x01, 0x92, 0x14, 0x3a,
	0x73, 0x69, 0x5a, 0x8e, 0xea, 0x22, 0x52,
	0x44, 0xc2, 0xb8, 0x66, 0x1e, 0x26, 0x1a,
	0x5d, 0x8f, 0x46, 0x6b, 0x8d, 0x3c, 0x71,
	0xcf, 0x1d, 0x72, 0x8d, 0x2f, 0x03, 0x54,
	0xdb, 0xe9, 0x82, 0x60, 0xe5, 0xf6, 0x40,
	0x4b, 0x6b, 0xae, 0x0a, 0xb2, 0x30, 0xba,
	0x1c, 0x45, 0x7e, 0x3f, 0xfd, 0xf7, 0xdc,
	0xa6, 0xbb, 0x98, 0xc4, 0xca, 0xfc, 0x66,
	0xf3, 0x48, 0x47, 0xbf, 0xdb, 0xd7, 0xdc,
	0xff, 0x1d, 0xeb, 0xa0, 0x4e, 0xb6, 0xff,
	0x75, 0xdc, 0x0c, 0x1d, 0x93, 0x9e, 0xd5,
	0xb3, 0x68, 0xe7, 0x07, 0x29, 0x91, 0xf1,
	0xae, 0xfc, 0x7e, 0x3a, 0xea, 0xec, 0x40,
	0xfc, 0x70, 0x7f, 0xf3, 0x36, 0x81, 0xec,
	0x97, 0xa7, 0x0d, 0x71, 0x2c, 0x5c, 0x4f,
	0xd9, 0x00, 0xcf, 0x62, 0x56, 0xfb, 0x09,
	0x2d, 0x1b, 0x04, 0x3c, 0x00, 0xc8, 0x17,
	0xd7, 0x7d, 0x16, 0x20, 0x1e, 0x62, 0x9b,
	0xf4, 0x4f, 0xee, 0xa4, 0xbf, 0x0b, 0xde,
	0x51, 0x7c, 0x01, 0x76, 0x79, 0x73, 0x7d,
	0x7b, 0xec, 0xee, 0x14, 0xec, 0x83, 0xc3,
	0xb4, 0x42, 0x66, 0x19, 0x52, 0x19, 0x04,
	0x02, 0x71, 0x61, 0x5c, 0x78, 0xee, 0x5f,
	0x58, 0x1e, 0x5b, 0x2d, 0xf3, 0x0c, 0x6e,
	0x00, 0x0f, 0xd8, 0xf0, 0x86, 0xa1, 0x11,
	0xfd, 0x04, 0x07, 0xa6, 0xf7, 0x31, 0xb9,
	0xf6, 0x76, 0xfc, 0xea, 0xf0, 0x16, 0x98,
	0x37, 0x48, 0x1b, 0x0c, 0x32, 0x3f, 0x7e,
	0xfa, 0x02, 0x04, 0x2a, 0x48, 0x70, 0xb4,
	0xe3, 0xe0, 0xc1, 0x7f, 0x65, 0x70, 0xd0,
	0x71, 0x74, 0x86, 0xb7, 0x5d, 0xd4, 0x84,
	0xd5, 0x9d, 0x77, 0xf6, 0x72, 0x82, 0x4b,
	0x98, 0x8b, 0x49, 0x3a, 0x0b, 0x1e, 0x94,
	0x42, 0xf7, 0x0b, 0x3f, 0xec, 0xc2, 0x2b,
	0x7f, 0x55, 0xe2, 0x94, 0x48, 0xac, 0x04,
	0xb9, 0xb2, 0xb6, 0xca, 0xb4, 0x09, 0xe3,
	0xba, 0x6a, 0x55, 0x28, 0xf7, 0x8a, 0x73,
	0x4d, 0x21, 0xe1, 0xf4, 0xcd, 0x22, 0x15,
	0x9c, 0xe6, 0xcc, 0x1d, 0x9f, 0x81, 0x88,
	0x4c, 0x5a, 0x17, 0x9f, 0xe5, 0x8c, 0x85,
	0xf1, 0xa3, 0xcf, 0x6c, 0xa1, 0xbf, 0x5e,
	0x02, 0x61, 0xa8, 0x67, 0x6f, 0xb8, 0x20,
	0x1a, 0x4e, 0xf2, 0x05, 0xd7, 0xb4, 0x4b,
	0x3e, 0xca, 0x87, 0x49, 0x10, 0x16, 0xcc,
	0xc9, 0xe0, 0x1c, 0xc1, 0x83, 0xc7, 0xa0,
	0x54, 0x3d, 0x36, 0x17, 0x84, 0xc3, 0x84,
	0x2e, 0x5a, 0xe0, 0x75, 0x45, 0x01, 0xe6,
	0xf0, 0x3d, 0xf9, 0x33, 0x0a, 0xd9, 0x1e,
	0x66, 0x99, 0xb4, 0x21, 0xed, 0x6e, 0xda,
	0x6f, 0x37, 0x33, 0xdd, 0x8f, 0x25, 0x35,
	0x5e, 0x6c, 0x1e, 0x33, 0xc2, 0x41, 0x3f,
	0x58, 0x40, 0xbb, 0xe7, 0x2b, 0x54, 0xdb,
	0xd8, 0xcf, 0x3a, 0xba, 0x0c, 0xf1, 0x19,
	0xec, 0x9d, 0x50, 0xf6, 0x63, 0x22, 0x55,
	0x5e, 0x79, 0xd1, 0x3f, 0x46, 0x0f, 0xf3,
	0x7f
};

/* prime - 3072 bits */
static uint8 diffie_hellman_prime_3072[] = { 0x00,
	0xd5, 0x6e, 0xc8, 0x1f, 0xe9, 0x80, 0x9e,
	0x56, 0x35, 0x6d, 0x6d, 0xdb, 0xfa, 0x47,
	0x75, 0xcd, 0xfa, 0x32, 0x52, 0x1a, 0xc8,
	0xad, 0xee, 0xb0, 0xdb, 0xb7, 0x07, 0x58,
	0xa6, 0x42, 0xfe, 0x59, 0xfb, 0xce, 0xe8,
	0x12, 0x63, 0x09, 0x9f, 0x5d, 0x15, 0x25,
	0x49, 0xf2, 0x61, 0x83, 0xd8, 0x5c, 0x81,
	0xdd, 0x4c, 0x26, 0xe6, 0x24, 0xce, 0x6a,
	0xa5, 0x07, 0x80, 0x1c, 0x3d, 0x94, 0xd1,
	0x5d, 0x73, 0xbd, 0x26, 0x48, 0x22, 0x25,
	0xdd, 0x2f, 0x64, 0xe5, 0xed, 0xb3, 0xa9,
	0x94, 0xb3, 0x96, 0x88, 0x5d, 0x06, 0x41,
	0x80, 0xf8, 0xe1, 0x3c, 0x8f, 0xa9, 0x5b,
	0x44, 0x7e, 0x32, 0xbd, 0x62, 0x37, 0xe1,
	0xde, 0x18, 0xe8, 0x12, 0x7d, 0x28, 0x7d,
	0x5c, 0xcf, 0xa9, 0x16, 0x0f, 0xdc, 0xc1,
	0x92, 0xe0, 0x43, 0xac, 0xd0, 0x25, 0x37,
	0x8e, 0x5d, 0x4d, 0x26, 0x46, 0xbc, 0xc5,
	0x22, 0x05, 0x29, 0x41, 0x53, 0x2f, 0x7a,
	0x95, 0xa8, 0x36, 0xed, 0x85, 0xac, 0xf3,
	0xde, 0x0c, 0xbe, 0xa9, 0xfa, 0xc4, 0xa6,
	0x0b, 0x23, 0xfc, 0x7c, 0x77, 0xdc, 0x7c,
	0x94, 0x9b, 0x7c, 0xe0, 0x3b, 0xa1, 0x66,
	0x78, 0x85, 0x99, 0x5a, 0xba, 0x26, 0xa3,
	0xac, 0x97, 0xd4, 0x3a, 0x33, 0xee, 0xa3,
	0x96, 0xe0, 0x16, 0xdf, 0x61, 0xe7, 0x1f,
	0x35, 0xa5, 0x47, 0x54, 0x51, 0xce, 0x93,
	0x40, 0x6f, 0x40, 0x86, 0x3b, 0x17, 0x12,
	0xd3, 0x4d, 0x2e, 0xb3, 0x04, 0xf8, 0x8b,
	0x30, 0xb1, 0x27, 0xd7, 0xeb, 0xde, 0xd7,
	0xa9, 0x06, 0xfe, 0x6b, 0x59, 0x8c, 0x5d,
	0x9f, 0x93, 0x1f, 0x12, 0x65, 0xe6, 0xa6,
	0xeb, 0x5d, 0x4b, 0x9a, 0x16, 0x85, 0xce,
	0x18, 0x16, 0x5a, 0x5c, 0x3c, 0xeb, 0xc0,
	0xe1, 0x58, 0x64, 0x06, 0x38, 0x1c, 0x66,
	0x90, 0x4a, 0x30, 0xbe, 0x82, 0xe9, 0x9b,
	0x40, 0x2e, 0x6a, 0x91, 0x4f, 0x48, 0xc2,
	0x82, 0x40, 0xe9, 0xcd, 0x87, 0x77, 0x24,
	0xa7, 0xdc, 0x26, 0x05, 0x18, 0x9c, 0x8b,
	0x0e, 0x84, 0x29, 0x57, 0x76, 0x66, 0x7d,
	0x1e, 0x39, 0xc2, 0xf6, 0x2f, 0xbb, 0xeb,
	0x6e, 0x58, 0x3b, 0x11, 0x70, 0x75, 0xdb,
	0xe9, 0xf8, 0xcb, 0xd4, 0x4c, 0x84, 0xb3,
	0xcb, 0x66, 0x81, 0x4e, 0x93, 0xd9, 0x2f,
	0xc5, 0x60, 0x53, 0x69, 0x6e, 0xf3, 0x8e,
	0xa5, 0x6a, 0xa0, 0x96, 0xae, 0x31, 0xb6,
	0x12, 0x91, 0x0e, 0xc4, 0xc9, 0xd0, 0x50,
	0xf7, 0xbc, 0xe7, 0x78, 0xc9, 0x97, 0x02,
	0x26, 0x6a, 0xe3, 0x9a, 0x16, 0x63, 0xa2,
	0x5e, 0x1d, 0x4e, 0x71, 0x52, 0xb4, 0x73,
	0x31, 0x27, 0x6c, 0x46, 0xe4, 0x67, 0x02,
	0xde, 0x34, 0x7e, 0x24, 0x3b, 0xb9, 0xfe,
	0x08, 0x7e, 0xe9, 0x0a, 0xdc, 0xe7, 0xc2,
	0xa6, 0xa6, 0xb3, 0x7d, 0xe0, 0xa2, 0xe7,
	0x6d, 0x2e, 0x33, 0xed, 0x47, 0xf7
};

/* prime - 2048 bits */
static uint8 diffie_hellman_prime_2048[] = { 0x00,
	0xf0, 0x49, 0x65, 0x6d, 0x24, 0x61, 0xe6,
	0x86, 0x8e, 0x57, 0x2b, 0x9b, 0x1c, 0x53,
	0x2e, 0xef, 0xd2, 0x6e, 0xe5, 0x6c, 0xc4,
	0x0c, 0x77, 0x1d, 0xce, 0xc7, 0xe0, 0x92,
	0x78, 0x8b, 0x2b, 0x80, 0x9f, 0xc4, 0x59,
	0xb5, 0x2e, 0xeb, 0x81, 0x8b, 0xfa, 0x08,
	0x9f, 0x02, 0x5e, 0x94, 0x85, 0xab, 0xab,
	0x08, 0x8a, 0x71, 0xb5, 0x0c, 0x26, 0x63,
	0x2f, 0x34, 0x10, 0xdf, 0x32, 0x9a, 0xa1,
	0xd5, 0xb5, 0xd7, 0xa1, 0x46, 0x24, 0x9a,
	0xe3, 0x2a, 0xf1, 0x3a, 0x52, 0xc4, 0xa4,
	0xe6, 0xa2, 0x29, 0x5e, 0x49, 0x0e, 0x2a,
	0x4d, 0xad, 0xcd, 0x92, 0xb6, 0xa5, 0x25,
	0xe5, 0x09, 0xae, 0x76, 0xe4, 0x19, 0xec,
	0x29, 0x9b, 0x9b, 0xdb, 0x0c, 0xc8, 0x28,
	0x1c, 0x49, 0x11, 0x45, 0x30, 0x51, 0x73,
	0x31, 0x18, 0x9e, 0xa5, 0x89, 0x7d, 0x17,
	0x22, 0xd5, 0x49, 0xaf, 0xf6, 0xe5, 0x00,
	0x55, 0x7f, 0x2b, 0x33, 0x2d, 0x2f, 0x89,
	0x73, 0x0b, 0x4d, 0x44, 0x72, 0xb1, 0x2e,
	0xa3, 0x68, 0xbe, 0x52, 0x4e, 0x5a, 0x66,
	0x36, 0xf9, 0x2c, 0xe7, 0xce, 0x92, 0x4d,
	0x0c, 0xa3, 0xc7, 0x85, 0x7e, 0xe6, 0x97,
	0x02, 0x8b, 0x0c, 0xcb, 0xf3, 0x6f, 0x2e,
	0x04, 0xed, 0x6e, 0x75, 0xcf, 0xd1, 0xd4,
	0x9f, 0xd3, 0x44, 0x3e, 0x5f, 0x81, 0xaa,
	0xc1, 0xb8, 0xe2, 0xab, 0xed, 0x3b, 0xfc,
	0xeb, 0x47, 0x48, 0xee, 0xe5, 0xfd, 0xc2,
	0x79, 0x7a, 0x01, 0xe9, 0xab, 0xc6, 0x34,
	0x65, 0x6a, 0x0a, 0x6c, 0xe8, 0x89, 0xa6,
	0x96, 0xd2, 0x1e, 0xe5, 0xbe, 0x58, 0xf2,
	0xcf, 0x17, 0xb8, 0x75, 0x43, 0xec, 0x0b,
	0xb2, 0x91, 0x50, 0x93, 0x4c, 0xd2, 0xa3,
	0xa4, 0x8a, 0x67, 0x23, 0x7f, 0x86, 0xac,
	0xe3, 0x56, 0x9b, 0x18, 0x03, 0x03, 0x70,
	0x50, 0x7b, 0x1a, 0x02, 0x22, 0x0b, 0x93,
	0xc8, 0x9b, 0xa8, 0x8f
};

/* Holds the prime to be used in DH authentication.
 * Initialy the GNUTLS_MPIs are not calculated (must call global_init, or _gnutls_dh_calc_mpis()).
 */
_gnutls_dh_params _gnutls_dh_default_params[] = {
	{768, NULL, NULL, {DH_G_1024, sizeof(DH_G_1024)}
	 , {diffie_hellman_prime_1024, sizeof diffie_hellman_prime_1024}
	 , 0}
	,
	{1024, NULL, NULL, {DH_G_1024, sizeof(DH_G_1024)}
	 , {diffie_hellman_prime_1024, sizeof diffie_hellman_prime_1024}
	 , 0}
	,
	{2048, NULL, NULL, {DH_G_2048, sizeof(DH_G_2048)}
	 , {diffie_hellman_prime_2048, sizeof diffie_hellman_prime_2048}
	 , 0}
	,
	{3072, NULL, NULL, {DH_G_3072, sizeof(DH_G_3072)}
	 , {diffie_hellman_prime_3072, sizeof diffie_hellman_prime_3072}
	 , 0}
	,
	{4096, NULL, NULL, {DH_G_4096, sizeof(DH_G_4096)}
	 , {diffie_hellman_prime_4096, sizeof diffie_hellman_prime_4096}
	 , 0}
	,
	{0, NULL, NULL, {NULL, 0}
	 , {NULL, 0}
	 , 0}
};

static const
     _gnutls_dh_params _gnutls_dh_copy_params[] = {
	{768, NULL, NULL, {DH_G_1024, sizeof(DH_G_1024)}
	 , {diffie_hellman_prime_1024, sizeof diffie_hellman_prime_1024}
	 , 0}
	,
	{1024, NULL, NULL, {DH_G_1024, sizeof(DH_G_1024)}
	 , {diffie_hellman_prime_1024, sizeof diffie_hellman_prime_1024}
	 , 0}
	,
	{2048, NULL, NULL, {DH_G_2048, sizeof(DH_G_2048)}
	 , {diffie_hellman_prime_2048, sizeof diffie_hellman_prime_2048}
	 , 0}
	,
	{3072, NULL, NULL, {DH_G_3072, sizeof(DH_G_3072)}
	 , {diffie_hellman_prime_3072, sizeof diffie_hellman_prime_3072}
	 , 0}
	,
	{4096, NULL, NULL, {DH_G_4096, sizeof(DH_G_4096)}
	 , {diffie_hellman_prime_4096, sizeof diffie_hellman_prime_4096}
	 , 0}
	,
	{0, NULL, NULL, {NULL, 0}
	 , {NULL, 0}
	 , 0}
};

/* This function takes a number of bits and returns a supported
 * number of bits. Ie a number of bits that we have a prime in the
 * dh_primes structure.
 */
static const int supported_bits[] = { 768, 1024, 2048, 3072, 4096, 0 };
static int normalize_bits(int bits)
{
	if (bits >= 4096)
		bits = 4096;
	else if (bits <= 768)
		bits = 768;
	else if (bits <= 1024)
		bits = 1024;
	else if (bits <= 2048)
		bits = 2048;
	else if (bits <= 3072)
		bits = 3072;
	else if (bits <= 4096)
		bits = 4096;

	return bits;
}

/* Clears allocated GNUTLS_MPIs and data. Only to be called at exit.
 */
void _gnutls_dh_clear_mpis(void)
{
	int i;

	if (_gnutls_dh_default_params == NULL)
		return;

	i = 0;
	do {
		_gnutls_mpi_release(&_gnutls_dh_default_params[i]._prime);
		_gnutls_mpi_release(&_gnutls_dh_default_params[i].
				    _generator);
		if (_gnutls_dh_default_params[i].local != 0) {
			gnutls_free(_gnutls_dh_default_params[i].prime.
				    data);
			gnutls_free(_gnutls_dh_default_params[i].generator.
				    data);
		}
		i++;
	} while (_gnutls_dh_default_params[i].bits != 0);

}

/* Generates GNUTLS_MPIs from opaque integer data. Initializes the dh_primes to
 * be used.
 */
int _gnutls_dh_calc_mpis(void)
{
	int i;
	size_t n;

	if (_gnutls_dh_default_params == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	i = 0;
	do {
		n = _gnutls_dh_default_params[i].prime.size;
		_gnutls_mpi_release(&_gnutls_dh_default_params[i]._prime);

		if (_gnutls_mpi_scan
		    (&_gnutls_dh_default_params[i]._prime,
		     _gnutls_dh_default_params[i].prime.data, &n)
		    || _gnutls_dh_default_params[i]._prime == NULL) {
			gnutls_assert();
			return GNUTLS_E_MPI_SCAN_FAILED;
		}


		n = _gnutls_dh_default_params[i].generator.size;
		_gnutls_mpi_release(&_gnutls_dh_default_params[i].
				    _generator);

		if (_gnutls_mpi_scan
		    (&_gnutls_dh_default_params[i]._generator,
		     _gnutls_dh_default_params[i].generator.data, &n)
		    || _gnutls_dh_default_params[i]._generator == NULL) {
			gnutls_assert();
			return GNUTLS_E_MPI_SCAN_FAILED;
		}

		i++;
	} while (_gnutls_dh_default_params[i].bits != 0);

	return 0;
}

/* returns g and p, depends on the requested bits.
 * We only support limited key sizes.
 */
GNUTLS_MPI gnutls_get_dh_params(gnutls_dh_params dh_primes,
				GNUTLS_MPI * ret_p, int bits)
{
	GNUTLS_MPI g = NULL, prime = NULL;
	int i;

	if (dh_primes == NULL) {
		gnutls_assert();
		return NULL;
	}

	bits = normalize_bits(bits);

	i = 0;
	do {
		if (dh_primes[i].bits == bits) {
			prime = _gnutls_mpi_copy(dh_primes[i]._prime);
			g = _gnutls_mpi_copy(dh_primes[i]._generator);
			break;
		}
		i++;
	} while (dh_primes[i].bits != 0);

	if (prime == NULL || g == NULL) {	/* if not prime was found */
		gnutls_assert();
		_gnutls_mpi_release(&g);
		_gnutls_mpi_release(&prime);
		*ret_p = NULL;
		return NULL;
	}

	if (ret_p)
		*ret_p = prime;
	return g;
}

/* These should be added in gcrypt.h */
GNUTLS_MPI _gcry_generate_elg_prime(int mode, unsigned pbits,
				    unsigned qbits, GNUTLS_MPI g,
				    GNUTLS_MPI ** ret_factors);

int _gnutls_dh_generate_prime(GNUTLS_MPI * ret_g, GNUTLS_MPI * ret_n,
			      int bits)
{

	GNUTLS_MPI g, prime;
	int qbits;

	g = mpi_new(16);	/* this should be ok */
	if (g == NULL) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	/* generate a random prime */
	/* this is an emulation of Michael Wiener's table
	 * bad emulation.
	 */
	qbits = 120 + (((bits / 256) - 1) * 20);
	if (qbits & 1)		/* better have a even one */
		qbits++;

	prime = _gcry_generate_elg_prime(0, bits, qbits, g, NULL);
	if (prime == NULL || g == NULL) {
		_gnutls_mpi_release(&g);
		_gnutls_mpi_release(&prime);
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	if (ret_g)
		*ret_g = g;
	if (ret_n)
		*ret_n = prime;

	return 0;

}

/* returns a negative value if the bits is not supported 
 */
static int check_bits(int bits)
{
	int i = 0;
	do {
		if (supported_bits[i] == bits)
			return 0;
		i++;
	} while (supported_bits[i] != 0);

	gnutls_assert();
	return GNUTLS_E_INVALID_REQUEST;
}

/* Replaces the prime in the static DH parameters, with a randomly
 * generated one.
 */
/**
  * gnutls_dh_params_set - This function will replace the old DH parameters
  * @dh_params: Is a structure will hold the prime numbers
  * @prime: holds the new prime
  * @generator: holds the new generator
  * @bits: is the prime's number of bits
  *
  * This function will replace the pair of prime and generator for use in 
  * the Diffie-Hellman key exchange. The new parameters should be stored in the
  * appropriate gnutls_datum. 
  * 
  * Note that the bits value should be one of 768, 1024, 2048, 3072 or 4096.
  *
  **/
int gnutls_dh_params_set(gnutls_dh_params dh_params, gnutls_datum prime,
			 gnutls_datum generator, int bits)
{
	GNUTLS_MPI tmp_prime, tmp_g;
	int i = 0;
	gnutls_dh_params sprime=NULL;
	size_t siz = 0;

	if (check_bits(bits) < 0) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	i = 0;
	do {
		if (dh_params[i].bits == bits) {
			sprime = &dh_params[i];
			break;
		}
	} while (dh_params[++i].bits != 0);
	/* sprime is not null, because of the check_bits()
	 * above.
	 */

	siz = prime.size;
	if (_gnutls_mpi_scan(&tmp_prime, prime.data, &siz)) {
		gnutls_assert();
		return GNUTLS_E_MPI_SCAN_FAILED;
	}

	siz = generator.size;
	if (_gnutls_mpi_scan(&tmp_g, generator.data, &siz)) {
		_gnutls_mpi_release(&tmp_prime);
		gnutls_assert();
		return GNUTLS_E_MPI_SCAN_FAILED;
	}

	/* copy the generated values to the structure
	 */
	if (sprime->local != 0) {
		gnutls_free(sprime->prime.data);
		_gnutls_mpi_release(&sprime->_prime);
		gnutls_free(sprime->generator.data);
		_gnutls_mpi_release(&sprime->_generator);
	}
	sprime->local = 1;
	sprime->_prime = tmp_prime;
	sprime->_generator = tmp_g;

	sprime->generator.data = NULL;
	sprime->prime.data = NULL;

	if (_gnutls_set_datum(&sprime->prime, prime.data, prime.size) < 0) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}
	if (_gnutls_set_datum
	    (&sprime->generator, generator.data, generator.size) < 0) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	return 0;

}

/**
  * gnutls_dh_params_init - This function will initialize the DH parameters
  * @dh_params: Is a structure that will hold the prime numbers
  *
  * This function will initialize the DH parameters structure.
  *
  **/
int gnutls_dh_params_init(gnutls_dh_params * dh_params)
{

	(*dh_params) = gnutls_calloc(1, sizeof(_gnutls_dh_copy_params));
	if (*dh_params == NULL) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	memcpy((*dh_params), _gnutls_dh_copy_params,
	       sizeof(_gnutls_dh_copy_params));

	return 0;

}

/**
  * gnutls_dh_params_deinit - This function will deinitialize the DH parameters
  * @dh_params: Is a structure that holds the prime numbers
  *
  * This function will deinitialize the DH parameters structure.
  *
  **/
void gnutls_dh_params_deinit(gnutls_dh_params dh_params)
{
	int i;
	if (dh_params == NULL)
		return;

	i = 0;
	do {
		_gnutls_mpi_release(&dh_params[i]._prime);
		_gnutls_mpi_release(&dh_params[i]._generator);
		if (dh_params[i].local != 0) {
			gnutls_free(dh_params[i].prime.data);
			gnutls_free(dh_params[i].generator.data);
		}
		i++;
	} while (dh_params[i].bits != 0);

	gnutls_free(dh_params);

}

/* Generates a prime number and a generator, and returns 2 gnutls_datums that contain these
 * numbers.
 */
/**
  * gnutls_dh_params_generate - This function will generate new DH parameters
  * @prime: will hold the new prime
  * @generator: will hold the new generator
  * @bits: is the prime's number of bits
  *
  * This function will generate a new pair of prime and generator for use in 
  * the Diffie-Hellman key exchange. The new parameters will be allocated using
  * malloc and will be stored in the appropriate datum.
  * This function is normally very slow. An other function
  * (gnutls_dh_params_set()) should be called in order to replace the 
  * included DH primes in the gnutls library.
  * 
  * Note that the bits value should be one of 768, 1024, 2048, 3072 or 4096.
  * Also note that the generation of new DH parameters is only usefull
  * to servers. Clients use the parameters sent by the server, thus it's
  * no use calling this in client side.
  *
  **/
int gnutls_dh_params_generate(gnutls_datum * prime,
			      gnutls_datum * generator, int bits)
{

	GNUTLS_MPI tmp_prime, tmp_g;
	size_t siz;

	if (check_bits(bits) < 0) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	if (_gnutls_dh_generate_prime(&tmp_g, &tmp_prime, bits) < 0) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	siz = 0;
	_gnutls_mpi_print(NULL, &siz, tmp_g);

	generator->data = malloc(siz);
	if (generator->data == NULL) {
		_gnutls_mpi_release(&tmp_g);
		_gnutls_mpi_release(&tmp_prime);
		return GNUTLS_E_MEMORY_ERROR;
	}

	generator->size = siz;
	_gnutls_mpi_print(generator->data, &siz, tmp_g);


	siz = 0;
	_gnutls_mpi_print(NULL, &siz, tmp_prime);

	prime->data = malloc(siz);
	if (prime->data == NULL) {
		gnutls_free(generator->data);
		_gnutls_mpi_release(&tmp_g);
		_gnutls_mpi_release(&tmp_prime);
		return GNUTLS_E_MEMORY_ERROR;
	}
	prime->size = siz;
	_gnutls_mpi_print(prime->data, &siz, tmp_prime);

	_gnutls_log("Generated %d bits prime %s, generator %s.\n",
		    bits, _gnutls_bin2hex(prime->data, prime->size),
		    _gnutls_bin2hex(generator->data, generator->size));

	return 0;

}
