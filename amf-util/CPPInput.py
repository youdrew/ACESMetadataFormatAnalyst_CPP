import os
import sys
import re

pypath='/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/amf-util/amf-util.py'

def info(filepath):
    cmd='python3 '+pypath+' info '+filepath
    return os.system(cmd)




def render(filepath,ctlparh):
    cmd='python3 '+pypath+' render '+filepath+' '+ctlparh
    print(cmd)
    print(sys.path)
    return 0