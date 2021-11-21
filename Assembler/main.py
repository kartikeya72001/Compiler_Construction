import sys
import time as t


opcodes = {}
opcodes["CLA"] = "0000"
opcodes["LAC"] = "0001"
opcodes["SAC"] = "0010"
opcodes["ADD"] = "0011"
opcodes["SUB"] = "0100"
opcodes["BRZ"] = "0101"
opcodes["BRN"] = "0110"
opcodes["BRP"] = "0111"
opcodes["INP"] = "1000"
opcodes["DSP"] = "1001"
opcodes["MUL"] = "1010"
opcodes["DIV"] = "1011"
opcodes["STP"] = "1100"

opcodeTable = {}	#key is lc value is a list - first index is assembly opcode symbol;second is opcode of the symbol; third is operand
symbolTable = {}	#key is variable/label;value is a list- first index is value stored in variable; second index is memory allocated to variable
lableTable = {}		#lable table as dictionary : keys are labels and their value is location counter
ans = []
global divVariables
divVariables = False
memory = []


def passOne(file):
	global divVariables

	start = file.readlines()
	
	startList = start[0].split("\t")
	
	if("START" not in startList[0]):
		#print("ERROR : Missing START")
		sys.exit("ERROR: Missing START")		#stop program
		return
	elif(len(startList)==2):
		location_counter = int(startList[1])

	else:
		location_counter = 0

	address = 1		# index of start
	inputTable = {}


	while(address<len(start)):


		inputTable[location_counter] = list(map(lambda x:x.rstrip(),start[address].split("\t")))
		#	split the input and store in the dictionary inputTable as a list with lc as the key value

		if(len(inputTable[location_counter])>3):
			if(len(inputTable[location_counter][3])!=0 and inputTable[location_counter][3][0]!="/"):	#i.e. fourth element of list is not a comment
				sys.exit("ERROR at line " + str(location_counter) +": Incorrect syntax of input. Check documentation.")
			#	error if more than one variable is used in one instruction

		if(len(inputTable[location_counter])== 1):
			if("END" in inputTable[location_counter][0]):
				"""if END is read, first pass is finished."""
				return 
			sys.exit("ERROR at line " + str(location_counter) +": Incorrect syntax of input. Check documentation.")

		if(len(inputTable[location_counter])==2):
			if(inputTable[location_counter][1]!="CLA" and inputTable[location_counter][1]!="STP"):
				sys.exit("ERROR at line " + str(location_counter) +": Incorrect syntax of input. Check documentation.")

		if(isSymbolValid(inputTable[location_counter][1])):
			""" Exit program if an invalid opcode is detected."""
			sys.exit("ERROR at line " + str(location_counter) +": Invalid assembly opcode '"+inputTable[location_counter][1]+"'. Check documentation.")


		if(inputTable[location_counter][0]):
			"""	if there's a label"""
			if(inputTable[location_counter][0] in lableTable):
				"""	error if a label is declared twice in code"""
				sys.exit("ERROR at line "+str(location_counter)+": Label '"+inputTable[location_counter][0]+"' is already used. Cannot be used again. Check documentation and try again.")

			isLabelValid(inputTable[location_counter][0],location_counter)
			lableTable[inputTable[location_counter][0]] = location_counter		



		if(len(inputTable[location_counter])>=2):
			""" handling CLA and STP """

			if(inputTable[location_counter][1]=="STP"):

				if(len(inputTable[location_counter])>=3):
					if(inputTable[location_counter][2]):
						sys.exit("ERROR at line "+str(location_counter)+" : You cannot have a variable with STP.")

				if(address==len(start)-1):
					"""variables are not given values"""
					sys.exit("ERROR: Incorrect syntax of input.'END' expected at the end of program. Check documentation.")

				opcodeTable[location_counter] = [inputTable[location_counter][1],opcodes[inputTable[location_counter][1]]]
				address+=1
				location_counter+=1
				while(True):
					inputTable[location_counter] = list(map(lambda x:x.rstrip(),start[address].split("\t")))
					if(address==len(start)-1):
						if(inputTable[location_counter][0]=="END"):
							if(len(inputTable[location_counter])>1):
								# if there's a value/variable after END
								sys.exit("ERROR: Incorrect syntax . Check documentation.")

							if(divVariables):
								#if DIV is used n code
								symbolTable["R1"] = ['0',decimalToBinary(location_counter)]
								symbolTable["R2"] = ['0',decimalToBinary(location_counter+4)]
							return 
						sys.exit("ERROR: Incorrect syntax of input.'END' expected at the end of program. Check documentation.")
						# if END is missing
					if(inputTable[location_counter][0]=="END"):
						sys.exit("ERROR: Incorrect syntax. Check documentation.")
					readVariables(inputTable,location_counter)					#	to read variables after STP
					address+=1
					location_counter+=4											#	one word = 4 bytes
			elif(inputTable[location_counter][1]=="CLA"):
				# for CLA
				if(len(inputTable[location_counter])>=3):
					if(inputTable[location_counter][2]):
						sys.exit("ERROR at line "+str(location_counter)+" : You cannot have a variable with CLA.")


				opcodeTable[location_counter] = [inputTable[location_counter][1],opcodes[inputTable[location_counter][1]]]
				address+=1
				location_counter+=1
				continue



		if(len(inputTable[location_counter])>=3):


			if(inputTable[location_counter][1]=="BRZ" or inputTable[location_counter][1]=="BRN" or inputTable[location_counter][1]=="BRP"):
				opcodeTable[location_counter] = [inputTable[location_counter][1],opcodes[inputTable[location_counter][1]],inputTable[location_counter][2]]
				
				if(inputTable[location_counter][2] not in lableTable):
					symbolTable[inputTable[location_counter][2]] = "label"
					#	for forward referencing
				else:
					symbolTable[inputTable[location_counter][2]] = ["-",decimalToBinary(lableTable[inputTable[location_counter][2]])]
				address+=1
				location_counter+=1
				continue


			if(inputTable[location_counter][1]=="DIV"):
				divVariables= True

			isVariableValid(inputTable[location_counter][2],location_counter)
			symbolTable[inputTable[location_counter][2]] = False					# if variable is used but not declared

				

		if(len(inputTable[location_counter][2])==0):
			sys.exit("ERROR at line "+str(location_counter)+" : Incorrect syntax of input. Check documentation.")
		opcodeTable[location_counter] = [inputTable[location_counter][1],opcodes[inputTable[location_counter][1]],inputTable[location_counter][2]]
		address +=1
		location_counter+=1


	sys.exit("ERROR: END and STP expected at the end of program")	#if both END and STP are not provided in input
	return


def isVariableValid(variable,lc):
	global divVariables
	""" to check if variable is valid"""
	if(variable in opcodes.keys() or variable=="START" or variable=="END" or variable in opcodes.keys()):
		sys.exit("ERROR at line "+str(lc)+": '"+variable+"' is an invalid variable. Check documentation.")

	if(variable=="R1" or variable=="R2"):
		if(not divVariables):
			sys.exit("ERROR at line "+str(lc)+": '"+variable+"' is an invalid variable. Check documentation.")


def isLabelValid(label,lc):
	"""check if label is valid.
	error if label is same as opcode or assembly directive or same as a variable"""
	if(label in symbolTable and symbolTable[label]=="label"):
		symbolTable[label] = ["-",decimalToBinary(lc)]
		return

	if (label in opcodes.keys() or label == "END" or label == "START" or label in symbolTable):
		sys.exit("ERROR at line "+str(lc)+": Invalid label "+label+" .Check documentation and try again.")
	

def readVariables(inputTable, location_counter):
	""" store variable values provided after STP"""
	if(inputTable[location_counter][0] not in symbolTable):
		print("WARNING: Variable '"+inputTable[location_counter][0]+"' is declared but not used in code.")
	if(len(inputTable[location_counter])==1 or len(inputTable[location_counter][1])==0):
		sys.exit("ERROR: Please provide value to the variable "+str(inputTable[location_counter][0])+".")
	isVariableValid(inputTable[location_counter][1],"-")
	symbolTable[inputTable[location_counter][0]] = [inputTable[location_counter][1],decimalToBinary(location_counter)]



def isSymbolValid(inputSymbol):
	"""check if given opcode is valid"""
	ErrorFlag = False

	if(inputSymbol not in opcodes):
		ErrorFlag = True
		
	return ErrorFlag


def decimalToBinary(n):
	""" converts decimal to binary """ 
	ans = ""
	if(n>1):
		ans += decimalToBinary(n//2)
	ans += str(n%2)
	return ans


def printDict(dictionary):
	for i in dictionary.keys():
		if(dictionary[i]==False):
			continue
		if(type(dictionary[i])==list):
			print(str(i)+" : ",end="")
			for i in dictionary[i]:
				print(i,end=" ; ")
			print()
			continue

		print(str(i)+" : "+str(dictionary[i]))
	print()



def secondPass():

	for i in symbolTable.keys():
		if (symbolTable[i]==False):
			sys.exit("ERROR: "+i+" is used in program but value is not provided.")
		elif (symbolTable[i]=="label"):
			sys.exit("ERROR: label '"+i+"' is used but not defined.")

	
	for i in opcodeTable.keys():

		if(len(opcodeTable[i])==3):
			objAddress = symbolTable[opcodeTable[i][2]][1]
			if(len(objAddress)>8):
				objAddress = '001'
			objAddress = "0"*(8-len(objAddress)) + objAddress
			ans.append(opcodeTable[i][1]+ objAddress)
		else:
			""" in case of CLA and STP"""
			ans.append(opcodeTable[i][1]+("0"*8))


	print("OBJECT CODE")	
	for i in ans:
		""" print object code"""
		print(i)
	return


def printCool(toPrint):
	for i in toPrint:
		print(i,end="")
		t.sleep(0.05)
		sys.stdout.flush()
	for i in range(4):
		t.sleep(0.5)
		print(".",end=" ")
		sys.stdout.flush()
	print()



######################################## MAIN CODE STARTS HERE #######################################

inputFlag = True
while(inputFlag):
	try:
		file = input("Enter name of the file: ")
		asmFile = open(file, 'r')
		print()
		printCool("Initialising assembler")

		print("GIVEN OPCODES")
		printDict(opcodes)
		printCool("Initialising first pass")
		(passOne(asmFile))
		print("LABEL TABLE")
		printDict(lableTable)

		print("SYMBOL TABLE")
		printDict(symbolTable)

		print("OPCODE TABLE")
		printDict(opcodeTable)

		print()
		printCool("Initialising second pass")
		
		secondPass()
		
		inputFlag = False
	except FileNotFoundError:
		print("No File Found, Kindly Retry")