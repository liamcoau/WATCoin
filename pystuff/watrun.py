import bank
import mailbox
from uwldap import getUser
import pickle
import comms

imap_username = 'watcoinbank@gmail.com'
imap_password = 'thisisastrongpassword'

#Our mailbox for receiving addresses
mbox = mailbox.MailBox(imap_username, imap_password)

#watcoind
bnk = bank.Bank()
bnk.ensureServer()

#load users from database

users= pickle.load(open( "users.pic", "rb" ) )

#Each user is a dictionary:
# bank - the account name. Same as quest.
# quest - The quest ID
# name - Real name
# sendTo - BTC address to send coins to.
#serial responder:
sr = comms.SerialResponder(bnk,users)


