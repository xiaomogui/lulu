__author__ = 'Administrator'
import ConfigParser

def getValue(fp, section, key):
    config = ConfigParser.ConfigParser()
    config.readfp(fp)
    return config.get(section, key)