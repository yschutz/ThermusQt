#update the mass of particle tables (from http://pdg.lbl.gov/2017/mcdata/mass_width_2017.mcd)
import os.path

def isBlank (myString):
    return not (myString and myString.strip())

def isNotBlank (myString):
    return bool(myString and myString.strip())

filename = 'particles_mass.txt'
if (os.path.isfile(filename)):
	f = open(filename, 'r')
else: 
	print (filename + "does not exist")
	quit()
# reading the file line by line 	
for line in f:
	if line.startswith("*") is False:
			pdg1 = int(line[0:8])
			if isNotBlank(line[9:17]):
				print "xxxxxx", line[17], "yyyyyy"
				pdg2 = int(line[9:17])
			else:
				pdg2 = 0
			if isNotBlank(line[19:25]):
				pdg3 = int(line[19:25])
			else:
				pdg3 = 0
			if isNotBlank(line[27:33]):
				pdg4 = int(line[27:33])
			else:
				pdg4 = 0
			mass = float(line[35:41])
			print pdg1, pdg2, pdg3, pdg4, mass
