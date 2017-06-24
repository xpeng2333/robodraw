# -*- coding:utf8 -*-
import urllib.request
if __name__ == '__main__':
    url_get_base = "http://ltpapi.voicecloud.cn/analysis/?"
    api_key = 'Z1O4D7P5c5p4p7x2I677SrKabY4VjIwCkoufiZDT'
    text = '帮我画一幅素描吧'
    format = 'json'
    pattern = 'all'
    result = urllib.request.urlopen("%sapi_key=%s&text=%s&format=%s&pattern=%s" % (
        url_get_base, api_key, text, format, pattern))
    content = result.read().strip()
    print(content)
