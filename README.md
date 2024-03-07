# Arm-Arduino
Arduino code for controlling the stepper motors in the arm.

# Example
```python
from arm import Arm, Ports
from sys import exit

# Get a list of all the ports
available_ports = Ports.list()
if len(available_ports) <= 0:
    print("No ports are available! Please connect the arm")
    exit(1)
print("Wich port would you like to use?")
for i in range(len(available_ports)):
    print(i+1, available_ports[i])
port = available_ports[int(input("Index: "))-1]

# Connect to arm
try:
    my_arm = Arm(port)
except:
    print("Failed to establish connection!")
    exit(1)

# Print the name of the arm
print("I connected to an arm named: ", my_arm.name)

# Order slave 1 to move its joint 5 steps with a uptime of 50ms and a downtime of 500ms
# And order slave 2 to move its joint 15 steps with a uptime of 20ms and a downtime of 5ms
my_arm.slave_command(["5:50:500", "15:20:5"])
print("Executed first move command")

# Order slave 4 to move its joint 100 steps with a uptime and downtime of 10ms
my_arm(["", "", "", "100:10:10"])
print("Executed second move command")

# Done
print("Program finnished execution")
exit(0)
```
