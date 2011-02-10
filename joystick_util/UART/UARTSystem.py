'''
UARTSystem.py
@author: River Allen
@date: July 12, 2010

#------------------------------------------------------------------------------ 
@todo: Add some in-batching
@todo: Add some sort of 'auto' mode
@todo: --simulation mode (be able to run without actually having the physical roomba 
		present
@todo: Attempt to deal with read in uart data that has been split up (this will probably have to
		be done by implementation)
@todo: General Housekeeping and cleaning up (everywhere)
@todo: Fix up everything to do with out-batching (file names...etc)
#------------------------------------------------------------------------------ 
'''

import logging
import serial
import threading
import time

import UARTCLI

#Based off code from:
#http://eli.thegreenplace.net/2009/07/30/setting-up-python-to-work-with-the-serial-port/

global uart_lock
uart_lock = None

class UART(threading.Thread):
	def __init__(self, serial_port, baud_rate, take_input=False, cli=None, 
				simulation=False, log=None, approve=False, debug=False):
		'''
		Read/log data from UART, open an input window, and simulate having UART 
		(not fully implemented).
		
		@param serial_port: The number of the port used by UART.
		@type serial_port: int or str
		
		@param baud_rate: The baud_rate for the UART connection.
		@type baud_rate: int
		
		@param take_input: Flag to set whether or CLI should is used. When True, a CLI prompt is displayed
		and input is read in. When False, essentially acts as an easy configurable Realterm.
		False by default.
		@type take_input: bool
		
		@param cli: UARTCLI implementation. If None, will use the default implementation. See @todo:, for
		more details. None by default.
		@type cli: UARTCLI
		
		@param simulation: NOT IMPLEMENTED. The idea is to be able to run this without having
		a COM port open and simply read from file. (Possible
		@type simulation: bool
		
		@param log: The logger used by UART and UARTOutput. If None, uses 'logging' and assumes 
		a global logging has been setup. None by default.
		@type log: Logger
		
		@param approve: Only write when data has been added to write [add_data_to_write()]. Poor 
		name, but it was the only one I could think of at the time.
		@type approve: bool
		'''
		super(UART, self).__init__()
		self.simulation = simulation
		self.serial_port = serial_port
		self.baud_rate = baud_rate
		self.take_input = take_input
		self._debug = debug
		
		# Locks -- Needed if separate threads are accessing data.
		self._input_lock = threading.Lock()
		self._output_lock = threading.Lock()
		self._write_lock = threading.Lock()
		
		self.quit = False
		
		# Assume global logging has been setup
		if log is None:
			self.log = logging
		else:
			self.log = log
		
		if not self.simulation:
			# Attempt to open the COM port
			try:
				self.ser = serial.Serial(self.serial_port, self.baud_rate, timeout=0)
			except:
				self.log.exception( 'Cannot open Port: "%s"' %str(self.serial_port) )
				self.quit = True
				raise
			self.uo = UARTOutput(self.ser, debug=self._debug)
		else:
			raise NotImplemented, "Simulation is not implemented."
		
		if self.take_input:
			if cli is None:
				self.ui = UARTCLI.UARTCLI()
			else:
				self.ui = cli
	
		self._output_data_buffer = []
		self._input_data_buffer = []
		
		self.approve = approve
		self._write_data_buffer = []
		
	def get_output_data(self):
		'''
		Retrieves received UART data since last called.
		'''
		with self._output_lock:
			r_data = self._output_data_buffer
			self._output_data_buffer = []
			return r_data
	
	def set_output_data(self, dat):
		'''
		
		'''
		with self._output_lock:
			if type(dat) == type(list):
				self._output_data_buffer.extend(dat)
			else:
				self._output_data_buffer.append(dat)
	
	def get_input_data(self):
		'''
		Retrieves all input commands since last called.
		'''
		with self._input_lock:
			r_data = self._input_data_buffer
			self._input_data_buffer = []
			return r_data
	
	def set_input_data(self, dat):
		'''
		Locks, writes to input buffer, then unlocks.
		'''
		with self._input_lock:
			if type(dat) == type(list):
				self._input_data_buffer.extend(dat)
			else:
				self._input_data_buffer.append(dat)

	def add_data_to_write(self, cmds):
		with self._write_lock:
			self._write_data_buffer.extend(cmds)

	def exit(self):
		'''
		Safely closes all threads and then closes serial port.
		'''
		self.uo.quit = True
		if self.uo.isAlive():
			self.uo.join(100) # Wait for it to finish
			if self.uo.isAlive():
				raise RuntimeError, 'The UART output thread is not dieing...'
		
		if self.take_input:
			self.quit = True
			if self.ui.isAlive():
				self.ui.join(100)
				if self.ui.isAlive():
					raise RuntimeError, 'The UART CLI thread is not dieing...'
				
		self._close_serial()
	
	def _close_serial(self):			
		'''
		Attempt to close the serial connection.
		'''
		if self.ser is not None:
			try:
				self.ser.close()
			except:
				pass

	def run(self):
		'''
		- Start uartOut and CLI
		- Log data from uartOut
		- write any data from CLI
		'''
		self.uo.start()
		if self.take_input:
			self.ui.start()
		
		self.lock = threading.Lock()
		try:
			while not self.quit:
				if self.uo.quit or (self.take_input and self.ui.quit):
					print 'uo or ui Quit, so Quitting...'
					self.quit = True
					break
				
				# poll uartOut
				self.set_output_data(self.uo.get_data())
				
				if self.take_input:
					# poll for new data from CLI
					uin_data = self.ui.get_commmands()
					self.set_input_data(uin_data)
					if not self.approve:
						for uin in uin_data:
							self.ser.write(uin)
							if self._debug:
								print 'SE:', uin
							
					
				if self.approve:
					if len(self._write_data_buffer) > 0:
						with self._write_lock:
							if self._debug:
								print 'SE:', self._write_data_buffer[0]
							self.ser.write(self._write_data_buffer.pop(0))
				time.sleep(0.1)
		except:
			raise	
		finally:
			self.exit()
		print "EXITING..."

class UARTOutput(threading.Thread):
	'''
	@todo: May want to move this to its own file, but can only be used by UART in its current
	form...
	'''
	def __init__(self, ser, newline='\r\n', debug=False):
		'''
		@param ser: Open and ready to go serial.
		@type ser: serial.Serial
		
		@param newline: String for when a line should be read. By default, '\r\n'.
		@type newline: str
		'''
		threading.Thread.__init__(self)
		self._debug = debug
		self.quit = False
		
		# Rewrite the batch log file
		# @todo: Make this possible to set, but use date+time by default.
		
		self.ser = ser
		
		# Log initial information.
		if self._debug:
			logging.info( '-' * 50 )
			logging.info( 'Starting...' )
			logging.info( 'Device:\t\t\t' + str(self.ser.name) )
			logging.info( 'Baud Rate:\t\t' + str(self.ser.baudrate) )
			print "Hit 'Ctrl + C' or type 'exit' in the input window to exit..."
			logging.info( '-' * 50 )
			
		self._rec_lock = threading.Lock()
		self._received_data = []

		
	def exit(self):
		'''
		
		'''
		self.quit = True
		#sys.exit()
		
	def get_data(self):
		with self._rec_lock:
			ret_data = self._received_data
			self._received_data = []
			return ret_data
	
	def run(self):
		'''
		
		'''
		#print 'UartOutput thread starting...'
		try:
			temp_buffer = ''
			newline = '\r\n'
			while not self.quit:
				val = self.ser.read(999)
				if len(val) > 0:
					temp_buffer += val

					# Need to do this as incomplete lines can be read.
					# In addition, may read complete line + incomplete line
					# or two complete lines.
					if newline in temp_buffer:
						# Get the index of the point where to cut data+newline
						n_index = temp_buffer.index(newline) + len(newline)
						# Store/Log new data
						newest_data = temp_buffer[:n_index]
						self._received_data.append(newest_data)
						if self._debug:
							logging.info(newest_data)
						temp_buffer = temp_buffer[n_index:]
					else:
						pass
				
				time.sleep(0.01)

			self.exit()
		except KeyboardInterrupt:
			self.exit()
			#self.quit = True



if __name__ == "__main__":
	execfile("..\globalConfig.py")
	logging.basicConfig(level=logging.DEBUG,
		format=logFormat,
		datefmt=logDateFormat,
		filename=logFilename,
		filemode=logFileMode)
	console = logging.StreamHandler()
	log = logging.getLogger()
	log.addHandler(console)
	ua = UART(serialPort, baudRate, take_input=True)
	ua.start()
	
	