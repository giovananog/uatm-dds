#!/bin/bash
if [ $# -eq 0 ]
  then
    echo "Expecing certificate name / prefix as argument"
fi

openssl genrsa -out participants/$1_private_key.pem 2048
openssl req -new -key participants/$1_private_key.pem -out participant/$1.csr
openssl ca -config identity_ca_openssl.cnf -days 3650 -in participant/$1.csr -out participant/$1_cert.pem