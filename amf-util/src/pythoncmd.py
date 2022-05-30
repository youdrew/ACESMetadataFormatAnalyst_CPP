def run_cmd_Popen_fileno(cmd_string):
    import subprocess
    print('运行cmd指令：{}'.format(cmd_string))
    return subprocess.Popen(cmd_string, shell=True, stdout=None, stderr=None).wait()




if __name__ == '__main__':
    run_cmd_Popen_fileno('python3 /Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/amf-util/amf-util.py render /Users/youdrew/QT_CPP/ACESMetadataFormatAnalyst_qmake/TestFile/LogCEI800-Rec.709100nitsdim.amf /Users/youdrew/Documents/AMF/Test_file/aces-dev/transforms/ctl')