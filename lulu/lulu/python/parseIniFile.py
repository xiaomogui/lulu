__author__ = 'Administrator'
import ConfigParser

def getValue(ini_file_path, section, key):
	with open(ini_file_path, "rb") as fp:
		config = ConfigParser.ConfigParser()
		config.readfp(fp)
		return config.get(section, key)