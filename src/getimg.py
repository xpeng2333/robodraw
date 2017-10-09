#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
import re
import urllib
import json
import socket
import sys
import urllib.request
import urllib.parse
import urllib.error
# 设置超时
import time

timeout = 5
socket.setdefaulttimeout(timeout)


class Crawler:
    # 睡眠时长
    __time_sleep = 0.1
    __amount = 0
    __start_amount = 0
    __counter = 0
    headers = {
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11'}

    # 获取图片url内容等
    # t 下载图片时间间隔
    def __init__(self, t=0.1):
        self.time_sleep = t

    # 保存图片
    def __save_image(self, rsp_data, word):

        if not os.path.exists("../data/img"):
            os.mkdir("../data/img")
        # 判断名字是否重复，获取图片长度
        for image_info in rsp_data['imgs']:
            try:
                time.sleep(self.time_sleep)
                fix = self.__get_suffix(image_info['objURL'])
                urllib.request.urlretrieve(
                    image_info['objURL'], '../data/img/picture_carton.jpg')
            except urllib.error.HTTPError as urllib_err:
                print(urllib_err)
                continue
            except Exception as err:
                #                time.sleep(1)
                print(err)
                print("产生未知错误，放弃保存")
                continue
            else:
                break
        return

    # 获取后缀名
    @staticmethod
    def __get_suffix(name):
        m = re.search(r'\.[^\.]*$', name)
        if m.group(0) and len(m.group(0)) <= 5:
            return m.group(0)
        else:
            return '.jpg'

    # 获取前缀
    @staticmethod
    def __get_prefix(name):
        return name[:name.find('.')]

    # 开始获取
    def __get_images(self, word='大象'):
        search = urllib.parse.quote(word)

        url = 'http://image.baidu.com/search/avatarjson?tn=resultjsonavatarnew&ie=utf-8&word=' + search + \
            '&cg=girl&pn=' + \
            str(0) + '&rn=60&itg=0&z=0&fr=&width=&height=&lm=-1&ic=0&s=0&st=-1&gsm=1e0000001e'
        # 设置header防ban
        try:
            time.sleep(self.time_sleep)
            req = urllib.request.Request(url=url, headers=self.headers)
            page = urllib.request.urlopen(req)
            rsp = page.read().decode('unicode_escape')
        except UnicodeDecodeError as e:
            print(e)
            print('-----UnicodeDecodeErrorurl:', url)
        except urllib.error.URLError as e:
            print(e)
            print("-----urlErrorurl:", url)
        except socket.timeout as e:
            print(e)
            print("-----socket timout:", url)
        else:
            # 解析json
            rsp_data = json.loads(rsp)
            self.__save_image(rsp_data, word)
        finally:
            page.close()
        print("下载任务结束")
        return

    def start(self, word):
        self.__get_images(word)


def main(argv):
    crawler = Crawler(0.05)
    crawler.start(argv[1] + '简笔画')


if __name__ == '__main__':
    main(sys.argv)
