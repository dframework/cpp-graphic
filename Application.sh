#!/bin/sh

D_PWD=`pwd`

if [ "$DFRAMEWORK_CPP_HOME" = "" ]; then
    DFRAMEWORK_CPP_HOME="/opt/dframework"
fi

DFRAMEWORK_CPP_COMMON="${DFRAMEWORK_CPP_HOME}/cpp-common"
DFRAMEWORK_CPP_GRAPHIC="${DFRAMEWORK_CPP_HOME}/cpp-graphic"


#############################################################


if test ! -d $DFRAMEWORK_CPP_HOME ; then
    echo "Not found $DFRAMEWORK_CPP_HOME directory."
    echo "See your environment variables DFRAMEWORK_CPP_HOME."
    exit 1
fi

if test ! -d $DFRAMEWORK_CPP_COMMON ; then
    echo "Not found $DFRAMEWORK_CPP_COMMON directory."
    echo "See your environment variables DFRAMEWORK_CPP_HOME."
    echo "and cpp-common folder in DFRAMEWORK_CPP_HOME."
    exit 1
fi

if test ! -d $DFRAMEWORK_CPP_GRAPHIC ; then
    echo "Not found $DFRAMEWORK_CPP_GRAPHIC directory."
    echo "See your environment variables DFRAMEWORK_CPP_HOME."
    echo "and cpp-graphic folder in DFRAMEWORK_CPP_HOME."
    exit 1
fi

