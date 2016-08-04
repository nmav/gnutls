/*
 * GnuTLS PKCS#11 support
 * Copyright (C) 2010-2016 Free Software Foundation, Inc.
 * Copyright (C) 2015-2016 Red Hat, Inc.
 * 
 * Authors: Nikos Mavrogiannopoulos, Stef Walter
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/* Make sure that we include pkcs11.h prior to including gnutls/pkcs11.h
 * to enable the definition of gnutls_pkcs11_privkey_get_handles
 */
#define CRYPTOKI_GNU
#include <p11-kit/pkcs11.h>

#ifndef CK_FUNCTION_LIST_PTR
# define CK_FUNCTION_LIST_PTR struct ck_function_list*
#endif

#ifndef CK_SESSION_HANDLE
# define CK_SESSION_HANDLE ck_session_handle_t
#endif

#ifndef CK_OBJECT_HANDLE
# define CK_OBJECT_HANDLE ck_object_handle_t
#endif

#include "gnutls_int.h"
#include "errors.h"
#include <pkcs11_int.h>

/**
 * gnutls_pkcs11_privkey_get_handles:
 * @pkey: Holds the key
 * @pfunclist: will contain a pointer to the CK_FUNCTION_LIST
 * @session: will contain the CK_SESSION_HANDLE of the key
 * @obj: will contain the CK_OBJECT_HANDLE of the key
 *
 * Obtains the PKCS#11 internal handles of a pkcs11 private key.
 *
 * Note that since this function returns PKCS#11 types, it requires
 * pkcs11.h to be included by an application using it. To reduce
 * API clutter gnutls/pkcs11.h does not include it by default. For that,
 * you will need to include p11-kit/pkcs11.h prior to including
 * gnutls/pkcs11.h if you intend to use this function.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 * 
 * Since: 3.5.3
 *
 **/
int gnutls_pkcs11_privkey_get_handles(gnutls_pkcs11_privkey_t pkey, CK_FUNCTION_LIST_PTR *pfunclist,
				      CK_SESSION_HANDLE *session, CK_OBJECT_HANDLE *obj)
{
	int ret;

	ret = gnutls_pkcs11_privkey_status(pkey);
	if (ret < 0)
		return ret;

	*pfunclist = pkey->sinfo.module;
	*obj = pkey->ref;
	*session = pkey->sinfo.pks;

	return 0;
}

