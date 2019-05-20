#!/bin/bash

filename="$1"

while read -r line; do
    command="$line"
    $command
done < "$filename"