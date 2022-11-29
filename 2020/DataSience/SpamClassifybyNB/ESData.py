#!/usr/bin/env python
#coding=utf-8
#-*- codeing = utf-8 -*-

# 此模块为基于elasticsearch搭建搜索引擎准备数据
# 将带标签数据和不带标签数据都按elasticsearch可接受的形式整理成json文件 data4es.json

import time
import pandas as pd
import json

start = time.time()
f = open(r'labled.txt','r',encoding='utf8')
data = pd.read_csv(r"labled.txt", sep='\t',names=['lable', 'message'])
f.close()

number = 1
# 带标签数据
# 垃圾短信
jsonfile = open('data4es.json','w',encoding='utf8')
spam = data[data['lable'] == 1]
for index,row in spam.iterrows():
    tmp = row['message']
    dic = {}
    dic["text_entry"] = tmp
    dic["lable"] = "spam"
    json.dump(dic, jsonfile, ensure_ascii=False)
    jsonfile.write('\n')
    number+=1

print('spam finish')

# 正常短信
normal = data[data['lable'] == 0]
for index,row in normal.iterrows():
    tmp = row['message']
    dic = {}
    dic["text_entry"] = tmp
    dic["lable"] = "normal"
    json.dump(dic, jsonfile, ensure_ascii=False)
    jsonfile.write('\n')
    number+=1

print('normal finish')
# 不带标签数据
ff = open(r'unlabled.txt','r',encoding='utf8')
line = ff.readline()
while line:
    dic = {}
    dic["text_entry"] = line
    dic["lable"] = "unknown"
    json.dump(dic, jsonfile, ensure_ascii=False)
    jsonfile.write('\n')
    line = ff.readline()
ff.close()
print("unlabled finish")
jsonfile.close()

end = time.time()
print("total time:",end-start)

