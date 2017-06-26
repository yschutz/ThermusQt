# pip install peewee
# get sqlite bowswer

from peewee import *
import os
import glob

# Naming
PARTICLES_LIST = 'PartList_PPB2014_CBHN_fixed_saveQM14.txt'
DECAY_SUFFIX   = '_decay.txt'
DB_FILE_NAME   = 'Thermus.db'

# Create DB
if os.path.exists(DB_FILE_NAME):
    os.remove(DB_FILE_NAME)
db = SqliteDatabase(DB_FILE_NAME)

# Models definition
class Particles(Model):
	name = CharField(verbose_name="Particles name")
	mcid = IntegerField()
	deg  = IntegerField()
	stat = IntegerField()
	mass = DoubleField()
	S    = IntegerField()
	B    = IntegerField()
	Q    = IntegerField()
	charm  = IntegerField(default = 0)
	beauty = IntegerField(default = 0)
	top    = IntegerField(default = 0)
	Scontent = DoubleField(default = 0.0)
	Ccontent = DoubleField(default = 0.0)
	Bcontent = DoubleField(default = 0.0)
	Tcontent = DoubleField(default = 0.0)
	width    = DoubleField(default = 0.0)
	threshold = DoubleField(default = 0.0)
	radius    = DoubleField(default = 0.0)
	
	def anti_particule(self):
		if self.B or self.S or self.Q or self.charm or self.beauty or self.top:
			antip      = Particles()
			antip.name   = "anti-"+self.name
			antip.mcid   = -self.mcid
			antip.deg    = self.deg
			antip.stat    = self.stat
			antip.mass    = self.mass
			antip.S      = -self.S
			antip.B      = -self.B
			antip.Q      = -self.Q
 			antip.charm  = -self.charm
			antip.beauty = -self.beauty
			antip.top    = -self.top
			return antip
		else:
			return None

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

# functions
def particle_from_name(name):
	try:
		particle = Particles.get(Particles.name == name)
	except:	
		particle = None
	return particle

def particle_from_mcid(mcid):
	try:
		particle = Particles.get(Particles.mcid == mcid)
	except:	
		particle = None
	return particle

# Open DB
db.connect()

# Create DB tables schema
db.create_tables([Particles, DecayChannels, Daughters])

# Populate particles table reading from a file 
# First all particles

with open(PARTICLES_LIST) as file:
	for line in file:
		line = line.rstrip()
		fields = line.split("\t")
		p = Particles()
		p.name      = fields[1]
		p.mcid      = int(fields[2])
		p.deg       = int(fields[3])
		p.stat      = int(fields[4])
		p.mass      = float(fields[5])
		p.S         = int(fields[6])
		p.B         = int(fields[7])
		p.Q         = int(fields[8])
		p.charm     = int(fields[9])
		p.beauty    = int(fields[10])
		p.top       = 0
		if len(fields) > 11:
			p.Scontent  = float(fields[11])
			p.Bcontent  = 0
			p.Ccontent  = 0
			p.Tcontent  = 0
		if len(fields) > 12:
			p.width = float(fields[12])
		if len(fields) > 13:
			p.threshold = 0
		p.radius    = 0.0		
		p.save()

		# save anti particles if avaible 
		antip = p.anti_particule()
		if antip is not None:
			antip.save()
	
for particle in Particles.select():
	filename = particle.name + DECAY_SUFFIX
	if os.path.isfile(filename):
		with open(filename) as file:
			for line in file:
				line = line.rstrip()
		 		fields = line.split("\t")
		 		# save decay channel
		 		decay_channel = DecayChannels(parent = particle, branching = fields[0])
		 		decay_channel.save()
		 		# save daughters
		 		for mcid in fields[1:]:
			 		daughter_particle = particle_from_mcid(mcid)
					print(filename, mcid, daughter_particle)
			 		daughter = Daughters(decay = decay_channel, particle = daughter_particle, count = 1)
			 		daughter.save()
        
# # Second load the decays
# for file in glob.glob(DECAY_SUFFIX):
# 	name = file.replace(DECAY_SUFFIX, "")
# 	particle = particle_from_name(name)
# 	if particle is not None:

# #		d = DecayChannels(parent = p, branching = 43)
# #		d.save()
