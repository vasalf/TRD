#!/usr/bin/env bash

clang-format -i $@
sed -i '/^$/d' $@
cat $@
