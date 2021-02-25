#!/usr/bin/python3
import struct
import os
import sys
import re
from itertools import groupby


folder_path = os.path.abspath(r'%s' % sys.argv[1]).replace('\\','/')
folder_path1 = os.path.abspath("x_ape_get_ali").replace('\\','/')
folder_path2 = os.path.abspath("x_ape_get_ali.c").replace('\\','/')
folder_path3 = os.path.abspath("x_ape_get_ali.h").replace('\\','/')
folder_path4 = os.path.abspath("op_dat.c").replace('\\','/')
folder_path5 = os.path.abspath("surface_dat.c").replace('\\','/')
folder_path6 = os.path.abspath("").replace('\\','/')

##########################x_ape_get_ali_data.c#################################
def HandFile():

	os.chdir(folder_path)
	L=[]
	for file1 in os.listdir(folder_path):
		# for k, g in groupby(file1, key=lambda x: x.isdigit()):
		# 	print k
		# 	print g
		# 	print list(g)
		ss = [''.join(list(g)) for k, g in groupby(file1, key=lambda x: x.isdigit())] #分割字母和数字，排除掉flag=1的文件
		if( int(ss[-2]) != 1 ):
			# print ss[-2]
			L.append(int(file1.split("_")[3])) 
			L.sort()	
	sorted_file = []
	for sort_num in L:															#剩下文件按照ali的值排序
		for file1 in os.listdir(folder_path):
			if str(sort_num) == file1.split("_")[3]:
				sorted_file.append(file1)
	print (sorted_file)

	for ali in range(len(sorted_file)):
		# print sorted_file[ali]
		file = open(r"%s" %  sorted_file[ali], 'rb')
		num = os.path.getsize("%s" % sorted_file[ali])
		# print sorted_file[ali]
	
		o_file = open(r"%s%d%s" % (folder_path1,L[ali],"_data.c"), 'w')
		o_file.write("%s \n\n" %  "#include \"x_ape_get_ali.h\" ")
		o_file.write("%s%d%s\n" % ("void *x_ape_get_ali", L[ali] ,"(void) {" ))
		o_file.write("%s \n" % "//#pragma section(\"my_data_section2\")")
		o_file.write("%s%d%s \n" % ("static const unsigned int ali",L[ali],"_data[  ] ={" ))
	
		if(num % 4==0):
			for c in range(0,num,4):
				file.seek(c)
				data = struct.unpack("I", file.read(4))
				if(c % 16==0):
					o_file.write("\n")
				o_file.write('0x%08x' % (data[0]) + ',')
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
					o_file.write("\n")
				o_file.write('0x%08x' % (buffer[0]) + ',')

		file.close()
		o_file.write("\n%s \n\n" % "}; " )
		o_file.write("%s%d%s \n\n" % ("return (void *)(ali",L[ali],"_data);"))
		o_file.write("%s \n" % "} " )

	o_file.close()

#################################################################################################
#op/surface_index
	P =[]
	I =[]
	for desc in L:
		for file5 in os.listdir(folder_path):
			if desc == 0 :
				if str(desc) == file5.split("_")[3]:
					P.append(file5)
	file = open(r"%s" %  P[0], 'rb')
	num5 = os.path.getsize("%s" % P[0])
	for d in range(0,num5,2):
		file.seek(d)
		data = struct.unpack("H", file.read(2))
		I.append(data)
	o = (I[0][0])
	s = (I[1][0])
	
	desc_file = open(r"%s" % folder_path4, 'w')
	desc_file.write("%s \n\n" %  "#include \"x_ape_get_ali.h\" ")
	desc_file.write("%s \n\n\n" %  "void x_ape_get_op_data(){ ")
	desc_file.write("%s%d%s \n\n" %  ("	x_ape_get_ali",o,"();"))
	desc_file.write("%s \n\n" %  "	return;")
	desc_file.write("%s \n" %  "}")
	desc_file.close()

	desc1_file = open(r"%s" % folder_path5, 'w')
	desc1_file.write("%s \n\n" %  "#include \"x_ape_get_ali.h\" ")
	desc1_file.write("%s \n\n\n" %  "void x_ape_get_surface_data(){ ")
	desc1_file.write("%s%d%s \n\n" %  ("	x_ape_get_ali",s,"();"))
	desc1_file.write("%s \n\n" %  "	return;")
	desc1_file.write("%s \n" %  "}")
	desc1_file.close()		

###################################################################################################
#x_ape_get_ali.c
	M=[]
	S=[]
	Q=[]
	R=[]
	Z=[]
	for file3 in os.listdir(folder_path):
		Q.append(int(file3.split("_")[3]))

		ss = [''.join(list(g)) for k, g in groupby(file3, key=lambda x: x.isdigit())]
		if( int(ss[-2]) == 5 ):                        	#input
			M.append(int(file3.split("_")[3])) 			
    
		elif( int(ss[-2]) == 1 ):                   	#alloc   
			S.append(int(file3.split("_")[3]))
		
		elif( int(ss[-2]) == 9 ):						#output
			Z.append(int(file3.split("_")[3]))
	M.sort()
	S.sort()
	Q.sort()	
	Z.sort()

	for q in range(len(Q)):								#weight	
		if Q[q] ==1:
			for file3 in os.listdir(folder_path):
				if str(Q[q]) == file3.split("_")[3]:
					R.append(file3)
					R.append(int(file3.split("_")[3]))
	
	# print R
	# print M
	# print S
	# print Q
	# print (Z)
	sorted_file1 = []	
	for sort_num1 in M:
		for file3 in os.listdir(folder_path):
			if str(sort_num1) == file3.split("_")[3]:
				sorted_file1.append(file3)
	# print sorted_file1	

	sorted_file2 = []
	for sort_num2 in S:
		for file4 in os.listdir(folder_path):
			if str(sort_num2) == file4.split("_")[3]:
				sorted_file2.append(file4)
	# print sorted_file2	

	sorted_file3 = []
	for sort_num3 in Z:
		for file5 in os.listdir(folder_path):
			if str(sort_num3) == file5.split("_")[3]:
				sorted_file3.append(file5)
	# print (sorted_file3)

	outfile = open(r"%s" % folder_path2, 'w')
	outfile.write("#include \"x_ape_get_ali.h\"\n")    
	outfile.write("#include \"ape_dev_if.h\"\n")  
	outfile.write("#include \"ape_config.h\"\n") 
	outfile.write("#include \"ape_interface.h\"\n")
	outfile.write("#include \"ape_engine_internal.h\"\n") 
	outfile.write("#include \"log.h\"\n\n")
	outfile.write("%s%d%s\n\n" % ("unsigned long long mem_ptr[",len(Q),"]={};" ))
	outfile.write("void x_ape_get_init_data(){\n\n")

	outfile.write("	ScU8 * weight_ptr =(ScU8 *)(x_ape_get_ali1());\n\n")
	# for num2 in range(len(sorted_file1)):
	# 	outfile.write("%s%d%s%d%s\n" % ("	ScU8 * input_ptr", num2," =(ScU8 *)(x_ape_get_ali",M[num2],"());"))
	# outfile.write("\n\n")

	# for num3 in range(len(sorted_file3)):
	# 	outfile.write("%s%d%s%d%s\n" % ("	ScU8 * output_ptr", num3," =(ScU8 *)(x_ape_get_ali",Z[num3],"());"))
	# outfile.write("\n\n")

	weight_size  = os.path.getsize("%s" % R[0])
	outfile.write("%s %d %s %d %s \n" % ("	mem_ptr[" , R[1] ,"] = dram_init(WEIGHT_SIZE,",R[1],");"))
	outfile.write("%s %d %s \n\n" % ("	dram_copy( WEIGHT_SIZE ,weight_ptr,", R[1], ");"))

	for s in range(len(sorted_file2)):											#alloc
		alloc_size  = os.path.getsize("%s" % sorted_file2[s])
		outfile.write("%s %d %s %d %s %d %s\n" % ("	mem_ptr[", S[s], "] = dram_init(",alloc_size,",", S[s], ");"))
		# outfile.write("%s %d %s %d %s \n\n" % ("	dram_copy(",alloc_size,",NULL,",S[s],");"))
	outfile.write("\n\n")

	for m in range(len(sorted_file1)):											#input						
		input_size  = os.path.getsize("%s" % sorted_file1[m])
		outfile.write("%s %d %s %d %s \n" % ("	mem_ptr[",M[m],"] = dram_init(INPUT_SIZE,",M[m],");"))
		# outfile.write("%s %d %s%d %s %d %s \n\n" % ("	dram_copy( ",input_size ,",input_ptr",m,",",M[m],");"))	
	outfile.write("\n\n")
	

	for z in range(len(sorted_file3)):											#output
		output_size  = os.path.getsize("%s" % sorted_file3[z])
		outfile.write("%s %d %s %d %s \n\n" % ("	mem_ptr[", Z[z], "] = dram_init(OUTPUT_SIZE,",Z[z],");"))
	# 	# outfile.write("%s%d %s %d %s\n\n" % ("	dram_copy( OUTPUT_SIZE ,output_ptr",z,",",Z[z],");"))

	# for debug_index in range(len(L)):
	# 	outfile.write("%s %d %s %d %s \n\n"  % ("	LOG_DEBUG(\"mem_ptr[", L[debug_index], "] = %x \\n\",mem_ptr[" ,L[debug_index],"]);"))	
	outfile.write("	return ;\n\n")
	outfile.write("}\n")

	outfile.close()

###################################################################################################
# x_ape_get_ali_h():

	f= open(r"%s" % folder_path3, 'w')

	f.write("%s \n" % "#ifndef _X_APE_GET_ALI_H")
	f.write("%s \n\n" % "#define _X_APE_GET_ALI_H")

	f.write("#include \"ape_types.h\"\n\n") 
	f.write("%s %d \n" % ("#define APE_OPERATION_NUM",(I[15][0])*4))
	f.write("%s %d \n\n" % ("#define WEIGHT_SIZE",weight_size))
	# f.write("%s %d \n" % ("#define ALLOC_SIZE",alloc_size))
	f.write("%s %d \n\n" % ("#define INPUT_SIZE",input_size))
	f.write("%s %d \n\n" % ("#define OUTPUT_SIZE",output_size))
	
	for k in range(len(L)):
		
		f.write("%s%d%s \n\n" % ("void * x_ape_get_ali",L[k],"(void);"))

	f.write("%s \n" % "#endif")
	f.close()

	return 
HandFile()

###########################################################

# os.chdir(folder_path6)
# def sub(list, ostr, cstr):
#     """特定字符串替换"""
#     for i in range(len(list)):
#         if list[i] == ostr:
#             list[i] = cstr

# def cmd_append(list, sstr):
#     """将列表原元组扩充一倍,扩充部分用同一字符串元组，例如将‘a’‘b’‘c’改为‘a’‘d''b'd''c''d'"""
#     chlist = []
#     for i in range(2 * len(list)):
#         if i % 2 == 0:
#             chlist.append(sstr)
#         else:
#             chlist.append(list[int((i - 1)/ 2)])
#     return chlist

# def append(list):
#     """将列表原元组等值扩充一倍，例如将‘a’‘b’‘c’改为‘a’‘a''b'b''c''c'"""
#     chlist = []
#     for i in range(2 * len(list)):
#         if i % 2 == 0:
#             chlist.append(list[int(i/2)])
#         else:
#             chlist.append(list[int((i-1)/2)])
#     return chlist

# def rdma_needed(alist, sstr, stxt):
#     """对目标文本中是否存在特定字符串进行遍历，这里是’utils_get_free_group‘字符串，并进行后续操作"""
#     rdma_list = len(alist) * ['1']
#     if sstr in stxt:
#         for i in range(len(alist)):
#             if 'CONV' in alist[i]:
#                 rdma_list[i] = '0'
#     return rdma_list

# def wait_needed(alist, blist):
#     """对不同算子的enable阶段是否需要wait_need信号进行判断，先是索引到enable阶段，在判断是否与上一行的op_index相同；如果不相同，则需要等待，即wait_need信号为1"""
#     clist = len(alist) * ['0']
#     for i in range(len(alist)):
#         if 'ENABLE_OP' in alist[i]:
#             if blist[i] != blist[i-1]:
#                 clist[i] = '1'
#     return clist

# def main(): 
#     file = open(r'%s' % sys.argv[2])
#     txt = file.read()
#     pattern = re.compile('^\[.*\] (\w{6,7} \w{3,11} operation index \d* ROI \d.{0,9})$',re.M) 
#     mylist = re.findall(pattern, txt)
#     """获取文本中关键字符串的正则表达式"""
    
#     new_list = mylist[ : ]
#     for i in range(len(mylist)):
#         if 'Enable' in mylist[i]:
#             for nlist in new_list:
#                 a = mylist[i]           
#                 while ('Program' in nlist) & (a.split()[4] == nlist.split()[4]):
#                     mylist[i] = a + ' ' + nlist.split()[7]
#                     break
#     """对提取的enable语句缺少的group_id补充，其值与相同op_index的program语句的group_id相同""" 
#     # for i in range(len(mylist)):
# 	#     print ('%s\n' % mylist[i])
#     part_1 = [mlist.split()[0] for mlist in mylist]
#     part_2 = [mlist.split()[1] for mlist in mylist]
#     part_3 = [mlist.split()[7] for mlist in mylist]
#     part_6 = [mlist.split()[4] for mlist in mylist]
#     ostr1, ostr2, ostr3, ostr4, ostr5, ostr6, ostr7= 'Program', 'Enable', 'Convolution', 'SDP', 'PDP', 'Group[0]', 'Group[1]'
#     cstr1, cstr2, cstr3, cstr4, cstr5, cstr6, cstr7 = 'PROGRAM_OP', 'ENABLE_OP', 'APE_OP_CONV', 'APE_OP_SDP', 'APE_OP_PDP', '0', '1'
#     sub(part_1, ostr1, cstr1), sub(part_1, ostr2, cstr2) 
#     sub(part_2, ostr3, cstr3), sub(part_2, ostr4, cstr4), sub(part_2, ostr5, cstr5)
#     sub(part_3, ostr6, cstr6), sub(part_3, ostr7, cstr7)

#     rdma_str = 'utils_get_free_group'
#     part_4 = rdma_needed(part_2, rdma_str, txt)

#     part_5 = wait_needed(part_1, part_6)

#     sstr1 = 'SETHWIDX_OP'	
#     cmd_type = cmd_append(part_1, sstr1)
#     op_type = append(part_2)
#     group_id = append(part_3)
#     rdma_need = append(part_4)
#     wait_need = cmd_append(part_5, '0')
#     op_index = append(part_6)
#     # print(wait_need)
    
#     outfile = open(r'%s' % sys.argv[3],'w')
#     outfile.write("#include \"ape_cmd.h\"\n")  
#     outfile.write("#include \"ape_config.h\"\n") 
#     outfile.write("#include \"x_ape_get_ali.h\" \n")
#     outfile.write("//#pragma section(\"my_data_section5\") \n")
#     # outfile.write("#if RESNET\n\n")
#     outfile.write("struct ape_cmd cmd_data[APE_OPERATION_NUM] = {\n")
#     cmd_len = len(cmd_type) - 1  
#     for i in range(cmd_len):
#         outfile.write("%20s%s%s%s%s%s%s%s%s%s%s%s%s%2s\n" % (" ","{",cmd_type[i].ljust(13),",",op_type[i].ljust(13),",",group_id[i].ljust(3),",",rdma_need[i].ljust(4),",",wait_need[i].ljust(4),",",op_index[i].ljust(5),"}," ))
#     outfile.write("%20s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (" ","{",cmd_type[cmd_len].ljust(13),",",op_type[cmd_len].ljust(13),",",group_id[cmd_len].ljust(3),",",rdma_need[cmd_len].ljust(4),",",wait_need[cmd_len].ljust(4),",",op_index[cmd_len].ljust(5),'}'))
#     outfile.write("%16s%2s\n" % ("","};"))
#     outfile.close()
    
  
# if __name__ == '__main__':
#     main()
