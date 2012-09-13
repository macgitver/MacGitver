#/bin/bash

# USE: $0 . ..
# -> takes sources from . and puts docs into ../html

cp Doxyfile.in Doxyfile
sed -i Doxyfile -e "s#%BASE%#$1/#g" -e "s#%OUTPUT%#$2/#"
