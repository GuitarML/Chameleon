#!/bin/bash

echo "Create the Chameleon tar.gz file for deployment"
tar -czvf Chameleon.tar.gz Chameleon.vst3

if [ -f "Chameleon.tar.gz" ]; then
  echo "Add the tar.gz as a binary payload to the install script to create installer file"
  if ! ./add_binary_payload.sh --binary "Chameleon.tar.gz"; then
    exit 1
  fi
  echo "Completed build process."
else
  echo "Chameleon.tar.gz does not exist."
  exit 1
fi
