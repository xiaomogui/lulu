__author__ = 'Administrator'
#coding:utf-8

from parseIni import *
from parseIniFile import *

def test0():
    # 一般写法
    # f = open("0.ini","rb")
    # print getValue(f,"global","port");
    # f.close()

    # 自动管理close,try catch [with open as 从python2.5引入(需要通过 from __future__ import with_statement 导入后才可以使用),从 2.6 版本开始缺省可用]
    with open("0.ini","rb") as f:
        print getValue(f,"global","port");

def test1():
    # 一般写法
    # f = open("1.ini","rb")
    # strFileContent = f.read()
    # f.close()

    # 自动管理close,try catch [with open as 从python2.5引入(需要通过 from __future__ import with_statement 导入后才可以使用),从 2.6 版本开始缺省可用]
    with open("1.ini","rb") as f:
        strFileContent = f.read()

    vardict = {}
    var1 = getPlatformMap(strFileContent)
    for k,v in var1.items():
        var2 = getSectionMap(v)
        dict3 = {}
        for k2,v2 in var2.items():
            var3 = getValueMap(v2)
            dict3[k2] = var3
        vardict[k] = dict3

    print vardict["part2"]["global"]["ip"]

test0();
test1();