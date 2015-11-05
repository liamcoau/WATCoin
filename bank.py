import subprocess
import time
import json

class Bank:
	_address = "CjQzNUFLnKUJTfH3tVYFH9H6ZNNkF1xwpU"
	_watcoind = "./watcoin/src/watcoind"

	def __init__ (self):
		subprocess.Popen(["killall", "watcoind"])
		try:
			self.server = subprocess.Popen([self._watcoind, "-irc", "-printtoconsole"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		except Exception as e:
			print "Hit critical error when starting the watcoin server."
			raise e

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

	def getBankAddress (self):
		return self._address
	
	def getBalance (self, account):
		self.ensureServer()
		if account == self._address:
			balance = subprocess.Popen([self._watcoind, "getbalance"], stdout=subprocess.PIPE).communicate()[0]
			print balance
			return balance

	def transferCoins (self, amount, account, destination):
		self.ensureServer()
		#result = subprocess.Popen([self._watcoind, "sendtoaddress", destination, amount]).communicate()

	def shutdown (self):
		self.server.terminate()

