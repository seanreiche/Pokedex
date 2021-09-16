import xml.dom.minidom
import sys
import os

OutFile = open("pokedex.txt", "w+")

path = "/home/SReiche/Pokedex/pokedex/"

document = xml.dom.minidom.parse(path + "pokedex.xhtml")
	
body = document.getElementsByTagName('body')[0]

tbody = body.getElementsByTagName('tbody')[0]

for tr in tbody.getElementsByTagName('tr'):
	
	for element in tr.getElementsByTagName('span'):
		if (element.hasAttribute('class')):
			a = element.getAttributeNode('class')
			b = a.nodeValue
			if ("infocard-cell-data" in b):
				num = element.childNodes[0].nodeValue
					
	for element in tr.getElementsByTagName('a'):
		if (element.hasAttribute('class')):
			a = element.getAttributeNode('class')
			b = a.nodeValue
			if ("ent-name" in b):
				name = element.childNodes[0].nodeValue
	
	sub_name = ""
	
	for element in tr.getElementsByTagName('small'):
		if (element.hasAttribute('class')):
			a = element.getAttributeNode('class')
			b = a.nodeValue
			if ("text-muted" in b):
				sub_name = " (" + element.childNodes[0].nodeValue + ")"

	types = []

	for element in tr.getElementsByTagName('a'):
		if (element.hasAttribute('class')):
			a = element.getAttributeNode('class')
			b = a.nodeValue
			if ("type-icon" in b):
				types.append(element.childNodes[0].nodeValue)
					
	OutFile.write(num + "/" + name + sub_name + "/")

	print(num, "-", name + sub_name, "- ", end="")
	for t in types:
		OutFile.write(t + "/")
		print(t, end=" ")
	OutFile.write("\n")
	print()

