#!/usr/bin/python3
import struct
import os
import sys


# str1 = 	"#include \"engine_data.h\" "
#str2 =  "void *x_ape_get_ali (void) { "
# str3 =  "//#pragma section(\"my_data_section2\")"
# str4 =  "static const unsigned int surface_data[  ] ={"
# str5 = 	"}; "
# str6 = 	"return (void *)(surface_data); "
# str7 = 	"} "


#x_ape_get_ali_data.c
def HandFile():

#if os.path.exists("/home/flw/python/data"):
	os.chdir("/home/flw/python/data")
	L=[]
	arr=[]
	for file1 in os.listdir("/home/flw/python/data"):
	 	L.append(int(file1.split("_")[3])) 
		L.sort()
	print (L)
	
	sorted_file = []
	for sort_num in L:
		for file1 in os.listdir("/home/flw/python/data"):
			if str(sort_num) == file1.split("_")[3]:
				sorted_file.append(file1)
	
	for ali in range(len(sorted_file)):
		
		file = open(r"%s" %  sorted_file[ali], 'rb')
		num = os.path.getsize("%s" % sorted_file[ali])
		#print sorted_file[ali]
		
		outfile = open(r"%s" % "/home/flw/python/x_ape_get_ali_data.c", 'ab+')
		outfile.write("%s \n\r" %  "#include \"engine_data.h\" ")
		outfile.write("%s%d%s\n" % ("void *x_ape_get_ali_",ali+1,"(void) {" ))
		outfile.write("%s \n" % "//#pragma section(\"my_data_section2\")")
		outfile.write("%s \n" % "static const unsigned int surface_data[  ] ={" )
		
		
		if(num % 4==0):
			for c in range(0,num,4):
				file.seek(c)
				data = struct.unpack("I", file.read(4))
				if(c % 16==0):
					outfile.write("\n")
				outfile.write('0x%08x' % (data[0]) + ',')
		else:
			fmt1="I"
			for c1 in range(0,num,4):
				file.seek(c1)
				if c1 < num-(num % 4):
					buffer = struct.unpack("I", file.read(4))
				elif (num %4 ) == 1:
					buffer = struct.unpack("B",file.read(1))
				elif (num % 4) == 2:
					buffer = struct.unpack("H",file.read(2))
				elif (num % 4) == 3:
					buffer = struct.unpack("I",file.read(3)+'\0')
					# print buffer 

				if(c1% 16==0):
					outfile.write("\n")
				outfile.write('0x%08x' % (buffer[0]) + ',')


				

		# if(num % 16!=0):	
		# 	ex_file = open(r"%s" %  "/home/flw/python/ex.bin", 'wb')

		# 	for c in range(0,num,1):
		# 		file.seek(c)
		# 		data = struct.unpack("B", file.read(1))
		# 		x=str(data)
				
		# 		if(c % 16==0):
		# 			ex_file.write("\n")
		# 		ex_file.write('%s'  %  x.encode(encoding="utf-8"))
		# 		y=str(0)
		# 	for c in range(num,num+16-(num % 16)):
		# 			ex_file.write('%s' %  (y).encode(encoding="utf-8"))
		# 	ex_file.close()

			
		# 	read_ex_file = open(r"%s" %  "/home/flw/python/ex.bin", 'rb')

		# 	for j in range(0,num+16-(num % 16),4):
		# 		file.seek(j)
		# 		data1 = struct.unpack("I", read_ex_file.read(4))
		# 		if(j % 16==0):
		# 			outfile.write("\n")
		# 			print data1
		# 		outfile.write('0x%08x'  % (data1[0]) + ',')
			
		# 	read_ex_file.close
		file.close()

		outfile.write("\n%s \n" % "}; " )
		outfile.write("%s \n" % "return (void *)(surface_data); " )
		outfile.write("%s \n" % "} " )

		outfile.close()
		
	return 
	

HandFile()


#x_ape_get_ali.h
b = 1
size = 10
f= open(r"%s" % "/home/flw/python/x_ape_get_ali.h", 'wb')

for k in range(b,size,1):
    f.write("%s%d%s \n" % ("void * x_ape_get_ali",k,"(void);"))
f.close()


#x_ape_get_ali.c
# fp= open(r"%s" % str(sys.argv[3]), 'w')

# fp.write("%s \n" % "#include \"x_ape_get_ali.h\" ")
# fp.write("%s \n" % "#include \"ape_dev_if.h\" ")
# fp.write("%s \n\r" % "#include \"ape_config.h\" ")
# fp.write("%s \n\r" % "unsigned int mem_ptr[5]={}; ")
# fp.write("%s \n\r" % "void x_ape_get_ali(){ ")

# fp.close()







