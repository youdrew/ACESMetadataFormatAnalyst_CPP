#!/usr/bin/env python
# coding=utf-8


def run_cmd_Popen_fileno(cmd_string):
    import subprocess
    print('运行cmd指令：{}'.format(cmd_string))
    return subprocess.Popen(cmd_string, shell=True, stdout=None, stderr=None).wait()
