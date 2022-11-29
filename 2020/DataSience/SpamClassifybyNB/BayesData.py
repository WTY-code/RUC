#coding=utf-8
#-*- codeing = utf-8 -*-

# 此模块准备供naive bayes分类的数据，形成两个csv文件: NB_spam.csv(垃圾短信) NB_normal.csv(正常短信)

import jieba
import pandas as pd
import segmentation as seg

f = open('labled.txt','r',encoding='utf8')
data = pd.read_csv("labled.txt", sep='\t',names=['lable', 'message'])
f.close()

# 垃圾短信
spamfile = open('NB_spam.csv','w',encoding='utf8')
spam = data[data['lable'] == 1]

for index,row in spam.iterrows():
    row['message'] = jieba.cut(seg.clearSen(row['message']), cut_all=False)
    print(' '.join(row['message']), file=spamfile)
spamfile.close()
print('spam finish')

# 正常短信
normalfile = open('NB_normal.csv','w',encoding='utf8')
normal = data[data['lable'] == 0]

for index,row in normal.iterrows():
    row['message'] = jieba.cut(seg.clearSen(row['message']), cut_all=False)
    print(' '.join(row['message']), file=normalfile)
normalfile.close()
print('normal finish')


