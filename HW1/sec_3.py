import os
import subprocess

def create_random_file():
    subprocess.run(['dd', 'if=/dev/urandom', 'of=testfile', 'bs=1M', 'count=100'])
    print("Random file 'testfile' of size 100MB created.")

def clear_cache():
    subprocess.run(['sync'])
    subprocess.run(['echo', '3', '|', 'sudo', 'tee', '/proc/sys/vm/drop_caches'], shell=True)
    print("System cache cleared.")

def copy_from_random(buf):
    subprocess.run(['time', './copy', 'testfile', 'outputfile', str(buf)])

if __name__ == '__main__':
    create_random_file()
    clear_cache()
    for buf in [1, 64, 512, 1024, 8192, 65536]:
