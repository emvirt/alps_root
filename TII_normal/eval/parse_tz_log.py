#!/usr/bin/python
# -*- coding: utf-8- -*-
# 한글 주석이 있어도 실행이 되나?
import os
import sys
import math

tzLogNormalFileList = []
tzLogSecureFileList = []

tzLog = []
tzLogSum= [[0, [],[],[],[],[],[],[],[] ], [1, [],[],[],[],[],[],[],[] ] ,[2, [],[],[],[],[],[],[],[] ] ,[3, [],[],[],[],[],[],[],[] ] ]
def build_file_list():
	for root, dirs, files in os.walk('./'):
		for fname in files:
			full_fname = os.path.join(root, fname)
			if full_fname.find('.txt') > 0:
				if full_fname.find('normal') > 0:
					tzLogNormalFileList.append(full_fname)
				elif full_fname.find('secure') > 0:
					tzLogSecureFileList.append(full_fname)

	#print tzLogNormalFileList
	#print tzLogSecureFileList

	#print len(tzLogFileList)
	"""
	try:
		out_f=open(sys.argv[1], "r")
	except IOError as e:
		print(e)
		sys.exit(1)
	"""
	return
def construct_exec_secquence(secure_filename, normal_filename):
	try:
		secure_fd=open(secure_filename, "r")
		normal_fd=open(normal_filename, "r")
	except IOError as e:
		print(e)
		sys.exit(1)


	#DEBUG_LEVEL_2 처리
	line = normal_fd.readline()
	while(line):
		temp_log = []
		while ( line.find("debug_level = 2") < 0):
			line = normal_fd.readline()	
		#print line
		line = line.split()
		
		requestedFileOpID = int(line[5].split(',')[0],10)
		if requestedFileOpID == 0:
			requestedFileOp = 'Open'
		elif requestedFileOpID == 1:
			requestedFileOp = 'Read'
		elif requestedFileOpID == 2:
			requestedFileOp = 'Write'		
		elif requestedFileOpID == 3:
			requestedFileOp = 'Close'

		#print line[5].split(',')[0], requestedFileOp,  line[8].split(')')[0]
		temp_log.append(requestedFileOp)
		temp_log.append(requestedFileOpID)
		temp_log.append( int(line[8].split(')')[0] , 10))
		#print temp_log

		#DEBUG_LEVEL_3 처리
		line = secure_fd.readline()
		while (line.find("debug_level = 3") < 0):
			line = secure_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))
		
		#DEBUG_LEVEL_4 처리

		line = secure_fd.readline()
		while (line.find("debug_level = 4") < 0):
			line = secure_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))
		
		#DEBUG_LEVEL_5 처리

		line = secure_fd.readline()
		while (line.find("debug_level = 5") < 0):
			line = secure_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))

		#DEBUG_LEVEL_6 처리
		line = secure_fd.readline()
		while (line.find("debug_level = 6") < 0):
			line = secure_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))

		#DEBUG_LEVEL_7 처리
		line = normal_fd.readline()
		while (line.find("debug_level = 7") < 0):
			line = normal_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))

		#DEBUG_LEVEL_8 처리
		line = normal_fd.readline()
		while (line.find("debug_level = 8") < 0):
			line = normal_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))

		#DEBUG_LEVEL_9 처리
		line = normal_fd.readline()
		while (line.find("debug_level = 9") < 0):
			line = normal_fd.readline()
		#print line
		line = line.split()

		#print line[8].split(')')[0]
		temp_log.append( int(line[8].split(')')[0] , 10))
		#print temp_log

		tzLog.append(temp_log)

		line = normal_fd.readline()
		if ( line.find("debug_level = 0") > 0):
			return
	return

def get_avg():

	#for entery in tzLog:
	#	tzLogSum[entry[1]][].append(tzLog[entry][2])
	for entry in tzLog:
		#print entry[1]
		tzLogSum[entry[1]][1].append(entry[2])
		tzLogSum[entry[1]][2].append(entry[3])
		tzLogSum[entry[1]][3].append(entry[4])
		tzLogSum[entry[1]][4].append(entry[5])
		tzLogSum[entry[1]][5].append(entry[6])
		tzLogSum[entry[1]][6].append(entry[7])
		tzLogSum[entry[1]][7].append(entry[8])
		tzLogSum[entry[1]][8].append(entry[9])


	for index in range(len(tzLogSum)):
		print tzLogSum
		print index, sum(tzLogSum[index][1], 0.0) /  len(tzLogSum[index][1])
		print index, sum(tzLogSum[index][2], 0.0) /  len(tzLogSum[index][2])
		print index, sum(tzLogSum[index][3], 0.0) /  len(tzLogSum[index][3])
		print index, sum(tzLogSum[index][4], 0.0) /  len(tzLogSum[index][4])
		print index, sum(tzLogSum[index][5], 0.0) /  len(tzLogSum[index][5])
		print index, sum(tzLogSum[index][6], 0.0) /  len(tzLogSum[index][6])
		print index, sum(tzLogSum[index][7], 0.0) /  len(tzLogSum[index][7])
		print index, sum(tzLogSum[index][8], 0.0) /  len(tzLogSum[index][8])
		print 


	"""
	print sum(tzLogSum[entry[1]][1], 0.0) /  len(tzLogSum[entry[1]][1])
	print sum(tzLogSum[entry[1]][2], 0.0) /  len(tzLogSum[entry[1]][2])
	print sum(tzLogSum[entry[1]][3], 0.0) /  len(tzLogSum[entry[1]][3])
	print sum(tzLogSum[entry[1]][4], 0.0) /  len(tzLogSum[entry[1]][4])
	print sum(tzLogSum[entry[1]][5], 0.0) /  len(tzLogSum[entry[1]][5])
	print sum(tzLogSum[entry[1]][6], 0.0) /  len(tzLogSum[entry[1]][6])
	print sum(tzLogSum[entry[1]][7], 0.0) /  len(tzLogSum[entry[1]][7])
	print sum(tzLogSum[entry[1]][8], 0.0) /  len(tzLogSum[entry[1]][8])
	print len(tzLogSum[0])
	#print tzLogSum[0][1]
	#print tzLogSum[0][2]
	#print tzLogSum[0][3]
	#print tzLogSum[0][4]
	#print tzLogSum[0][5]
	#print tzLogSum[0][6]
	#print tzLogSum[0][7]
	#print tzLogSum[0][8]
	"""





	#print tzLogSum
	return


def main_entry():
	
	build_file_list()
	
	if len(tzLogSecureFileList) == len(tzLogNormalFileList):
		fileListLen = len(tzLogNormalFileList)
		for index in range(len(tzLogNormalFileList)):
			construct_exec_secquence(tzLogSecureFileList[index], tzLogNormalFileList[index])
	
	#print tzLog
	get_avg()
	#print len(tzLog)
	#print tzLog[4799]
	#print len(tzLogSum[0][1])
	#print len(tzLogSum[1][1])
	#print len(tzLogSum[2][1])
	#print len(tzLogSum[3][1])
	#print tzLogSum

if __name__=='__main__':
	main_entry()
