import pickle
import serial

#Handle query for user info
#Handle new user registration
#Keep track of mappings between name, questID, and account number
#Fetch balances from watcoind module upon request



#literally event loop - spin and wait for serial input
#Serial protocol (binary):
# char for type of message - Q - query user, A - add new user, W - withdraw
#Q: 4 byte WatCard hash
#response: Either: 1 byte N, which means new user, or O, followed by 2 byte # of coins, then length of name, then name, null terminated.
#A: 4 byte WatCard hash, 8-character Quest ID
#W - 4 byte WatCard hash, 2 byte # of coins

