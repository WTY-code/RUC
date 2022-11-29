#coding=utf-8
#-*- codeing = utf-8 -*-

# 用naive bayes对短信分类
from nltk.classify import NaiveBayesClassifier
from nltk.classify import accuracy as nltk_accuracy
from nltk.corpus import PlaintextCorpusReader
import random
import time

# 定义一个特征函数，用于生成特征
def massage_feature(word,num_letter=1):
    return {'feature':word[-num_letter:]}

start = time.time()
# 加载短信语料库
message_corpus = PlaintextCorpusReader('./',['NB_spam.csv','NB_normal.csv'])
all_message = message_corpus.words()

# 对短信特征进行标记提取
labels_name = ([(massage,'spam') for massage in message_corpus.words('NB_spam.csv')]+[(massage,'normal') for massage in message_corpus.words('NB_normal.csv')])
random.seed(7)
random.shuffle(labels_name)

# 训练并预测模型
featuresets = [(massage_feature(n),massage) for (n,massage) in labels_name]
# 划分训练测试集
train_set,test_set = featuresets[200000:],featuresets[:200000]
classifier = NaiveBayesClassifier.train(train_set)

print('结果准确率：',str(100*nltk_accuracy(classifier,test_set))+str('%'))
end = time.time()
print('time:',end-start)