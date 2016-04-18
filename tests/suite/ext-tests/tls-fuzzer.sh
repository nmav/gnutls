#!/bin/sh

# Copyright (C) 2016 Red Hat, Inc.
#
# This file is part of GnuTLS.
#
# GnuTLS is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# GnuTLS is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GnuTLS; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

srcdir="${srcdir:-.}"
SERV="../../../src/gnutls-serv${EXEEXT} -q"
PORT=4433

ECC256_CERT="${srcdir}/../../certs/cert-ecc256.pem"
ECC256_KEY="${srcdir}/../../certs/ecc256.pem"

RSA_CERT="${srcdir}/../../certs/cert-rsa-2432.pem"
RSA_KEY="${srcdir}/../../certs/rsa-2432.pem"

OUTFILE=tls-fuzzer.debug

declare -a known_failures=("tlsfuzzer/scripts/test-dhe-rsa-key-exchange-signatures.py"
"tlsfuzzer/scripts/test-empty-extensions.py" "tlsfuzzer/scripts/test-interleaved-application-data-and-fragmented-handshakes-in-renegotiation.py"
"tlsfuzzer/scripts/test-interleaved-application-data-in-renegotiation.py"
"tlsfuzzer/scripts/test-invalid-cipher-suites.py" "checking tlsfuzzer/scripts/test-openssl-3712.py"
"tlsfuzzer/scripts/test-record-layer-fragmentation.py" "tlsfuzzer/scripts/test-rsa-sigs-on-certificate-verify.py"
"tlsfuzzer/scripts/test-SSLv3-padding.py" "tlsfuzzer/scripts/test-truncating-of-client-hello.py"
"tlsfuzzer/scripts/test-certificate-verify.py" "tlsfuzzer/scripts/test-openssl-3712.py")

. "${srcdir}/../scripts/common.sh"

pushd ext-tests

if ! test -d tlsfuzzer;then
	exit 77
fi

rm -f "$OUTFILE"

pushd tlsfuzzer
ln -s ../python-ecdsa/ecdsa ecdsa 2>/dev/null
ln -s ../tlslite-ng/tlslite tlslite 2>/dev/null

popd

launch_server $$ --http --x509keyfile "${RSA_KEY}" --x509certfile "${RSA_CERT}" --x509ecckeyfile "${ECC256_KEY}" --x509ecccertfile "${ECC256_CERT}" --disable-client-cert &
PID=$!
wait_server ${PID}

test -z "$PID" && exit 2

retval=0

for i in tlsfuzzer/scripts/*;do
	echo -n "checking $i... "
	PYTHONPATH=tlsfuzzer python "$i" >>$OUTFILE 2>&1
	ret=$?
	if test $ret != 0;then
		found=0
		for j in "${known_failures[@]}";do
			if test "$i" = "$j";then
				found=1
				break
			fi
		done
		if test $found = 0;then
			echo fail
			retval=1
		else
			echo "fail (but expected)"
		fi
	else
		echo ok
	fi
done

kill ${PID}
wait

popd

exit $retval
