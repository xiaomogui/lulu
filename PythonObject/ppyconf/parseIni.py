__author__ = 'Administrator'
#coding=utf-8

partLable = ("<",">")
sectionLable = ("[","]")
endlineLable = "\r\n"# windows下的行标志
#endlineLable = "\n"# linux下的行标志
equalLable = "="# 赋值标志
noteLable = '#'# 注释标志

# 得到总配置的map
def getPlatformMap(strtmp,lable1 = partLable,lable2 = sectionLable):
    tmp = strtmp.split(lable1[0])
    tmp = [elem for elem in tmp if len(elem) > 1]
    tmp = [elem for elem in tmp if elem.rfind(lable1[1]) > 0]
    platdict = {}
    for elem in tmp:
        key = elem[0:elem.find(lable1[1]):]
        value = elem[elem.find(lable2[0])::]
        platdict[key] = value
    return platdict

# 得到各部分的map
def getSectionMap(strtmp,lable1 = sectionLable):
    tmp = strtmp.split(lable1[0])
    tmp = [elem for elem in tmp if len(elem) > 1]
    tmp = [elem for elem in tmp if elem.rfind(lable1[1]) > 0]
    sectionDict = {}
    for elem in tmp:
        key = elem[0:elem.find(lable1[1]):]
        value = elem[elem.find(endlineLable)+len(endlineLable)::]
        sectionDict[key] = value
    return sectionDict

# 获取具体配置值
def getValueMap(strtmp):
    tmp = strtmp.split(endlineLable)
    tmp = [elem for elem in tmp if len(elem) > 1]
    valueDict = {}
    for elem in tmp:
        if elem.find(noteLable) > 0: # 如果有注释则去掉注释
            elem = elem[0:elem.find(noteLable):]
        elem = ''.join(elem.split()) # 去掉空白字符
        key = elem[0:elem.find(equalLable):]
        value = elem[elem.find(equalLable)+len(equalLable)::]
        valueDict[key] = value
    return valueDict
