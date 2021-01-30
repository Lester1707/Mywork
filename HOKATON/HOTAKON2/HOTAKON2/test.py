import time
import os

list_name = os.listdir("File")
start_time = time.clock()
for i in range(len(list_name)):
	os.system('python main.py {}'.format(list_name[i]))
	print(time.clock() - start_time)
print('End time {}'.format(time.clock() - start_time))