#!/usr/bin/env python
#coding=utf-8
#-*- codeing = utf-8 -*-

# 此模块实现给定关键词进行全文搜索功能
# 用户输入key_words,输出与关键词相关的短信全文和标签(直接修改key_words)
# 标签有三种: spam 垃圾短信; normal 正常短信; unknown 不带标签的数据

import time
from elasticsearch import Elasticsearch

key_words = "优惠"

es = Elasticsearch(["127.0.0.1"])
doc = {
            "query": {
                "match": {
                    "text_entry": key_words
                }
            }
        }
start = time.time()
res = es.search(index="message",body=doc)
for i in range(len(res)):
    print(res['hits']['hits'][i]['_source']['text_entry'],res['hits']['hits'][i]['_source']['lable'])
end = time.time()
print("\ntotal time:",end-start)