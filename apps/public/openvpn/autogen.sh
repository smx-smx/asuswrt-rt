#!/bin/sh
libtoolize --force
autoreconf -if
[ ! -f .gitignore ]; touch .gitignore
[ ! -f .gitattributes ]; touch .gitattributes
