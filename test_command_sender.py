import serial
import time
import random
import get_data


ser = serial.Serial(2)
ser.baudrate =9600
ser.timeout = 2
time.sleep(2)
#TFT Colors
# Green 2016
# Red 63488

stats = get_data.UsageStats()

ser.write("c\n");
ser.flush()
while True:

	#ser.write("s\n10\n10\n65000\nCpu Usage:\n")
	#ser.flush()
	ser.write("s\n10\n10\n20\n65000\nCpu Usage:\n")
	ser.write("b\n10\n20\n50\n%d\n2016\n" % int(stats.getCpuUsage()*0.5))
	#ser.flush()
	ser.write("s\n10\n30\n20\n65000\nMemory Usage:\n")
	#ser.flush()
	ser.write("b\n10\n40\n50\n%d\n2016\n" % int(stats.getRamUsage()*0.5))
	
	
	net_speed = stats.getNetSpeed()
	
	ser.write("s\n10\n50\n20\n65000\nNetwork:\n")
	ser.write("b\n10\n60\n50\n%d\n2016\n" % int(net_speed[0]*50/500000))
	ser.write("s\n60\n60\n30\n65000\n%dKBps Up\n"% int(net_speed[0]/1024))
	ser.write("b\n10\n70\n50\n%d\n2016\n" % int(net_speed[1]*50/1000000))
	ser.write("s\n60\n70\n30\n65000\n%dKBps Down\n"% int(net_speed[1]/1024))
	#ser.write("s\n10\n60\n60\n65000\n%dKBps Up : %dKBps Down:\n"%(int(net_speed[0]/1024), int(net_speed[1]/1024)))
	#print net_speed
	ser.flush()
	#print ser.read(500)
	time.sleep(2)

ser.write("c\nb\n12\n40\n50\n30\n65535\n")
while True:
	ser.write("c\nb\n12\n20\n100\n%d\n%d\n" % (int(random.random()*100), int(random.random()*65000)))
	ser.write("b\n12\n40\n100\n%d\n%d\n" % (int(random.random()*100), int(random.random()*65000)))
	ser.write("b\n12\n60\n100\n%d\n%d\n" % (int(random.random()*100), int(random.random()*65000)))
	ser.write("b\n12\n80\n100\n%d\n%d\n" % (int(random.random()*100), int(random.random()*65000)))
	ser.write("b\n12\n100\n100\n%d\n%d\n" % (int(random.random()*100), int(random.random()*65000)))
	#print "c\nb\n12\n40\n50\n%d\n65535\n" % random.random()*50
	time.sleep(2)
print ser.readline()
time.sleep(10)
