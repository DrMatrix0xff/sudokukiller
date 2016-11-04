#!/bin/bash

# --install $(target) $(name_in_etc_alternative_directory) $(actual_bin_path) $(priority) --slave $(target) $(name_in_etc_alternative_directory) $(actual_man_path)

#update-alternatives --install /usr/bin/java java /usr/local/jdk/bin/java 100 --slave /usr/share/man/man1/java.1 java.1 /usr/local/jdk/man/man1/java.1

JVMDIR=/usr/local/jdk
INSTDIR=/usr/bin
MANDIR=/usr/share/man/man1

CMD="update-alternatives --install ${INSTDIR}/java java ${JVMDIR}/bin/java 100 --slave ${MANDIR}/java.1 java.1 ${JVMDIR}/man/man1/java.1"

for x in javac\
	javap\
	javadoc\
	jjs\
	jar\
	orbd\
	rmic\
	rmid\
	keytool\
	policytool\
	servertool\
	tnameserv\
	; do
    SLAVE=" --slave ${INSTDIR}/${x} ${x} ${JVMDIR}/bin/${x} --slave ${MANDIR}/${x}.1 ${x}.1 ${JVMDIR}/man/man1/${x}.1"
    CMD="${CMD}${SLAVE}"
done;

echo ${CMD}
eval ${CMD}

