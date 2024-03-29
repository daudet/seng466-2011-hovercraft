'''
UARTCLI.py
@author: River Allen
@date: July 12, 2010

Universal Asynchronous Receive/Transmit Command Line Interface (UARTCLI)
'''

import cmd
import threading

class UARTCLI(cmd.Cmd, threading.Thread):
	'''
	UART Command Line Interface.
	'''
	def __init__(self):
		cmd.Cmd.__init__(self)
		threading.Thread.__init__(self)
		self.intro = 'Type "help" for additional information...'
		self.prompt = '>> '
		self._commands = []
		self.quit = False

	def run(self):
		'''
		Overwriting the built-in Thread function. This is essentially the main
		of the thread that initiates after calling self.start() (another Thread() built-in).
		'''
		print 'CLI thread starting...'
		self.cmdloop()
		
	def get_commmands(self):
		'''
		Retrieves list of all VALID commands that have been processed since last called.
		'''
		ret_data = self._commands
		self._commands = []
		return ret_data
		
	
	def help_help(self):
		self.do_help('')
		
	def help_exit(self):
		print 'syntax: <exit|quit|bye>',
		print '-- Close main and input window.'
		
	def do_exit(self, args):
		self.quit = True
		return True
		#sys.exit(1)

	def help_nothing(self):
		print 'syntax: nothing'
		print '-- Does absolutely nothing.'
	
	def do_nothing(self, args):
		pass
	
	
	def run_cmd(self, cmds):
		'''
		The goal of this method is to allow a separate thread the ability to run commands 
		through the CLI as if it were done by a user. This is great for batching and
		allows for easier creation of systems that can be both automated or user controlled.
		
		@param cmds: the command or commands to be added then run by the CLI. If a str is passed,
		it is interpreted as a single command. If a list is passed, the list is assumed to
		be a list of commands.
		@type cmds: str or list of str 
		'''
		if type(cmds) is str:
			cmds = [cmds]
		elif type(cmds) is list:
			pass
		else:
			raise TypeError, 'cmd is not list or str.'
		
		self.cmdqueue.extend(cmds)
		print 'Before flush', self.cmdqueue
		#self.stdin.flush()
		#print 'After flush', self.cmdqueue
		#self.stdin.write('nothing\n')
	
	# Shortcuts
	do_quit = do_exit
	help_quit = help_exit
	do_bye = do_exit
	help_bye = help_exit
	do_EOF = do_exit
	help_EOF = help_exit


def main():
	try:
		CLI = UARTCLI()
		CLI.cmdloop()
	except KeyboardInterrupt:
		CLI.do_exit('')	
	except:
		raise
	
if __name__ == "__main__":
	main();