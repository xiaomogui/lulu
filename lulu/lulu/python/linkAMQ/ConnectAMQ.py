__author__ = 'Administrator'
#coding=utf-8

import threading
from stompy.simple import Client

# 单例模式
class ConnectAMQSinleton(object):
    __instance = None
    __lock = threading.Lock() # used to synchronize code

    __stompClient = None
    # 默认订阅amf_pcap_dataamf_pcap_data队列
    __subscribe = '/queue/amf_pcap_data'

    def __init__(self):
        'disable the __init__ method'

    def __new__(cls):
        return object.__new__(cls)

    @staticmethod
    def getInstance():
        if not ConnectAMQSinleton.__instance:
            ConnectAMQSinleton.__lock.acquire(0)
            if not ConnectAMQSinleton.__instance:
                ConnectAMQSinleton.__instance = object.__new__(ConnectAMQSinleton)
                object.__init__(ConnectAMQSinleton.__instance)
            ConnectAMQSinleton.__lock.release()
        return ConnectAMQSinleton.__instance

    def connect(self, hostname='192.168.1.166', port=61613):
        # 通过simple方式连接JMS服务器
        # 指定hostname和port（tips:ActiveMQ支持多种协议连接stomp协议的默认端口为61613，这里不要写为61616）
        ConnectAMQSinleton.__stompClient = Client(hostname, port)
        #stomp = Client()#如果是ActiveMQ和ActiveMQ客户端（本程序）在同一台机器可使用默认值：hostname="localhost",port=61613

        # 连接服务器
        ConnectAMQSinleton.__stompClient.connect()

    def disConnect(self):
        if ConnectAMQSinleton.__stompClient != None:
            # 退订
            ConnectAMQSinleton.__stompClient.unsubscribe(ConnectAMQSinleton.__subscribe)
            # 关闭连接
            ConnectAMQSinleton.__stompClient.disconnect()

    def send(self, data):
        # 发送消息到指定的queue
        ConnectAMQSinleton.__stompClient.put(data, destination=ConnectAMQSinleton.__subscribe)

    def receive(self):
        # 从指定的queue订阅消息。ack参数指定为"client",不然可能出现一个问题（具体忘了，以后补充）,ack默认值为"auto"
        ConnectAMQSinleton.__stompClient.subscribe(ConnectAMQSinleton.__subscribe,ack="client")
        # 等待接收ActiveMQ推送的消息
        message = ConnectAMQSinleton.__stompClient.get()
        # 消息的主体
        receiveData =  message.body
        ConnectAMQSinleton.__stompClient.ack(message)
        return receiveData

    # get方法
    def getSubscribe(self):
        return ConnectAMQSinleton.__subscribe

    # set方法
    def setSubscribe(self, subscribe):
        ConnectAMQSinleton.__subscribe = subscribe

class ConnectAMQ:
    __stompClient = None
    # 默认订阅amf_pcap_dataamf_pcap_data队列
    __subscribe = '/queue/amf_pcap_data'

    def __init__(self):
        'init'

    def connect(self, hostname='192.168.1.166', port=61613):
        # 通过simple方式连接JMS服务器
        # 指定hostname和port（tips:ActiveMQ支持多种协议连接stomp协议的默认端口为61613，这里不要写为61616）
        ConnectAMQ.__stompClient = Client(hostname, port)
        #stomp = Client()#如果是ActiveMQ和ActiveMQ客户端（本程序）在同一台机器可使用默认值：hostname="localhost",port=61613

        # 连接服务器
        ConnectAMQ.__stompClient.connect()

    def disConnect(self):
        if ConnectAMQ.__stompClient != None:
            # 退订
            ConnectAMQ.__stompClient.unsubscribe(ConnectAMQ.__subscribe)
            # 关闭连接
            ConnectAMQ.__stompClient.disconnect()

    def send(self, data):
        # 发送消息到指定的queue
        ConnectAMQ.__stompClient.put(data, destination=ConnectAMQ.__subscribe)

    def receive(self):
        # 从指定的queue订阅消息。ack参数指定为"client",不然可能出现一个问题（具体忘了，以后补充）,ack默认值为"auto"
        ConnectAMQ.__stompClient.subscribe(ConnectAMQ.__subscribe,ack="client")
        # 等待接收ActiveMQ推送的消息
        message = ConnectAMQ.__stompClient.get()
        # 消息的主体
        receiveData =  message.body
        ConnectAMQ.__stompClient.ack(message)
        return receiveData

    # get方法
    def getSubscribe(self):
        return ConnectAMQ.__subscribe

    # set方法
    def setSubscribe(self, subscribe):
        ConnectAMQ.__subscribe = subscribe