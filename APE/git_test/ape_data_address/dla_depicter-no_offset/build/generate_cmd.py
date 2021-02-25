#!/usr/bin/python3
import os
import sys
import re

#获取group_id
def group(group_init,sort_name):
    count = 0
    count1 = 0
    count2 = 0
    for p in range(len(sort_name)):
        for i in range(len(group_init)):
            if (str(sort_name[p].split(' ')[0])== "CONV")& (i==p):
                if (count % 2 == 0):
                    group_init[i] = group_init[i]
                else:
                    group_init[i] = group_init[i]+1
                count +=1
                # print("group_init[",i,"]=",group_init[i],"count=",count)
            elif (str(sort_name[p].split(' ')[0])== "SDP")& (i==p):
                if (count1 % 2 == 0):
                    group_init[i] = group_init[i]
                else:
                    group_init[i] = group_init[i]+1
                count1 +=1
                # print("group_init[",i,"]=",group_init[i],"count1=",count1)
            elif (str(sort_name[p].split(' ')[0])== "PDP")& (i==p):
                if (count2 % 2 == 0):
                    group_init[i] = group_init[i]
                else:
                    group_init[i] = group_init[i]+1
                count2 +=1
                # print("group_init[",i,"]=",group_init[i],"count2=",count2)

    # print(group_init)    
    return group_init


#获取wait_isr
def wait(wait_init,sort_name):
    for w in range(len(sort_name)):
        for s in range(len(wait_init)):
            if ((str(sort_name[w].split(' ')[0]) =="SDP") &(str(sort_name[w-1].split(' ')[0]) =="SDP") &( s == w)):
                wait_init[s-1] = 1
    # print (wait_init)
    return wait_init

def main():
    file = open(r'%s' % sys.argv[1])
    #按行读取文件
    file_context =file.read().splitlines()
    #删除列表中重复元素
    lists = list(set(file_context))

    #获取Index列表，并排序删除重复元素
    L=[]
    for op in lists:
        L.append(int(op.split(' ')[1]))
    list1=sorted(set(L))

    #获取排序后的算子运算顺序
    sort_name=[]
    for sort_num in list1: 
        for op1 in lists:
            if (str(sort_num) == op1.split(' ')[1]) & (str(op1.split(' ')[0]) != "MEM_ADDR"):
                sort_name.append(op1)
    # print("debug:",sort_name)
    #SDP和Convolution调换顺序
    for i in range(len(sort_name)):
        if (str(sort_name[i].split(' ')[0]) =="SDP"):
            sort_name[i],sort_name[i-1] = sort_name[i-1],sort_name[i]
    #当SDP和CONV调换顺序后，连续出现两个SDP时，调换两SDP位置
    c=0
    for i in range(len(sort_name)):
        if((str(sort_name[i].split(' ')[0]) =="Convolution") &(str(sort_name[i-1].split(' ')[0]) =="SDP") &(str(sort_name[i-2].split(' ')[0]) =="SDP")):
            if (c % 2 == 0):
                sort_name[i-2],sort_name[i],sort_name[i-1] = sort_name[i-2],sort_name[i-1],sort_name[i]   
            c+=1 

    for j in range(len(sort_name)): 
        if (str(sort_name[j].split(' ')[0]) =="Convolution"):   
            sort_name[j]=str(sort_name[j].replace("Convolution","CONV"))
            # print(str(sort_name[j]))
    print ("sort_name=",sort_name)
    #M存放算子名称，Index存放索引
    M=[]
    Index=[]
    for m in sort_name:
        M.append(str(m.split(' ')[0]))
        Index.append(int(m.split(' ')[1]))
    # print ("M=",M)
    group_init = [0]*(len(Index))
    group_id = group(group_init,sort_name)
    print("group_id=",group_id,"size",len(Index))
    wait_init = [0]*(len(Index))
    wait_isr = wait(wait_init,sort_name)
    
    outfile = open(r'%s' % sys.argv[2],'w')
    outfile.write("#include \"ape_cmd.h\"\n")  
    outfile.write("#include \"ape_config.h\"\n") 
    outfile.write("#include \"x_ape_get_ali.h\" \n")
    outfile.write("//#pragma section(\"my_data_section5\") \n")
    outfile.write("struct ape_cmd cmd_data[APE_OPERATION_NUM] = {\n")
    for i in range(len(Index)):
        if (str(M[i])=="CONV"):
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{SETHWIDX_OP    ,APE_OP_",M[i].ljust(14),",",group_id[i],",",0,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{PROGRAM_OP     ,APE_OP_",M[i].ljust(14),",",group_id[i],",",0,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{SETHWIDX_OP    ,APE_OP_",M[i].ljust(14),",",group_id[i],",",0,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{ENABLE_OP      ,APE_OP_",M[i].ljust(14),",",group_id[i],",",0,",",1,",",Index[i],"},"))
        elif (str(M[i])=="PDP"):
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{SETHWIDX_OP    ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{PROGRAM_OP     ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{SETHWIDX_OP    ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{ENABLE_OP      ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",1,",",Index[i],"},"))
        elif (str(M[i])=="SDP"):
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{SETHWIDX_OP    ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{PROGRAM_OP     ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{SETHWIDX_OP    ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",0,",",Index[i],"},"))
            outfile.write("%20s%s%s%s%d%4s%d%4s%d%4s%s%5s\n" % (" ","{ENABLE_OP      ,APE_OP_",M[i].ljust(14),",",group_id[i],",",1,",",wait_init[i],",",Index[i],"},"))    
    outfile.write("%16s%2s\n" % ("","};"))
    outfile.close()
    file.close()




if __name__ == '__main__':
    main()
    
