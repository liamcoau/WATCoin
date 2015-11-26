#import ldap

#returns the user's information based on their Quest ID.
def getUser(uid):
	res={}
	res["name"]=uid#"Alexander Roth"
	res["qid"]=uid
	res["quest"]=uid
	res["bank"]=uid
	res["email"]=uid+"@uwaterloo.ca"
	return res
	l=ldap.open('uwldap.uwaterloo.ca')
	l.simple_bind_s()
	base="ou=people,dc=uwaterloo,dc=ca"
	result_id=l.search(base,ldap.SCOPE_SUBTREE,"uid="+uid,None)
	result_set=[]
	while 1:
		result_type, result_data = l.result(result_id, 0)
		if (result_data == []):
			break
		else:
			if result_type == ldap.RES_SEARCH_ENTRY:
				result_set.append(result_data)
	
	res={}
	res["name"]=result_set[0][0][1]["displayName"][0]
	res["qid"]=uid
	res["quest"]=uid
	res["bank"]=uid
	res["email"]=result_set[0][0][1]["mail"][0]
	return res
