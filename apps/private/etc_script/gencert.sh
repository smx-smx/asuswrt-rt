#!/bin/sh
SECS=1262278080

cd /etc

#NVCN=`nvram get https_crt_cn`
#NVCN=`tcapi get AiCloud_Entry https_crt_cn`
#if [ "$NVCN" == "" ]; then
	#NVCN=`nvram get lan_ipaddr`
	NVCN=`tcapi get Lan_Entry0 IP`
#fi

cp openssl.cnf openssl.config

I=0
for CN in $NVCN; do
        echo "$I.commonName=CN" >> openssl.config
        echo "$I.commonName_value=$CN" >> openssl.config
        I=`expr $I + 1`
done

# create the key and certificate request
OPENSSL_CONF=/etc/openssl.config openssl req -new -out /tmp/cert.csr -keyout /tmp/privkey.pem -newkey rsa:1024 -passout pass:password
# remove the passphrase from the key
OPENSSL_CONF=/etc/openssl.cnf openssl rsa -in /tmp/privkey.pem -out /etc/key.pem -passin pass:password
# convert the certificate request into a signed certificate
OPENSSL_CONF=/etc/openssl.cnf RANDFILE=/dev/urandom openssl x509 -in /tmp/cert.csr -out /etc/cert.pem -req -signkey /etc/key.pem -days 3653

#	openssl x509 -in /etc/cert.pem -text -noout

# server.pem for WebDav SSL
chmod 777 /etc/key.pem /etc/cert.pem
cat /etc/key.pem /etc/cert.pem > /etc/server.pem

rm -f /tmp/cert.csr /tmp/privkey.pem openssl.config
