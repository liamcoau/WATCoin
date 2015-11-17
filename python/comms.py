import serial
import array
import struct
import string
import bank
import mailbox
from uwldap import getUser
import pickle
import comms
import threading
#Shoehorned SMTP here. Minmum viable product!

import smtplib

fromaddr = 'watcoinbank@gmail.com'


username = fromaddr
password = 'thisisastrongpassword'


imap_username = 'watcoinbank@gmail.com'
imap_password = 'thisisastrongpassword'

#Our mailbox for receiving addresses
mbox = mailbox.MailBox(imap_username, imap_password)

#watcoind
bnk = bank.Bank()
bnk.ensureServer()
users= pickle.load(open( "users.pic", "rb" ) )

import time
def doMailCheck():
	global users
	global mbox
	while True:
		dats=mbox.get_addrs()
		for k,v in dats.iteritems():
			users[k]["sendTo"]=v
		time.sleep(5)

class SerialResponder:
	def __init__(self):
		global bank
		global users
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
				totalAmount=self.bank.getBalance(self.users["userString"]["bank"])
				name=self.users[userString]["quest"]
				self.serial.write("X".encode("ascii")) # success.
				self.serial.flush()
				print self.serial.read(1)
				tAmount=struct.pack(">I", totalAmount)[2:]
				self.serial.write(tAmount) # success.
				self.serial.write(bytes([len(name)])) # success.
				self.serial.write(name.encode("ascii"));
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
			#self.users[userString]={'bank':"bla"} # we need an account number from the Bank module. 
			print "Enrolled user with userString: "+userString
			lenQuest = self.serial.read(1)
			lenQuest = array.array('B', lenQuest)
			
			x=int(lenQuest[0]) # sent as byte.
			qID=self.serial.read(x)
			questUser=str(qID)
			self.users[userString]=getUser(questUser)
			#self.users[userString]["quest"]=questUser
			print "Quest "+questUser
			#Send an email, get real name.
			server = smtplib.SMTP('smtp.gmail.com:587')
			server.starttls()
			server.login(username,password)
			server.sendmail(fromaddr, questUser+'@uwaterloo.ca', "Welcome to the WatCoin bank! Your personalized destination address is "+self.bank.getAccountAddress(questUser)+". Please reply to this email with your desired recepient address.")
			server.quit()

		elif rd[0]==0x03:
			#withdraw money
			cksum=self.serial.read(4) # Read the 4 byte checksum
			userString=str(cksum)
			bID=self.users[userString]["bank"]
			amount=self.serial.read(2)
			realAmount=struct.unpack(">H", amount)
			print "Sending "+str(int(realAmount[0]))+" from " +bID
			self.bank.transferCoins(str(int(realAmount[0])),bID,self.users[userString]["sendTo"])
		else:
			print "Dde"
			exit()




#load users from database

t = threading.Thread(target=doMailCheck)
t.start()

s=SerialResponder()

while True:
	s.waitSerial()
