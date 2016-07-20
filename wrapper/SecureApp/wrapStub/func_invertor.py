#!/usr/bin/python
import sys
import re

wrap_func_list=[]

def create_function_list():
	try:
        	fd = open("./functions.h","ro")
	except IOError as e:
        	print(e)
        	sys.exit(1)
	while 1:
		line = fd.readline()
		if not line: break
		if line == '\n':continue
		token = re.split('[: "(" ]+',line )
		if token[0] == "int":
			wrap_func_list.append(token[1])
	fd.close()

	header = "int (*wrap_func_ptr[" +str(len(wrap_func_list)+1) + "])(int *pDatabuf)" + " = { \n"
	for func_name in wrap_func_list:
		header = header + "\t"  + func_name + ",\n"
	header = header + "\t0\n" + "};"
#	print header

	fd = open("func_pointers.h", "w")
	fd.write(header)
	fd.close()

if __name__ == '__main__':
	create_function_list()
