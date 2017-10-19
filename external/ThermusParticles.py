#!/usr/local/bin/python3
from __future__ import print_function
import sys
import os
import os.path
from peewee import * # installation of peewee required: pip3 install peewee
import array
import urllib.request



HBAR = 6.5821192815e-25 # GeV 
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

dbname = os.environ['PARTDIR'] + os.environ['DBNAME']
db = SqliteDatabase(dbname)

#=======================================================================
def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
#=======================================================================
def normDecay(decays):
	jbr = 0 
	ndec = len(decays)
	brt = 0.0
	if ndec : 
		for idec in range(0, ndec):
			decay = decays[idec]
			brt += decay[jbr]
		if brt: 
			brt = 100. / brt
	return brt
#=======================================================================
def countDecays(name):
	filename = os.environ['PARTDIR'] + name + "_decay.txt"
	if (os.path.isfile(filename)):
		f = open(filename, 'r')
	else: 
		# eprint ("%s does not exist" %(filename))
		return 0
	ndecay = 0
	for line in f:
		ndecay = ndecay + 1
	return ndecay
#=======================================================================
def makeDecays(thermusName, part):
	filename = os.environ['PARTDIR'] + thermusName + "_decay.txt"
	if (os.path.isfile(filename)):
		f = open(filename, 'r')
	else: 
		# eprint ("%s does not exist" %(filename))
		return 0
	decays = list()
	for line in f:
		line = line.strip('\n')
		data = line.split('\t')
		br = float(data[0])
		children = list()
		for i in range(1, len(data)):
			children.append(int(data[i]))
		decay = [br, children]
		decays.append(decay)
	norm = normDecay(decays)	
	for decay in decays:
		br  = decay[0]
		brn = br * norm
		children = decay[1]
		dd = Decay.create(mother = part, dtype = 0, br = br, brn = brn, ndaughters = len(children))
		dd.save()
		for i in range(0, len(children)):
			dodo = Daughter.create(decay = dd, pdg = children[i])
			dodo.save() 
#=======================================================================
def makeADecays(part, apart):
	for decay in Decay.select().join(Particle).where(Particle.pdg == part.pdg):
   		dd = Decay.create(mother = apart, dtype = decay.dtype, br = decay.br, brn = decay.brn, ndaughters = decay.ndaughters)
   		dd.save()
   		for child in Daughter.select().where(Daughter.decay == decay):
   			dodo = Daughter.create(decay = dd, pdg = -child.pdg)
   			dodo.save() 
#=======================================================================
def updateMasses():
	url = 'http://pdg.lbl.gov/2017/mcdata/mass_width_2017.mcd'
	filename = os.environ['PARTDIR'] + 'mass_width_2017.txt'
	try:
		urllib.request.urlretrieve(url, filename)
	except:
		eprint ("url %s not found" % url)
		return False
	if (os.path.isfile(filename)):
		f = open(filename, 'r')
	else: 
		eprint (filename + "does not exist")
		return False
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
					eprint ("error: cannot decode %s" % charges[index])
					return False
			for index in range(0,4):
				if pdg[index] != 0:
					part = Particle.select()
					if part.where(Particle.pdg == pdg[index]).exists():
						part = Particle.select().where(Particle.pdg == pdg[index]).get()
						part.mass     = mass
						part.width    = width
						part.lifetime = HBAR / width if width > 0 else 0.0
						part.save()
						print ("Mass/width of %s (%i) have been updated to %f/%f" %(part.name, part.pdg, mass, width))
					# else:
					# 	print ("Particle %i %s added to database" % (pdg[index], name[index]))
					# 	lifetime = HBAR / width if width > 0 else 0.0
					# 	Particle.create(name = name[index], \
					# 		pdg      = pdg[index], 
					#  		matter   = True, 
				 # 			pcode    = 100,
				 # 			pclass   = "unknown", 
			 	# 			charge   = charge[index], 
  		 #  					mass     = mass, 
  		 #  					width    = width, 
  		 #  					lifetime = lifetime, 
  		 # 					isospin  = -100, 
  		 # 					iso3     = 0, 
  		 #  					strange  = -100, 
  		 #  					flavor   = -1, 
		   # 					track    = -1, 
  		 #  					ndecay   = 0)
					apart = Particle.select()
					if apart.where(Particle.pdg == -pdg[index]).exists():
						apart = Particle.select().where(Particle.pdg == -pdg[index]).get()
						apart.mass     = part.mass
						apart.width    = part.width
						apart.lifetime = part.lifetime
						apart.save()
	f.close()
	return True
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
def updateDecays():
	url = 'https://raw.githubusercontent.com/rootpy/rootpy/master/rootpy/etc/pdg_table.txt'
	filename = "pdg_table.txt"
	try:
		urllib.request.urlretrieve(url, filename)
	except:
		eprint ("url %s not found" % url)
		return False
	if (os.path.isfile(filename)):
		f = open(filename, 'r')
	else: 
		eprint (filename + "does not exist")
		return False
	np = 0
	while True:
		try:
			line = f.readline()
		except:
			break; 
		if ( np == 0):
			for i in range(0,3):
				if line.startswith("#") is False:
					eprint ("There should be three lines of comments at the beginning \n")
					return False
				line = f.readline()
			if line.startswith("#"):
				eprint ("Comment not expected here!!!\n")
				return False
		np += 1
		data = getPart(line)
		if np != data[I_COUNT]:
   			eprint ("Wrong sequence count np(%i) != count(%i)" % np, int(data[I_COUNT])) 
   			return False
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
   					line = f.readline()
   					if line.startswith("#") is False:
   						eprint ("Disaster comment!!!\n")
   						return False
	   			decays = list()
   				for i in range(0, data[I_NDECAY]): 
   					line = f.readline()
   					if line.startswith("#"): 
   						eprint ("No comment expected here!!!\n")
   						return False
	   				dcount = int(line[0:13])
   					decay = getDecay(line)
   					if dcount != i + 1: 
   						eprint ("Wrong sequence dcount (%i) != i+1 (%i)" %(dcount, i + 1))
   						# return False 
   					decays.append(decay)
   				norm = normDecay(decays)
   			elif life == 0.0: 
   				life = 1e38
   			pdg  = data[I_PDG]
   			part = Particle.select()
   			if part.where(Particle.pdg == pdg).exists():
   				part = Particle.select().where(Particle.pdg == pdg).get()
   				if part.ndecay > 0: 
   					decay = Decay.select()
   					if decay.where(Decay.mother == part).exists():
   						decay = Decay.select().where(Decay.mother == part).get()
   					else:
   						eprint("no decay found in Thermus for %s" %(part.name))   
   			else :
   				eprint ("%i %s Not found in the Thermus Table" %(pdg, data[I_NAME]))
   			# for decay in Decay.select().join(Particle).where(Particle.pdg == part.pdg):
   			# 	dd = Decay.create(mother = apart, dtype = decay.dtype, br = decay.br, brn = decay.brn, ndaughters = decay.ndaughters)
   			# 	dd.save()
   			# 	for child in Daughter.select().where(Daughter.decay == decay):
   			# 		dodo = Daughter.create(decay = dd, pdg = -child.pdg)
   			# 		dodo.save() 
   			# print ("Processing {}, {}" .format(data[I_COUNT], data[I_NAME]))
	f.close()
	return True

#=======================================================================
def rename(pdg): 
	partname = ""
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
	pdgname = os.environ['DBNAME'].replace('Thermus', 'PDG')
	localname = os.environ['PARTDIR'] + pdgname
	db.init(localname)
	part = Particle.select()
	if part.select().where(Particle.pdg == pdg).exists():
		part = Particle.select().where(Particle.pdg == pdg).get()
		partname = part.name
	dbname = os.environ['PARTDIR'] + os.environ['DBNAME']
	db.init(dbname)
	return partname
#=======================================================================
def arename(pdg):
	partname = ""
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
	pdgname = os.environ['DBNAME'].replace('Thermus', 'PDG')
	localname = os.environ['PARTDIR'] + pdgname
	db.init(localname)
	apart = Particle.select()
	if apart.select().where(Particle.pdg == pdg).exists():
		apart = Particle.select().where(Particle.pdg == pdg).get()
		partname = apart.name
	dbname = os.environ['PARTDIR'] + os.environ['DBNAME']
	db.init(dbname)
	return partname
#=======================================================================
def createDB(): 
	if Particle.table_exists():
		Particle.drop_table()
	Particle.create_table()
	if Decay.table_exists():
		Decay.drop_table()
	Decay.create_table()

	if Daughter.table_exists():
		Daughter.drop_table()
	Daughter.create_table()

	filename = os.environ['PARTDIR'] + sys.argv[2]
	if (os.path.isfile(filename)):
		f = open(filename, 'r')
	else: 
		eprint ("%s does not exist" %(filename))
		return False
	for line in f:
		data = line.split('\t')
		stable    = int(data[0])
		name      = data[1]
		pdg       = int(data[2])
		spin      = int(data[3])  # spin degeneracy
		stat      = int(data[4])  # statistics 0:Boltzmann +1:Fermi-Dirac -1:Bose-Einstein
		mass      = float(data[5])
		s         = int(data[6])  # strangeness
		baryon    = int(data[7])  # baryon number
		charge    = int(data[8])  # electric charge
		c         = int(data[9])  # charm
		b         = int(data[10]) # beauty
		t         = 0        # top
		sc        = float(data[11]) # #s + #s-bar quarks
		cc        = abs(c)
		bc        = abs(b)
		tc        = abs(t)
		width     = float(data[12])
		life      = HBAR / width if width != 0 else 0.
		threshold = float(data[13])
		radius    = 0.       # hard sphere excluded volume radius
		ndecay = 0
		if stable == 0:
			ndecay = countDecays(name)
		pdgName = rename(pdg) 
		thermusName = name
		#print ("Adding ", thermusName, pdgName)
		if not pdgName == "":
			name = pdgName
		part = Particle.create(name  = name, 
	  		pdg       = pdg,
	  		spin      = spin,
	  		statistic = stat,
	  		mass      = mass, 
	  		s         = s,
	  		baryon    = baryon,
	  		charge    = charge,
	  		c         = c,
	  		b         = b,
	  		t         = t,
	  		sc        = sc,
	  		cc        = cc,
	  		bc        = bc,
	  		tc        = tc,
	  		width     = width,
	  		lifetime  = life,
	  		threshold = threshold,
	  		radius    = radius,
	  		ndecay    = ndecay)
		if stable == 0:
			makeDecays(thermusName, part)
		if  not (baryon == 0 and charge == 0 and s == 0 and c == 0 and b == 0 and t == 0) :
			aname = name + '_bar' 
			newname = arename(-pdg)
			if not newname == "":
				aname = newname
			else:
				if name.find('Delta') == -1 and name.find('Sigma') == -1: 
					if name.find('+') != -1: 
						aname = name.replace('+', '-')
					elif name.find('-') != -1:
						aname = name.replace('-', '+')
			#print ("Adding ", aname)
			apart = Particle.create(name = aname, 
				pdg = -part.pdg, 
				spin = part.spin, 
				statistic = part.statistic, 
				mass      = part.mass, 
				s         = -part.s, 
				baryon    = -part.baryon,
	  			charge         = -part.charge,
		  		c         = -part.c,
		  		b         = -part.b,
		  		t         = -part.t,
		  		sc        = part.sc,
	  			cc        = part.cc,
		  		bc        = part.bc,
		  		tc        = part.tc,
		  		width     = part.width,
	  			lifetime  = part.lifetime,
		  		threshold = part.threshold,
	 	 		radius    = part.radius,
	  			ndecay    = part.ndecay)
			makeADecays(part, apart)
	f.close()	
	return True
#=======================================================================
class BaseModel(Model):
	class Meta:
		database = db

class Particle(BaseModel):
	name     = CharField(unique = True)
	pdg      = IntegerField(unique = True)
	spin     = IntegerField()
	statistic= IntegerField()
	mass     = DoubleField()
	s        = IntegerField()
	baryon   = IntegerField()
	charge   = FloatField()
	c        = IntegerField()
	b        = IntegerField()
	t        = IntegerField()
	sc       = IntegerField()
	cc       = IntegerField()
	bc       = IntegerField()
	tc       = IntegerField()
	width    = DoubleField()
	lifetime = DoubleField()
	threshold= DoubleField()
	radius   = DoubleField()
	ndecay   = IntegerField()

class Decay(BaseModel): 
	mother     = ForeignKeyField(Particle, related_name = 'decays')
	dtype      = IntegerField()
	br         = DoubleField()
	brn        = DoubleField()
	ndaughters = IntegerField()

class Daughter(BaseModel): 
	decay = ForeignKeyField(Decay, related_name = 'daughters')
	pdg   = IntegerField()

db.connect()

if sys.argv[1] == "Create":
	createDB()
elif sys.argv[1] == "Update":
	updateMasses()
# elif sys.argv[1] == "UpdateD":
# 	updateDecays()
else: 
	sys.exit("%s is a wrong option" %(sys.argv[1]))
db.close()

