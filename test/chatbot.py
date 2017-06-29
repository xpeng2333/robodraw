# coding:utf-8
import json
import requests

KEY = 'ff772ad12e0c421f98da2dd7f6a9289c'    # change to your API KEY
url = 'http://www.tuling123.com/openapi/api'

req_info = u'你叫什么？/'.encode('utf-8')


query = {'key': KEY, 'info': req_info}
headers = {'Content-type': 'text/html', 'charset': 'utf-8'}


r = requests.get(url, params=query, headers=headers)
res = r.text
print(json.loads(res).get('text').replace('<br>', '\n'))

