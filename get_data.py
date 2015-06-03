import os
import subprocess
import psutil
import time

class UsageStats:
	def __init__(self):
		self.last_time_recorded = int(time.time()*1000)
		self.last_bytes_sent = 0
		self.last_bytes_received = 0
		
	def getCpuUsage(self):
		return psutil.cpu_percent(interval=1)
		
	def getRamUsage(self):
		return psutil.virtual_memory()[2]

	def getDiskUsage(self):
		return psutil.disk_usage("/")[3]

	def getNetSpeed(self):
		counters = psutil.net_io_counters()
		cur_time = int(time.time()*1000)
		bytes_sent = counters[0] - self.last_bytes_sent
		up_speed = bytes_sent*1000/(cur_time - self.last_time_recorded)
		bytes_received = counters[1] - self.last_bytes_received
		down_speed = bytes_received*1000/(cur_time - self.last_time_recorded)
		
		self.last_time_recorded = cur_time
		self.last_bytes_sent = counters[0]
		self.last_bytes_received = counters[1]
		return [up_speed, down_speed]