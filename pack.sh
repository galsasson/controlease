#!/bin/sh

TMPDIR=`mktemp -d /tmp/contpack_XXXXXXX`
TARGET=`pwd`/Controlease.tar.gz

PAYLOADDIR=$TMPDIR/Controlease

# tests
if [ -f $TARGET ] ; then
	echo "$TARGET already exists."
	exit 1
fi

mkdir -p $PAYLOADDIR
cp -a xcode/build/Release/Controlease.app $PAYLOADDIR/
cp -a assets $PAYLOADDIR/
pushd $TMPDIR
tar -czf $TARGET Controlease
popd
rm -rf $TMPDIR
echo "done: $TARGET"
