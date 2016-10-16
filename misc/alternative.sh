
# --install $(target) $(name_in_etc_alternative_directory) $(actual_bin_path) --slave $(target) $(name_in_etc_alternative_directory) $(actual_man_path)

update-alternatives --install /usr/bin/java java /usr/lib/jvm/jdk-1.8/bin/java --slave /usr/share/man/man1/java.1 java.1 /usr/lib/jvm/jdk-1.8/man/man1/java.1
update-alternatives --install /usr/bin/javac javac /usr/lib/jvm/jdk-1.8/bin/javac --slave /usr/share/man/man1/javac.1 javac.1 /usr/lib/jvm/jdk-1.8/man/man1/javac.1
update-alternatives --install /usr/bin/javap javap /usr/lib/jvm/jdk-1.8/bin/javap --slave /usr/share/man/man1/javap.1 javap.1 /usr/lib/jvm/jdk-1.8/man/man1/javap.1
