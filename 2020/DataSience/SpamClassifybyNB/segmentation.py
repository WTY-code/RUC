#-*- codeing = utf-8 -*-
# 该模块完成第一项任务，给带标签和不带标签数据分词
# 去除了停用词和标点符号
# 分词结果保存在seg_labled.txt和seg_unlabled.txt中
import jieba
import time

# 去除停用词和标点符号
def clearSen(s):
    s = s.strip(' ')
    s = s.replace('，','')
    s = s.replace(',', '')
    s = s.replace('。','')
    s = s.replace('、','')
    s = s.replace('、','')
    #s = s.replace('【','')是否是垃圾短信的代表信息？
    #s = s.replace('】','')
    s = s.replace('(','')
    s = s.replace(')','')
    s = s.replace('（','')
    s = s.replace('）','')
    s = s.replace('《','')
    s = s.replace('》', '')
    s = s.replace('；','')
    s = s.replace('~','')
    s = s.replace('～','')
    s = s.replace('…','')
    s = s.replace('\r', '')
    s = s.replace('\t', ' ')
    s = s.replace('\f', ' ')
    s = s.replace('/', '')
    s = s.replace('、', '')
    s = s.replace('/', '')
    s = s.replace('_', '')
    #s = s.replace('!', '',2)
    #s = s.replace('！', '',2)
    s = s.replace('?', '')
    s = s.replace('？', '')
    s = s.replace('➕', '')
    s = s.replace('：', '')
    s = s.replace('“', '')
    s = s.replace('”', '')
    s = s.replace('我','')
    s = s.replace('你', '')
    s = s.replace('他', '')
    s = s.replace('她', '')
    s = s.replace('们', '')
    s = s.replace('的', '')
    s = s.replace('地', '')
    s = s.replace('是', '')
    s = s.replace('啊', '')
    s = s.replace('吗', '')
    s = s.replace('嘛', '')
    s = s.replace('哟', '')
    s = s.replace('这', '')
    s = s.replace('那', '')
    s = s.replace('就', '')
    s = s.replace('哦', '')
    s = s.replace('了', '')
    s = s.replace('吧', '')
    s = s.replace('把', '')
    s = s.replace('被', '')
    s = s.replace('哎', '')
    s = s.replace('呀', '')
    s = s.replace('啦', '')
    s = s.replace('比', '')
    s = s.replace('并', '')
    s = s.replace('也', '')
    s = s.replace('和', '')
    s = s.replace('且', '')
    s = s.replace('着', '')
    s = s.replace('而', '')
    s = s.replace('有', '')
    s = s.replace('在', '')
    s = s.replace('位于','')
    s = s.replace('等', '')
    s = s.replace('哈', '')
    s = s.replace('x', '')
    s = s.replace('X', '')
    return s

# 将取出停用词和标点的短信分词
def cleanNseg(infilename,outfilename):
    """
    :param infilename: 要分词的数据文件
    :param outfilename: 将分词后的数据保存在此文件中
    :return cnt: 数据总条数
    """
    cnt=0
    inf = open(infilename,'r',encoding='utf8')
    line = inf.readline()
    outf = open(outfilename,'w',encoding='utf8')
    while line:
        line = clearSen(line)
        seg_list = jieba.cut(line, cut_all=False)
        print(' '.join(seg_list), file=outf)
        cnt += 1
        line = inf.readline()
    outf.close()
    inf.close()
    return cnt

if __name__ == "__main__":
    start = time.time()
    seginf1='labled.txt'
    segoutf1='seg_labled.txt'
    print(cleanNseg(seginf1,segoutf1))
    seginf2='unlabled.txt'
    segoutf2='seg_unlabled.txt'
    print(cleanNseg(seginf2,segoutf2))
    end = time.time()
    print("total time: ",end-start)
