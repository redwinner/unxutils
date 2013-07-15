rem cl -Ox -DWIN32 -MD -D_I386 -DSTDC_HEADERS -DHAVE_CONFIG_H -I. -I. -I.. -I.. -I. -c %1
cl -Ox -MD -I\projects\cpp\downhill\include -D_I386 -DSTDC_HEADERS -DHAVE_CONFIG_H -I. -I. -I.. -I.. -I. -I../intl -c %1
rem gcc -O2 -D_I386 -DSTDC_HEADERS -DHAVE_CONFIG_H -I. -I. -I.. -I.. -I. -I../intl -c %1
