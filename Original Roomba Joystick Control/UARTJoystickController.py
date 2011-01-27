'''
UARTJoystickController.py
@author: River Allen
@date: July 15, 2010

'''

import serial
import pygame
import struct
import time
import logging
import threading
from UART import UARTSystem

class UARTJoystickController(threading.Thread):
    def __init__(self, debug=True):
        '''
        Built for the Average Logitech USB (PS2) controller.
        
        Buttons:
        (Note: these numbers are num-1 in reality...i.e. button 1 is get_button(0))
         * 1 - 10 are labelled.
         * 11 - left axes button.
         * 12 - left axes button.
        Axes:
         * 0 - Left Stick X-axis (-ve: Left, +ve: Right)
         * 1 - Left Stick Y-axis (-ve: Up, +ve: Down)
         * 2 - Right Stick X-axis (-ve: Left, +ve: Right)
         * 3 - Right Stick Y-axis (-ve: Up, +ve: Down)
        Hat(s):
         * This is the D-Pad.
        '''
        threading.Thread.__init__(self)
        self.quit = False
        self._lock = threading.Lock()
        
        self.struct_fmt = 'b' * 18 + '\r\n' # 12 buttons + 2 (xyaxis1) + 2 (xyaxis2) + 2 (hat)
        self._debug = debug
        pygame.init()
        self.joy = pygame.joystick.Joystick(0)
        self.joy.init()
        if debug:
            self.log = logging
            self.log.info('-' * 50)
            self.log.info(str(self.joy.get_name()))
            self.log.info('-' * 50)
    
        self._packets = []
    
    def get_commmands(self):
        '''
        Return the latest packet(s) since last called.
        
        Poor name, but it allows us to pretend the joystick is a CLI...and thus interface
        with UARTSystem without touching anything.
        '''
        # Lock to ensure we do not mess with packets while another thread reads
        with self._lock:
            ret_data = self._packets
            self._packets = []
            return ret_data
    
    def run(self):
        try:
            button_history = [0,0,0,0,0,0,0,0,0,0,0,0]
            
            while not self.quit:
                pygame.event.pump()
                
                # Buttons
                buttons = []
                for i in range(self.joy.get_numbuttons()):
                    button_value = self.joy.get_button(i)
                    # Stop button holding and button tapping timing issues.
                    if button_history[i] and button_value:
                        buttons.append(0)
                    else:
                        buttons.append(button_value)
                        button_history[i] = button_value
                    
                
                # Axes
                axes = []
                for i in range(self.joy.get_numaxes()):
                    axes.append(int(self.joy.get_axis(i)*100)) # Normalize axes to 100 (fit in byte)
                
                # Hat (aka D-Pad)
                hat = list(self.joy.get_hat(0))
                
                all_data = []
                all_data.extend(buttons)
                all_data.extend(axes)
                all_data.extend(hat)
                
                packet = struct.pack(self.struct_fmt, *tuple(all_data))
                # Lock to ensure we do not mess with packets while another thread reads
                with self._lock:
                    self._packets.append(packet)
                
                if self._debug:
                    #self.log.info('Buttons:\t' + str(buttons))
                    self.log.info('Axes\t\t' + str(axes))
                    #self.log.info('Hat:\t\t' + str(hat))
                    self.log.info('-' * 50)
                
                time.sleep(0.15)
        except:
            print 'Crashed...'
            raise

if __name__ == '__main__':
    execfile("globalConfig.py")
    logging.basicConfig(level=logging.DEBUG,
        format=logFormat,
        datefmt=logDateFormat,
        filename=logFilename,
        filemode=logFileMode)
    console = logging.StreamHandler()
    log = logging.getLogger()
    log.addHandler(console)
    
    joy = UARTJoystickController(debug=False)
    #joy.run()
    
    ua = UARTSystem.UART(serialPort, baudRate, take_input=True, cli=joy, approve=False, debug=False)
    ua.start()
