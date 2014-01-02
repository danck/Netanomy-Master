#!/bin/bash

find ./src | xargs ./cpplint.py --verbose=3 2>&1 | egrep -v 'whitespace/braces|readability/streams'
