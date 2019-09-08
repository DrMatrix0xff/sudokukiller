#!/bin/bash

BINDIR=/usr/bin
JVMDIR=/usr/lib/jvm/java-1.8.0-sun-1.8.0.45-7.b14.fc24.x86_64/jre

CMD="update-alternatives --install ${BINDIR}/java java ${JVMDIR}/bin/java 100 --slave /usr/lib/jvm/jre jre ${JVMDIR}"

for x in jjs \
	keytool \
	orbd \
	pack200 \
	rmid \
	rmiregistry \
	servertool \
	tnameserv \
	policytool \
	unpack200 \
; do
	SLAVE=" --slave ${BINDIR}/${x} ${x} ${JVMDIR}/bin/${x}"
	CMD="${CMD}${SLAVE}"
done

echo ${CMD}
eval ${CMD}
