#for this code I drew from https://stackoverflow.com/questions/60607207/trying-to-set-up-a-server-to-receive-osc-input-keep-getting-this-error-when-i-r
# The timestamp and saving the file was implemented with help of chatGPT;
# I used this documentation library https://docs.python.org/3.11/library/stdtypes.html?highlight=fsync%20flush#file.close in general for reference, especially the bit from line 47
# I also used chatGPT for debugging;

import csv
import os
import datetime
from pythonosc import udp_client, dispatcher, osc_server

# OSC server setup  
serverIP = '172.20.10.3'#'192.168.8.102' #'192.168.1.146' # replace with the ESP32's IP address
serverPort = 6574
serverAddr = (serverIP, serverPort)
print(serverIP)

count = 1
while os.path.exists(f"saveMe_{count}.csv"):
    count += 1
    
# This creates a new CSV file; every time I run the code it happens anew
timestamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
filename = "saveMe_{}.csv".format(timestamp) #each file is identified by when  the code was run 
filepath = os.path.join("/Users/juliecst/Desktop/final project TD", filename)

with open(filepath, 'w', newline='') as file:
  writer = csv.writer(file)
  writer.writerow(['Squeeze'])
  
  # This is the function to take care of new OSC messages similar to Beccys documentation in Javascript
  def handle_OSC_message(address, squeeze):
    print(f"Received OSC message: {address} {squeeze}")
    writer.writerow([timestamp, squeeze])
    
  try:
    # This starts the OSC and is from the mentioned stack overflow
    dispatcher = dispatcher.Dispatcher()
    dispatcher.map("/sensorValue", handle_OSC_message) # this directs the sensorValue to the handle_OSC_message function
    server = osc_server.ThreadingOSCUDPServer(serverAddr, dispatcher)
    print(f"OSC server listening on {serverIP}:{serverPort}")
    server.serve_forever()
    
  #stop everything when ctrl+c is pressed in the terminal
  except KeyboardInterrupt:
    pass
      
  # This ends the writing and saves the CSV file
  file.flush()
  os.fsync(file.fileno())
  file.close()
