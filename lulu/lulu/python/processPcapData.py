__author__ = 'Administrator'
#coding=utf-8

import binascii
from parseData.ParseAMF0 import *
from linkAMQ.ConnectAMQ import *

def send_to_amq(data):
    print "\nin python function [send_to_amq] : "

    #得到字符串的字节码数组
    #bytes = bytearray(data)
    #for byte in bytes:
        #print byte;

    hex_data = binascii.b2a_hex(data)
    hex_data_in_space = " ".join([x+y for x,y in zip(hex_data[::2], hex_data[1::2])])

    # print data
    # print hex_data
    # print hex_data_in_space
    praseAmf0 = ParseAMF0()
    pdata = praseAmf0.prase(hex_data)

    connectAMQ = ConnectAMQSinleton.getInstance()
    connectAMQ.send(pdata)

    return 0

def connect_amq(hostname='192.168.1.166', port=61613):
    connectAMQ = ConnectAMQSinleton.getInstance()
    connectAMQ.connect(hostname, port)

def connect_send(data, hostname='192.168.1.166', port=61613):
    print "\nin python function [connect_send] : "

    hex_data = binascii.b2a_hex(data)
    praseAmf0 = ParseAMF0()
    pdata = praseAmf0.prase(hex_data)

    connectAMQ = ConnectAMQ()
    connectAMQ.connect(hostname, 61613)
    connectAMQ.send(pdata)
    connectAMQ.disConnect()
    print "send ok!"
    return 0

#connect_amq()
#send_to_amq("hello world")

#connect_send("hi")