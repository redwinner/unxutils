#!/bin/sh

usage="Usage: $0 [OPTION]...
Exit successfully.

   --help      display this help and exit
   --version   output version information and exit

Report bugs to bug-gnu-utils@gnu.ai.mit.edu"

case $# in
  1 )
    case "z${1}" in
      z--help )
	 echo "$usage"; exit 0 ;;
      z--version )
	 echo "true (@GNU_PACKAGE@) @VERSION@"; exit 0 ;;
      * ) ;;
    esac
    ;;
  * ) ;;
esac

exit 0
