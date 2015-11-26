import serial
import array
from BBb_GPIO import keypad
import struct
import string
import bank
import mailbox
from uwldap import getUser
import pickle
import comms
import threading
#Shoehorned SMTP here. Minmum viable product!
keypd=keypad()
import smtplib

fromaddr = 'watcoinbank@gmail.com'


username = fromaddr
password = 'thisisastrongpassword'


#imap_username = 
#imap_password = 

#Our mailbox for receiving addresses


#watcoind
bnk = bank.Bank()
bnk.ensureServer()
users= pickle.load(open( "users.pic", "rb" ) )
userMap= pickle.load(open( "userMap.pic", "rb" ) )
#users={}
#userMap={}

import time
def doMailCheck():
	global users
	global userMap
	mbox = mailbox.MailBox('watcoinbank@gmail.com', 'thisisastrongpassword')
	mbox.__enter__()
	while True:
		print mbox.get_count()
		dats=mbox.get_addrs()
		for k,v in dats.iteritems():
			users[userMap[k]]["sendTo"]=v
			pickle.dump(users,open("users.pic","wb"))
			pickle.dump(userMap,open("userMap.pic","wb"))
		time.sleep(5)


#serialBoard = serial.Serial('/dev/ttyACM1',9600)

def waitSerial():
	global serialBoard
	global bnk
	global users
	global userMap
	rd=serialBoard.read(1)
	rd=array.array('B', rd)
	print rd
	if rd[0]==0x01:
		#This is an auth/info request
		cksum=serialBoard.read(4)
		userString=str(cksum)
		print userString
		print users
		if userString in users:
			print "Found user."
			users[userString]["bank"] # use this to get the total.
			totalAmount=bnk.getBalance(users[userString]["bank"])
			name=users[userString]["quest"]
			serialBoard.write("X".encode("ascii")) # success.
			serialBoard.flush()
			print serialBoard.read(1)
			tAmount=struct.pack(">I", totalAmount)[2:]
			serialBoard.write(tAmount) # success.
			serialBoard.write(bytes([len(name)])) # success.
			serialBoard.write(name.encode("ascii"));
			serialBoard.flush()
		else:
			print 'nfound'
			serialBoard.write("Y".encode("ascii")) # No such user, we should expect an auth.
			serialBoard.flush()
			print serialBoard.read(1)
	elif rd[0]==0x02: #new user
		#Read chksum
		cksum=serialBoard.read(4) # Read the 4 byte checksum
		userString=str(cksum)
		#users[userString]={'bank':"bla"} # we need an account number from the Bank module. 
		print "Enrolled user with userString: "+userString
		lenQuest = serialBoard.read(1)
		lenQuest = array.array('B', lenQuest)
		print "here"
		x=int(lenQuest[0]) # sent as byte.
		print x
		qID=serialBoard.read(x)
		print qID
		questUser=str(qID)
		users[userString]=getUser(questUser)
		userMap[questUser]=userString
		#users[userString]["quest"]=questUser
		print "Quest "+questUser
		#Send an email, get real name.
		server = smtplib.SMTP('smtp.gmail.com:587')
		server.starttls()
		server.login(username,password)
		server.sendmail(fromaddr, questUser+'@uwaterloo.ca', "Welcome to the WatCoin bank! Your personalized destination address is "+bnk.getAccountAddress(questUser)+". Please reply to this email with your desired recepient address.")
		server.quit()
	elif rd[0]==0x03:
		#withdraw money
		cksum=serialBoard.read(4) # Read the 4 byte checksum
		userString=str(cksum)
		bID=users[userString]["bank"]
		amount=serialBoard.read(2)
		realAmount=struct.unpack(">H", amount)
		print "Sending "+str(int(realAmount[0]))+" from " +bID
		bnk.transferCoins(str(int(realAmount[0])),bID,users[userString]["sendTo"])
	else:
		print "Dde"
		exit()

t = threading.Thread(target=doMailCheck)
t.start()

s=serial.Serial("/dev/ttyACM0",9600)
watcard=serial.Serial("/dev/ttyO1",9600)

#s=SerialResponder()

#while True:
#	waitSerial()
#import BBb_gpio.py
while True:
	s.write("\rWelcome to\nWATCoin\nSwipe to begin".encode("ASCII"))
	x=str(watcard.read(4))
	print x
	if x in users:
		user=users[x]
		toSend="\rWelcome\n"+users[x]["name"].split(" ")[0]+"\nBalance:\n"+str(bnk.getBalance(user["bank"]))
		s.write(toSend.encode("ASCII"))
		print "found user"
		time.sleep(3)
		s.write("\rEnter amount\n followed by #\n".encode("ASCII"))
		digit=None
		amt=""
		while digit!='#':
			while digit==None:
				digit=keypd.getKey()
			if digit=="#":
				break
			digit=str(digit)
			s.write(digit.encode("ASCII"))
			amt=amt+digit
			digit=None
			time.sleep(0.2)
		s.write("\r Withdraw:\n".encode("ASCII"))
		s.write(amt.encode("ASCII"))
		bnk.transferCoins(str(int(amt)),users[x]["bank"],users[x]["sendTo"])
		time.sleep(5)
	else:
		s.write("@".encode("ASCII"))
		questUser=s.readline()
		questUser=questUser.rstrip()
		users[x]=getUser(questUser)
		userMap[questUser]=x
		s.write("\rCheck your email\nfor instructions!\n")
		server = smtplib.SMTP('smtp.gmail.com:587')
		server.starttls()
		server.login(username,password)
		server.sendmail(fromaddr, questUser+'@uwaterloo.ca', "Welcome to the WatCoin bank! Your personalized destination address is "+bnk.getAccountAddress(questUser)+". Please reply to this email with your desired recepient address.")
		server.quit()
		time.sleep(2)

	# Bank steps.
	
