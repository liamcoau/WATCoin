import subprocess
import time
import json

class Bank:
	_address = "CjQzNUFLnKUJTfH3tVYFH9H6ZNNkF1xwpU"
	_watcoind = "./watcoin/src/watcoind"

	def __init__ (self):
		#This is a race condition. The killall may, or may not run afterwards.
		#Used call to get around this - it'll wait for completion.
		#subprocess.call(["killall", "watcoind"])
		try:
			self.server = subprocess.Popen(["echo","x"])
			#self.server = subprocess.Popen([self._watcoind, "-irc", "-printtoconsole","-addnode=192.168.0.150","-addnode=10.154.60.147"], stdout=open("watcoin.log","a"), stderr=subprocess.PIPE)
		except Exception as e:
			print "Hit critical error when starting the watcoin server."
			raise e
		print self.server

	def ensureServer (self):
		while True:
			info = subprocess.Popen([self._watcoind, "getinfo"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
			print self.server
			print info
			if info[1]:
				print info[1]
			else:
				info = json.loads(info[0])["connections"]
				if info > 0:
					break
			time.sleep(1);
	def getAccountAddress(self,account): # creates an account if one doesn't exist.
		self.ensureServer()
		addr = subprocess.Popen([self._watcoind, "getaccountaddress",account], stdout=subprocess.PIPE).communicate()[0]
		return addr

	def getBalance (self, account):
		self.ensureServer()
		#if account == self._address:
		balance = subprocess.Popen([self._watcoind, "getbalance",account], stdout=subprocess.PIPE).communicate()[0]
		print "Account: "+str(account)+" Bal:" + str(balance)
		return float(balance)

	def transferCoins (self, amount, account, destination):
		self.ensureServer()
		result = subprocess.Popen([self._watcoind, "sendfrom", account, destination ,amount]).communicate()

	def shutdown (self):
		self.server.terminate()

