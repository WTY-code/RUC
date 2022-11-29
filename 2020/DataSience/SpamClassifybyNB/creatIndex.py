#coding=utf-8
#-*- codeing = utf-8 -*-

# 此模块创建索引并插入数据

import json
import time
from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk

class ElasticObj:
    def __init__(self, index_name, ip):
        """
        :param index_name: 索引名称
        :param index_type: 索引类型
        """
        self.index_name = index_name
        self.es = Elasticsearch([ip])

    def create_index(self):
        '''
        创建索引,创建索引名称为massage
        :param ex: Elasticsearch对象
        :return:
        '''
        # 创建映射
        _index_mappings = {
            "mappings": {
                "properties": {
                    "message": {
                        'type': 'text',
                        'analyzer': 'ik_smart',
                        'search_analyzer': 'ik_smart'
                    },
                    "lable": {
                        'type': 'text'
                    }
                }
            }
        }
        if self.es.indices.exists(index=self.index_name) is not True:
            res = self.es.indices.create(index=self.index_name, body=_index_mappings, ignore=400)
            print('create_index: ',res)


    # 插入数据
    def insert_data(self, inputfile):
        f = open(inputfile, 'r', encoding='utf8')
        data = []
        for line in f.readlines():
            # 存入list
            data.append(line.strip())
        f.close()

        ACTIONS = []
        i = 1
        bulk_num = 10000
        for list_line in data:
            list_line = json.loads(list_line)
            action = {
                "_index": self.index_name,
                "_id": i,
                "_source": {
                    "text_entry": list_line["text_entry"],
                    "lable": list_line["lable"]}
            }
            i += 1
            ACTIONS.append(action)
            # 批量处理，每一批10000条短信
            if len(ACTIONS) == bulk_num:
                print('插入', i / bulk_num, '批数据')
                print(len(ACTIONS))
                success, _ = bulk(self.es, ACTIONS, index=self.index_name, raise_on_error=True)
                del ACTIONS[0:len(ACTIONS)]
                print('bulk: ',success)

        if len(ACTIONS) > 0:
            success, _ = bulk(self.es, ACTIONS, index=self.index_name, raise_on_error=True)
            del ACTIONS[0:len(ACTIONS)]
            print('Performed %d actions' % success)


if __name__ == '__main__':
    filename = 'data4es.json'
    obj = ElasticObj("message", ip="127.0.0.1")
    start = time.time()
    obj.create_index()
    obj.insert_data(filename)
    end = time.time()
    print("total time: ",end-start)
