#!/bin/bash

BINDIR=/usr/bin
JVMDIR=/usr/lib/jvm/java-1.8.0-sun-1.8.0.45-7.b14.fc24.x86_64

CMD="update-alternatives --install ${BINDIR}/javac javac ${JVMDIR}/bin/javac 100"

for x in javap \
	javah \
	javadoc \
	jar \
; do
	SLAVE=" --slave ${BINDIR}/${x} ${x} ${JVMDIR}/bin/${x}"
	CMD="${CMD}${SLAVE}"
done

echo ${CMD}
eval ${CMD}
