#!/usr/local/bin/python
#update the mass of particle tables (from http://pdg.lbl.gov/2017/mcdata/mass_width_2017.mcd)
import os.path
import array
from peewee import *

HBAR = 6.5821192815e-25 # GeV s

def isBlank (myString):
    return not (myString and myString.strip())

def isNotBlank (myString):
    return bool(myString and myString.strip())


# data base stuff
dbname = "Particles.db"
db = SqliteDatabase(dbname)
if db is False:
	print "database " + dbname + " does not exist"
	quit(1)

class BaseModel(Model):
    class Meta:
        database = db

class Particle(BaseModel):
    name     = CharField(unique = True)
    pdg      = IntegerField()
    matter   = BooleanField()
    pcode    = IntegerField()
    pclass   = CharField()
    charge   = FloatField()
    mass     = DoubleField()
    width    = DoubleField()
    lifetime = DoubleField()
    isospin  = IntegerField()
    iso3     = IntegerField()
    strange  = IntegerField()
    flavor   = IntegerField()
    track    = IntegerField()
    ndecay   = IntegerField()

class Decay(BaseModel): 
    mother     = ForeignKeyField(Particle, related_name = 'decays')
    dtype      = IntegerField()
    br         = DoubleField()
    ndaughters = IntegerField()

class Daughter(BaseModel): 
    decay = ForeignKeyField(Decay, related_name = 'daughters')
    pdg   = IntegerField()

db.connect()

filename = 'particles_mass.txt'
if (os.path.isfile(filename)):
	f = open(filename, 'r')
else: 
	print (filename + "does not exist")
	quit()
# reading the file line by line 	
for line in f:
	if line.startswith("*") is False:
		pdg = array.array('i',(0 for i in range(0,4)))
		pdg[0]  = int(line[1:8].strip())
		tempo   = line[9:16].strip()
		pdg[1]  = int(tempo) if len(tempo) > 0 else 0
		tempo   = line[17:24].strip()
		pdg[2]  = int(tempo) if len(tempo) > 0 else 0
		tempo   = line[25:32].strip()
		pdg[3]  = int(tempo) if len(tempo) > 0 else 0
		mass    = float(line[33:52].strip())
		tempo   = line[71:89].strip()
		width   = float(tempo) if len(tempo) > 0 else 0.
		tempo   = line[107:]
		index   = tempo.index(" ")
		gname   = tempo[:index]
		scharges = tempo[index:].strip() 
		charges = scharges.split(",")
		name = ["" for x in range(4)]	
		charge = array.array('f',(99 for i in range(0,4)))
		for index in range(0, len(charges)):
			if charges[index] == "+":
				name[index]   = gname + charges[index]
				charge[index] = 1
			elif charges[index] == "-":
				charge[index] = -1
				name[index]   = gname + charges[index]
			elif charges[index] == "++":
				charge[index] = 2
				name[index]   = gname + charges[index]
			elif charges[index] == "--":
				charge[index] = -2
				name[index]   = gname + charges[index]
			elif charges[index] == "0":
				charge[index] = 0
				name[index]   = gname + charges[index]
			elif charges[index] == "-1/3":
				charge[index] = -1.0 /3.0
			elif charges[index] == "+2/3":
				charge[index] = -2.0 /3.0
			else:
				print "error: cannot decode %s" % charges[index]
		for index in range(0,4):
			if pdg[index] != 0:
				part = Particle.select()
				if part.where(Particle.pdg == pdg[index]).exists():
					part = Particle.select().where(Particle.pdg == pdg[index]).get()
					part.mass     = mass
					part.width    = width
					part.lifetime = HBAR / width if width > 0 else 0.0
					part.save()
  					print "Mass of %s (%i) has been update to %f" %(part.name, part.pdg, mass)
				else:
					print "Particle %i %s added to database" % (pdg[index], name[index])
					lifetime = HBAR / width if width > 0 else 0.0
				 	Particle.create(name = name[index], \
						pdg      = pdg[index], 
					 	matter   = True, 
				 		pcode    = 100,
				 		pclass   = "unknown", 
			 			charge   = charge[index], 
  		  				mass     = mass, 
  		  				width    = width, 
  		  				lifetime = lifetime, 
  		 				isospin  = -100, 
  		 				iso3     = 0, 
  		  				strange  = -100, 
  		  				flavor   = -1, 
		   				track    = -1, 
  		  				ndecay   = 0)
				part = Particle.select()
				if part.where(Particle.pdg == -pdg[index]).exists():
					part = Particle.select().where(Particle.pdg == -pdg[index]).get()
					part.mass = mass
					part.save()
f.close()
db.close()
