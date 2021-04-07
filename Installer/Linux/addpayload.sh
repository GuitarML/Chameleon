#!/bin/bash

# Check for payload format option (default is uuencode).
uuencode=1
if [[ "$1" == '--binary' ]]; then
	binary=1
	uuencode=0
	shift
fi
if [[ "$1" == '--uuencode' ]]; then
	binary=0
	uuencode=1
	shift
fi

if [[ ! "$1" ]]; then
	echo "Usage: $0 [--binary | --uuencode] PAYLOAD_FILE"
	exit 1
fi


mkdir -p Builds
in_file="install.sh.in"
installer_name="Builds/Chameleon-Install-VST3-Linux.sh"

if [[ $binary -ne 0 ]]; then
	# Append binary data.
	sed \
		-e 's/uuencode=./uuencode=0/' \
		-e 's/binary=./binary=1/' \
			 $in_file >$installer_name
	#echo "" >> $installer_name
	echo "PAYLOAD:" >> $installer_name

	cat $1 >>$installer_name
fi
if [[ $uuencode -ne 0 ]]; then
	# Append uuencoded data.
	sed \
		-e 's/uuencode=./uuencode=1/' \
		-e 's/binary=./binary=0/' \
			 $in_file >$installer_name
	echo "" >> $installer_name
	echo "PAYLOAD:" >> $installer_name

	cat $1 | uuencode - >>$installer_name
fi
cd ../..
echo ""
echo "Package generated: "`pwd`"/"$installer_name