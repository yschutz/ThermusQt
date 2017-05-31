# pip install peewee
# get sqlite bowswer

from peewee import *
import os
import glob

filename = 'Thermus.db'
if os.path.exists(filename):
    os.remove(filename)
db = SqliteDatabase(filename)

class Particles(Model):
	name = CharField()
	mcid = IntegerField()
	class Meta:
		database = db # This model uses the "people.db" database.

class DecayChannels(Model):
	parent    = ForeignKeyField(Particles)
	branching = DoubleField()
	class Meta:
		database = db # This model uses the "people.db" database.

class Daughters(Model):
	decay       = ForeignKeyField(DecayChannels)
	particle    = ForeignKeyField(Particles)
	count       = DoubleField()
	class Meta:
		database = db # This model uses the "people.db" database.





db.connect()
db.create_tables([Particles, DecayChannels, Daughters])



with open("PartList_PPB2014_CBHN_fixed_saveQM14.txt") as file:
	for line in file:
		line = line.rstrip()
		fields = line.split("\t")
		name = fields[1]
		idmc = fields[2]
		p = Particles(name=name, mcid=idmc)
		p.save()
        

for file in glob.glob('*_decay.txt'):
	name = file.replace("_decay.txt", "")
	try:
		particle = Particles.get(Particles.name == name)
	except:	
		print(name, "stable")	
	print(particle.mcid)
#		d = DecayChannels(parent = p, branching = 43)
#		d.save()
