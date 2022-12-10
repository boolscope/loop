#!/bin/bash

# Split the injection file into *.h and *.cpp type files.
# Ignores content of *.ino part-files.

outdir=""
outfile=""
isnewfile=true

# The first and required argument is the name of the file to be split.
# The second argument is the directory where the files will be created.
# If at least one argument is not specified, show an error.
if [ $# -ne 2 ]; then
    echo "Incorrect format."
    echo "Usage as: splitter.bash <ino-file> <out-dir> "
    echo "Example: splitter.bash example.ino ./"
    exit 1
fi

# Check that the ino-file exists.
if [ ! -f "$1" ]; then
    echo "File $1 does not exist."
    exit 1
fi

# Check that the output directory exists.
if [ ! -d "$2" ]; then
    echo "Directory $2 does not exist."
    exit 1
fi

# Show welcome message.
outdir="$2"
echo "Splitting $1 into *.h and *.cpp files."
echo "Output directory: $outdir"
echo "Wait please..."


# Read the file line by line.
while IFS= read -r line; do
    # Check if the line contains out file name.
    if [[ $line == *"@file"* ]]; then
        # Parse the line, get the file name,
        # and trim whitespaces from this one.
        outfile=$(echo $line | awk -F '@file' '{print $2}' | xargs)

        # When changing the file name, put a marker to clear this file.
        isnewfile=true

        # We don't write a comment with the file name in the file itself.
        continue
    fi

    # Ignore *.ino files.
    if [[ $outfile != *.h && $outfile != *.cpp ]]; then
        outfile=""
    fi

    # Continue if the name of outfile is empty.
    if [ -z "$outfile" ]; then
        continue
    fi

    # Get the full path to the file.
    target=$(readlink -m "$outdir/$outfile")

    # Clean the file if the file name has been changed.
    if [ "$isnewfile" = true ]; then
        truncate -s 0 "$target"
        isnewfile=false
    fi

    if [[ $line == *"@raw"* ]]; then
        line=$(echo $line | awk -F '@raw' '{print $2}' | sed 's/^[ \t]*//;s/[ \t]*$//')
    fi

    # Append the line to the file.
    echo "$line" >> "$target"
done < "$1"
echo "Done!"