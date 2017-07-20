# decoding the pdg particle tables and filling an sql wordsbase 
import os.path
# installation of peewee required: pip install peewee
from peewee import *
#=======================================================================
HBAR = 6.5821192815e-25 # GeV s

I_COUNT   = 0   # an absolute counter 
I_NAME    = 1   # name of the particle 
I_PDG     = 2   # PDG code of the particle 
I_MATTER  = 3   # matter or antimatter
I_PCODE   = 4   # ??
I_PCLASS  = 5   # category lepton, meson, baryon, gauge, .... 
I_CHARGE  = 6   # electric charge of the particle
I_MASS    = 7   # mass of the particle in MeV
I_WIDTH   = 8   # width in MeV, lifetime = hbar/width
I_ISOSPIN = 9   # isospin 2*I + 1
I_ISO3    = 10  # isospin projection I3
I_STRANGE = 11  # strangeness 2*S + 1
I_FLAVOR  = 12  # ?
I_TRACK   = 13  # ?
I_NDECAY  = 14  # number of decay channels 
I_IPART   =  3  # PCODE of anti particle 
I_ACODE   =  4  # antiparticle code 

I_DCOUNT    = 0 # a counter
I_DTYPE     = 1 # decay type (Pythia 6)
I_BR        = 2 # branching ratio
I_NDEC      = 3 # number of daughters
I_DAUGHTER1 = 4 # first daughter
#=======================================================================
def getPart(line): 
	count   = 0 
	name    = "" 
	pdg     = 0 
	matter  = False 
	pcode   = 0 
	pclass  = "" 
	charge  = 0 
	mass    = 0 
	width   = 0 
	isospin = 0 
	iso3    = 0 
	strange = 0 
	flavor  = 0 
	track   = 0 
	ndecay  = 0 
	ipart   = 0  
	acode   = 0 
	words = line.split()
	if len(words) == 0:
		quit(1)
	count = int(words[I_COUNT])
	name  = words[I_NAME]
	pdg   = int(words[I_PDG])
	if pdg < 0:
		matter = False
		ipart  = int(words[I_IPART])
		acode  = int(words[I_ACODE])
	else :
		jmatter = words[I_MATTER]
		if jmatter == 1:
			matter = 1
		else:
			matter = 0
		pcode   = int(words[I_PCODE])
		pclass  = words[I_PCLASS]
		charge  = int(words[I_CHARGE])
		mass    = float(words[I_MASS])
		width   = float(words[I_WIDTH])
		isospin = int(words[I_ISOSPIN])
		iso3    = int(words[I_ISO3])
		strange = int(words[I_STRANGE])
		flavor  = int(words[I_FLAVOR])
		track   = int(words[I_TRACK])
		ndecay  = int(words[I_NDECAY])
	rv = [count, name, pdg, matter, pcode, pclass, charge, mass, width, isospin, iso3, strange, flavor, track, ndecay, ipart, acode]
	return rv
#=======================================================================
def getDecay(line):
	words = line.split()
	dcount = int(words[I_DCOUNT])
	dtype  = int(words[I_DTYPE])
	br     = float(words[I_BR])
	ndec   = int(words[I_NDEC])
	daughter = list()
	for i in range(0, ndec): 
		daughter.append(int(words[I_DAUGHTER1 + i]))
	decay = [dtype, br, daughter]
	return decay 
#=======================================================================
def normDecay(decays):
	jbr = 1 
	ndec = len(decays)
	brt = 0.0
	if ndec : 
		for idec in range(0, ndec):
			decay = decays[idec]
			brt += decay[jbr]
		if brt: 
			brt = 1.0 / brt
			for idec in range(0, ndec): 
				decay = decays[idec]
				decay[jbr] = decay[jbr] * brt
#=======================================================================
db = SqliteDatabase('Particles.db')

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

if Particle.table_exists():
    Particle.drop_table()
Particle.create_table()

if Decay.table_exists():
    Decay.drop_table()
Decay.create_table()

if Daughter.table_exists():
    Daughter.drop_table()
Daughter.create_table()
#=======================================================================
# main
filename = 'pdg_table.txt'
if (os.path.isfile(filename)):
	f = open(filename, 'r')
else: 
	print (filename + "does not exist")
	quit()
# reading the file line by line 	
np = 0
while True:
	try:
		line = f.next()
	except:
		break; 
	if ( np == 0):
		for i in range(0,3):
			if line.startswith("#") is False:
				print ("There should be three lines of comments at the beginning \n")
				quit(1)
			line = f.next()
		if line.startswith("#"):
			print ("Comment not expected here!!!\n")
	   		quit(1)
	np += 1
   	data = getPart(line)
   	if np != data[I_COUNT]:
   		print ("Wrong sequence count np(%i) != count(%i)" % np, int(data[I_COUNT])) 
   		quit(1)
   	if data[I_PDG] >= 0:
   		if data[I_ISOSPIN] != -100: 
   			data[I_ISOSPIN] = (data[I_ISOSPIN] - 1) / 2
   		if data[I_STRANGE] != -100: 
   			data[I_STRANGE] = (data[I_STRANGE] - 1) / 2
		if data[I_WIDTH] > 0: 
			life = HBAR / data[I_WIDTH] 
		else:
			life = 0.0
   		if data[I_NDECAY] > 0: 
   			for i in range(0, 3): 
   				line = f.next()
				if line.startswith("#") is False:
   					print "Disaster comment!!!\n"
   					quit(1)
   			decays = list()
   			for i in range(0, data[I_NDECAY]): 
   				line = f.next()
   				if line.startswith("#"): 
   					print ("No comment expected here!!!\n")
   					quit(1)
   				dcount = int(line[0:13])
   				decay = getDecay(line)
   				if dcount != i + 1: 
   					print ("Wrong sequence dcount (%i) != i+1 (%i)" % dcount, i + 1)
   					quit(1) 
   				decays.append(decay)
   			normDecay(decays)
		elif life == 0.0: 
			life = 1e38
		part = Particle.create(name = data[I_NAME], \
			pdg      = data[I_PDG], 
			matter   = data[I_MATTER], 
			pcode    = data[I_PCODE],
			pclass   = data[I_PCLASS], 
			charge   = data[I_CHARGE] / 3.0, 
   			mass     = data[I_MASS], 
   			width    = data[I_WIDTH], 
   			lifetime = life, 
   			isospin  = data[I_ISOSPIN], 
   			iso3     = data[I_ISO3], 
   			strange  = data[I_STRANGE], 
   			flavor   = data[I_FLAVOR], 
   			track    = data[I_TRACK], 
   			ndecay   = data[I_NDECAY])
		part.save()
		for decay in decays:
			childs = decay[2]
			dd = Decay.create(mother = part, dtype = decay[0], br = decay[1], ndaughters = len(childs))
			dd.save()
			for child in childs:
				ichild = int(child)
				dodo = Daughter.create(decay = dd, pdg = ichild)
				dodo.save
   		print ("Processing {}, {}" .format(data[I_COUNT], data[I_NAME]))
   	else: # create anti particle from particle
   		part = Particle.get(Particle.pdg == -data[I_PDG])
   		if part.charge is 0:
   			acharge = 0
   		else:
   			acharge = -part.charge
   		if part.isospin is 0:
   			aisospin = 0
   		else:
   			aisospin = -part.isospin
   		if part.iso3 is 0: 
   			aiso3 = 0
   		else:
   			aiso3 = -part.iso3
   		if part.strange is 0:
   			astrange = 0
   		else:
   			astrange = -part.strange
   		if part.flavor is 0:
   			aflavor = 0
   		else:
   			aflavor = -part.flavor
   		apart = Particle.create(name = data[I_NAME], \
			pdg      = data[I_PDG], 
			matter   = data[I_MATTER], 
			pcode    = part.pcode,
			pclass   = part.pclass, 
			charge   = acharge, 
   			mass     = part.mass, 
   			width    = part.width, 
   			lifetime = part.lifetime, 
   			isospin  = aisospin, 
   			iso3     = aiso3, 
   			strange  = astrange, 
   			flavor   = aflavor, 
   			track    = part.track, 
   			ndecay   = part.ndecay)
   		apart.save()

   		for decay in Decay.select().join(Particle).where(Particle.pdg == part.pdg):
   			dd = Decay.create(mother = apart, dtype = decay.dtype, br = decay.br, ndaughters = decay.ndaughters)
   			dd.save()
   			for child in Daughter.select().where(Daughter.decay == decay):
   				dodo = Daughter.create(decay = dd, pdg = -child.pdg)
   				dodo.save() 
   		print ("Processing {}, {}" .format(data[I_COUNT], data[I_NAME]))
db.close()
f.close()
quit()
