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
        
    def slave_command(self, commands:list[str]) -> None:
        """Method used to send commands to each slave individually\n
        The index of the command in the array defines the id of the slave wich should receive the command"""
        toSend = ">"
        for i in commands:
            toSend += i
            if i != commands[-1]:
                toSend += "|"
        toSend += "<"
        
        self.message(toSend)

    def reset_position():
        """⚠️Function removed as of later versions⚠️"""
        raise NotImplementedError("Function reset_position() removed as of later updates")
    
    def move_joint(self, commands:list[str]):
        """As of later updates, this function just calls slave_command()\n
        ⚠️Please use slave_command()⚠️"""
        self.slave_command(commands=commands)


       

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