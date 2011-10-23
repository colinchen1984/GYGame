#coding=utf-8
tokens = (
   'STRUCTCOMMENT',
   'INT',
   'INT64',
   'BOOL',
   'UINT',
   'UINT64',
   'FLOAT',
   'STRING',
   'GUID',
   'GUIDARRAY',
   'INTARRAY',
   'INT64ARRAY',
   'UINT64ARRAY',
   'BOOLARRAY',
   'UINTARRAY',
   'FLOATARRAY',
   'STRINGARRAY',
   'EM',
   'SLEFDEfINETYPE',
   'SLEFDEfINETYPEARRAY',
   'BEGIN',
   'END',
   'VALUENAME',
   'NOCHECK',
   'VALUEMORECOMMENT',
   'NOBUDDY',
)

states = (
   ('SEARCHFORTYPTE','exclusive'),
   ('SEARCHNAME','exclusive'),
   ('SEARCHPARAM','exclusive'),
)

	
def t_STRUCTCOMMENT(t):
	r'\#.*'
	# print "comment\t%s|" % t.value
	# t.value = None
	t.value = {"Type":"Comment", "Value":t.value}
	return t
	
def t_INITIAL_BEGIN(t):
	r'\[.*\]'
	t.lexer.begin('SEARCHFORTYPTE')
	# print "begin\t%s|" % t.value	
	t.value = {"Type":"Begin", "Value":t.value}
	return t

def t_SEARCHFORTYPTE_GUIDARRAY(t):
	r'guid:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", true]
	t.value = {"Type":"Type", "DataType":"guid", "Array":True, "TypeName":t.value}
	return t
		
def t_SEARCHFORTYPTE_GUID(t):
	r'guid'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", false]
	t.value = {"Type":"Type", "DataType":"guid", "Array":False, "TypeName":"guid"}
	return t
	
def t_SEARCHFORTYPTE_BOOLARRAY(t):
	r'bool:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", true]
	t.value = {"Type":"Type", "DataType":"bool", "Array":True, "TypeName":t.value}
	return t
		
def t_SEARCHFORTYPTE_BOOL(t):
	r'bool'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", false]
	t.value = {"Type":"Type", "DataType":"bool", "Array":False, "TypeName":"bool"}
	return t

def t_SEARCHFORTYPTE_UINT64ARRAY(t):
	r'uint64:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", true]
	t.value = {"Type":"Type", "DataType":"uint64", "Array":True, "TypeName":t.value}
	return t
		
def t_SEARCHFORTYPTE_UINT64(t):
	r'uint64'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", false]
	t.value = {"Type":"Type", "DataType":"uint64", "Array":False, "TypeName":"uint64"}
	return t
	
def t_SEARCHFORTYPTE_INT64ARRAY(t):
	r'int64:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", true]
	t.value = {"Type":"Type", "DataType":"int64", "Array":True, "TypeName":t.value}
	return t
		
def t_SEARCHFORTYPTE_INT64(t):
	r'int64'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", false]
	t.value = {"Type":"Type", "DataType":"int64", "Array":False, "TypeName":"int64"}
	return t
	
def t_SEARCHFORTYPTE_INTARRAY(t):
	r'int:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", true]
	t.value = {"Type":"Type", "DataType":"int", "Array":True, "TypeName":t.value}
	return t
		
def t_SEARCHFORTYPTE_INT(t):
	r'int'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "int", false]
	t.value = {"Type":"Type", "DataType":"int", "Array":False, "TypeName":"int"}
	return t
	
def t_SEARCHFORTYPTE_UINTARRAY(t):
	r'uint:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "uint", true]
	t.value = {"Type":"Type", "DataType":"uint", "Array":True, "TypeName":t.value}
	return t
	
def t_SEARCHFORTYPTE_UINT(t):
	r'uint'
	t.lexer.begin('SEARCHNAME')
	# print "uint\t%s|" % t.value	
	# t.value = ["ValueType", "uint", false]
	t.value = {"Type":"Type", "DataType":"uint", "Array":False, "TypeName":"uint"}
	return t
	
def t_SEARCHFORTYPTE_FLOATARRAY(t):
	r'float:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "int\t%s|" % t.value	
	# t.value = ["ValueType", "float", true]
	t.value = {"Type":"Type", "DataType":"float", "Array":True, "TypeName":t.value}
	return t
	
def t_SEARCHFORTYPTE_FLOAT(t):
	r'float'
	t.lexer.begin('SEARCHNAME')
	# print "float\t%s|" % t.value	
	# t.value = ["ValueType", "float", false]
	t.value = {"Type":"Type", "DataType":"float", "Array":False, "TypeName":"float"}
	return t

def t_SEARCHFORTYPTE_STRINGARRAY(t):
	r'string:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "string\t%s|" % t.value		
	# t.value = ["ValueType", "string", true]
	t.value = {"Type":"Type", "DataType":"String", "Array":True, "TypeName":t.value}
	return t
	
def t_SEARCHFORTYPTE_STRING(t):
	r'string'
	t.lexer.begin('SEARCHNAME')
	# print "string\t%s|" % t.value		
	# t.value = ["ValueType", "string", false]
	t.value = {"Type":"Type", "DataType":"String", "Array":False, "TypeName":"String"}
	return t
	
def t_SEARCHFORTYPTE_EM(t):
	r'em[^\n\t]*'
	t.lexer.begin('SEARCHNAME')
	# print "em\t%s|" % t.value		
	# t.value = t.value
	# t.value = ["ValueType", "em", ]
	t.value = {"Type":"Type", "DataType":"Enum", "Array":False, "TypeName":t.value}
	return t
	
def t_SEARCHFORTYPTE_END(t):
	r'end'
	t.lexer.begin('INITIAL')
	# print "end\t%s|" % t.value	
	t.value = {"Type":"End"};
	return t
	
def t_SEARCHFORTYPTE_SLEFDEfINETYPEARRAY(t):
	r'[A-Za-z_][^\t\n]*:[a-zA-Z0-9_]+'
	t.lexer.begin('SEARCHNAME')
	# print "self\t%s|" % t.value	
	t.value = {"Type":"Type", "DataType":"Struct", "Array":True, "TypeName":t.value}
	return t
	
def t_SEARCHFORTYPTE_SLEFDEfINETYPE(t):
	r'[A-Za-z_][^\t\n]*'
	t.lexer.begin('SEARCHNAME')
	# print "self\t%s|" % t.value		
	t.value = {"Type":"Type", "DataType":"Struct", "Array":False, "TypeName":t.value}
	return t
	
def t_SEARCHNAME_VALUENAME(t):
	r'[A-Za-z_][^\t\n]*'
	t.lexer.begin('SEARCHPARAM')
	# print "value \t%s|" % t.value		
	t.value = {"Type":"ValueName", "ValueName":t.value}
	return t

def t_SEARCHPARAM_NOCHECK(t):
	r'NoCheck'
	# print "value comment\t%s|" % t.value	
	t.value = {"Type":"NoCheck"}
	return t
	
def t_SEARCHPARAM_VALUEMORECOMMENT(t):
	r'[^\t\n]*\n'
	t.lexer.begin('SEARCHFORTYPTE')
	# print "value comment\t%s|" % t.value	
	t.value = {"Type":"ExtraParam", "Value":t.value}
	return t
		
def t_SEARCHFORTYPTE_NOBUDDY(t):
	r'[ \n\t]+'	
	# print "nobody\t%s|" % t.value
	
def t_SEARCHNAME_NOBUDDY(t):
	r'[ \n\t]+'	
	# print "nobody\t%s|" % t.value
	
def t_SEARCHPARAM_NOBUDDY(t):
	r'[ \n\t]+'
	# print "nobody\t%s|" % t.value

def t_NOBUDDY(t):
	r'[ \n\t]+'
	
def t_SEARCHFORTYPTE_error(t):
	r'.*\n'
	# print len(t.value)
	
def t_SEARCHNAME_error(t):
	r'.*\n'
	# print len(t.value)
	
def t_SEARCHPARAM_error(t):
	r'.*\n'
	# print len(t.value)
	
def t_error(t):
	r'[.\n]*'
	# print t.value	
