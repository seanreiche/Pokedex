#!/bin/sh

# Tagsoup download link: http://maven.org/maven2/org/ccil/cowan/tagsoup/tagsoup/1.2.1/tagsoup-1.2.1.jar

cp tagsoup-1.2.1.jar pokedex/

cd pokedex/

wget -nd --accept html -E html -O pokedex.html https://pokemondb.net/pokedex/all

java -jar tagsoup-1.2.1.jar --files pokedex.html

rm tagsoup-1.2.1.jar

cd ..

python3 parsermon_pokemonDB.py

# remove the html and xhtml files from the subfolder

