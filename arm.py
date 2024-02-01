import serial
import time
import serial.tools.list_ports

class Arm():
    """Class used to send messages to the robotic arm 🦾"""
    def __init__(self, port:str) -> None:
        """Main module used to controll the arm"""
        self.__arduino = serial.Serial(port=port, baudrate=2000000, timeout=.1)
        
        # Find board name
        self.name = b''
        while(self.name == b''):
            self.message("w")
            self.name = self.__arduino.readline()
        self.name = self.name.decode()[:-2]
    
    def message(self, data:str) -> None:
        """Method used to send raw and unverified messages to arm.\n
        ⛔Do not use unless you 100% know what you are doing⛔""" 
        
        self.__arduino.write(bytes(data+"\r\n", 'utf-8'))
        
    def move_joint(self, steps:list) -> None:
        """Method used to move each joint by a fixed number of steps\n
        For example, to move joint I 3 steps and joint III 10 steps use:
        [3, 0, 10]"""
        
        toAdd = ""
        for i in steps:
            toAdd += str(i) + " "
        
        self.__arduino.readline()
        self.message(toAdd.strip())
        
        
        while(self.__arduino.readline() != b'DONE\r\n'):
            time.sleep(.1)
        
    def reset_position(self) -> None:
        """Rotate motors untill all of them reach a knwon position."""
        
        self.message("z")
        
        while(self.__arduino.readline() != b'DONE\r\n'):
            time.sleep(.1)

class Ports():
    """General utilities. 🔧"""
    def __init__(self) -> None:
        pass
    
    def list() -> list[str]:
        """List all available ports on this machine"""
        ports = []
        for port in serial.tools.list_ports.comports():
            ports.append(port.name)
        
        return ports