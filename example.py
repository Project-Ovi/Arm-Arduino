"""#################################
#                                  #
#      This is an example of       #
#       how to use this lib        #
#                                  # 
"""#################################
from arm import Arm, Ports
from sys import exit

# Get a list of all the ports
available_ports = Ports.list()
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

# Move the arm to position zero
my_arm.reset_position()
print("I reset all of the motor positions!")

# Move each motor a bunch of steps
my_arm.move_joint(steps=[4,3,2,1])
print("I moved the arm a bunch of steps!")

# Exit
print("I managed to execute all commands!")
exit(0)