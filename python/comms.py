import serial
import array
import struct
import string

#This handles communications with the master Tiva.
#Doesn't really do much yet, need the bank module finished to do much more.


class SerialResponder:
	def __init__(self,bank,users):
		self.serial = serial.Serial("/dev/ttyACM0",9600)
		self.bank=bank
		self.users=users
	def waitSerial(self):
		rd=self.serial.read(1)
		rd=array.array('B', rd)
		print rd
		if rd[0]==0x01:
			#This is an auth/info request
			cksum=self.serial.read(4)
			userString=str(cksum)
			print userString
			print self.users
			if userString in self.users:
				print "Found user."
				self.users[userString]["bank"] # use this to get the total.
				totalAmount=5000;
				name=self.users[userString]["quest"]
				self.serial.write("X".encode("ascii")) # success.
				self.serial.flush()
				print self.serial.read(1)
				#print self.serial.read(1);
				# balance, upper
				#balance, lower
				#strlen byte
				#Full name of user
				tAmount=struct.pack(">I", 5000)[2:]
				self.serial.write(tAmount) # success.
				self.serial.write(bytes([len(name)])) # success.
				self.serial.write(name.encode());
				self.serial.flush()
			else:
				print 'nfound'
				self.serial.write("Y".encode("ascii")) # No such user, we should expect an auth.
				self.serial.flush()
				print self.serial.read(1)
		elif rd[0]==0x02: #new user
			#Read chksum
			cksum=self.serial.read(4) # Read the 4 byte checksum
			userString=str(cksum)
			self.users[userString]={'bank':"bla"} # we need an account number from the Bank module. 
			print "Enrolled user with userString: "+userString
			lenQuest = self.serial.read(1)
			lenQuest = array.array('B', lenQuest)
			
			x=int(lenQuest[0]) # sent as byte.
			qID=self.serial.read(x)
			questUser=str(qID)
			self.users[userString]["quest"]=questUser
			print "Quest "+questUser
			#Send an email, get real name.
		elif rd[0]==0x03:
			#withdraw money
			cksum=self.serial.read(4) # Read the 4 byte checksum
			userString=str(cksum)
			bID=self.users[userString]["bank"]
			amount=self.serial.read(2)
			realAmount=struct.unpack(">H", amount)
			print "Sending "+str(int(realAmount[0]))+" from " +bID
			#use bank to send to user's real address. 
		else:
			print "Dde"
			exit()
