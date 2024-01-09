#!/bin/bash

# This script is used to add an outer namespace to google::protobuf:: to avoid symbol
# collisions when using multiple versions of protobuf concurrently.
#
# When upgrading protobuf, revert the commit that adds the namespace, perform the
# upgrade (e.g. by merging the new version into the current version), then run
# this script to re-apply the namespace.
#
# Note: this script only runs on OSX.

if [ "$#" -ne 1 ]; then
    echo "Usage: ./add_namespace_to_protobuf.sh namespaceName"
    exit 1
fi

SCRIPT_DIR=$( dirname -- "$( readlink -f -- "$0"; )"; )
echo "Switching to script directory $SCRIPT_DIR..."
cd $SCRIPT_DIR

echo "Applying namespace $1..."

FILES=$(find . -name "*.h" -or -name "*.c" -or -name "*.cc" -or -name "*.inc" -or -name "*.rb")

echo "$FILES" | while read FILE; do

    echo "Processing: $FILE"

    # ::google::protobuf::
    sed -i '' "s/::google::protobuf::/::$1::google::protobuf::/" "$FILE"
    # using google::protobuf::
    sed -i '' "s/using google::protobuf::/using $1::google::protobuf::/" "$FILE"

    # namespace google {
    sed -i '' "s/^namespace google {$/namespace $1 {\\nnamespace google {/" "$FILE"
    # } // namespace google
    sed -i '' "s/}  .. namespace google/} \/\/ namespace google\\n} \/\/ namespace $1/" "$FILE"

    # For port_def.inc defines
    sed -i '' "s/#define PROTOBUF_NAMESPACE_OPEN ./#define PROTOBUF_NAMESPACE_OPEN \\\\\n  namespace $1 { \\\/" "$FILE"
    sed -i '' "s/} .. namespace google ../} \/* namespace google *\/ \\\\\n  } \/* namespace $1 *\//" "$FILE"
    sed -i '' "s/#define PROTOBUF_NAMESPACE \"google::protobuf\"/#define PROTOBUF_NAMESPACE \"$1::google::protobuf\"/" "$FILE"
    sed -i '' "s/#define PROTOBUF_NAMESPACE_ID google::protobuf/#define PROTOBUF_NAMESPACE_ID $1::google::protobuf/" "$FILE"

    # For hash.h defines
    sed -i '' "s/# define GOOGLE_PROTOBUF_HASH_NAMESPACE_DECLARATION_START ./# define GOOGLE_PROTOBUF_HASH_NAMESPACE_DECLARATION_START \\\\\n  namespace $1 { \\\/" "$FILE"
    sed -i '' "s/# define GOOGLE_PROTOBUF_HASH_NAMESPACE_DECLARATION_END }}/# define GOOGLE_PROTOBUF_HASH_NAMESPACE_DECLARATION_END }}}/" "$FILE"

done

echo "All done."
