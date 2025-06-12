#!/bin/bash
if (( $# != 2 ))
then
echo "Usage: $0 indir outdir"
echo "
The program assumes directories do not contain '.'"
echo "
and files contain one"
exit 1 # Signal error to the caller
fi
for f in $( find $1 -type f )
do
folder=$2/$( echo $f | cut -d . -f 2 )
mkdir -p $folder
cp $f $folder
done