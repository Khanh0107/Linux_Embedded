#!/usr/bin/python3

import os
import sys
import os.path
import dbus
import subprocess
import re
import getpass
import fileinput


def usage():
	print ('''#****************************************************************************************************************************************
Run this script in super user mode.

Options Usage:

<Selected Option> <Paramaters Required>

-u  Proxy URL string
-n  User Name
-d  Domain Name
-p  Password

ex: setup_proxy.py -u "rb-proxy-apac.bosch.com:8080" -n "sample" -d "APAC" -p "Password"

-r  Option used to reset/delete the Proxy configuration 

#****************************************************************************************************************************************''')

def is_proxy(input): #to check whether the input is in correct url format
	word="http://"
	if word in input:
		return 1
	else:
		return 0

def is_only_alpha(word): #to check whether the input is only alphabets
	if(word.isalpha()):
		return 1
	else:
		return 0

def is_alphanumeric(word): #to check whether the input is alphanumeric
	if(word.isalnum()):
		return 1
	else:
		return 0

def set_cntlm_conf(Username, Domain, Proxy):

	#To update the values of username, domain, proxy and passowrd into the cntlm.conf file

	if (Username=="testuser"):
		if(os.path.exists('/etc/cntlm.conf')):
			with open ('/etc/cntlm.conf', 'r') as in_file, open ('temp.txt', 'w') as out_file:
				for line in in_file:
					if line.startswith ('Username'):
						line = "Username\t" + Username +"\n"
						out_file.write (line)
					elif line.startswith ('Domain'):
						line = "Domain\t" + Domain + "\n"
						out_file.write (line)
					elif line.startswith ('Password'):
						line = "Password\t Password\n"
						out_file.write (line)
					elif line.startswith ('Proxy'):
						line = "Proxy\t" + Proxy +"\n"
						out_file.write (line)
					else:
						out_file.write (line)

			with open ('temp.txt', 'r') as in_file, open ('/etc/cntlm.conf', 'w') as out_file:
				for line in in_file:
					out_file.write(line)
			os.remove('temp.txt')		
			flag=1	
			while (flag==1):
				flag=0
				with open('/etc/cntlm.conf', 'r') as f:
					for line in f:
						if line.startswith('PassLM'):	
							flag=1	        
				if (flag==1):                 
					lines = open('/etc/cntlm.conf', 'r').readlines()
					del lines[-1]
					del lines[-1]
					del lines[-1]
					open('/etc/cntlm.conf', 'w').writelines(lines)
	else:
		global search_index
		#print "USERNAME " + Username
		#print "DOMAIN " + Domain.upper()
		#print "PROXY " + Proxy
		#print "\n"

		#To generate a has for the password and store in the cntlm.conf file
		if len(sys.argv) >= 2:
			search_index=0
			search_options("-p")
			if(search_index == 0):
				passw = getpass.getpass()
			else:
				passw = sys.argv[search_index]
		else:
			passw=getpass.getpass()

		passw=re.escape(passw)
		with open ('/etc/cntlm.conf', 'r') as in_file, open ('temp.txt', 'w') as out_file:
			for line in in_file:
				if line.startswith ('Username'):
					line = "Username\t" + Username + "\n"
					out_file.write (line)
				elif line.startswith ('Domain'):
					line = "Domain\t" + Domain.upper() + "\n"
					out_file.write (line)
				elif line.startswith ('Password'):
					line = "Password\t" + "\n"
					out_file.write (line)
				elif line.startswith ('Proxy'):
					line = "Proxy\t" + Proxy + "\n"
					out_file.write (line)
				else:
					out_file.write (line)

		with open ('temp.txt', 'r') as in_file, open ('/etc/cntlm.conf', 'w') as out_file:
			for line in in_file:
				out_file.write(line)

		os.remove('temp.txt')

		flag=1
		while (flag==1):
			flag=0
			with open('/etc/cntlm.conf', 'r') as f:
				for line in f:
					if line.startswith('PassLM'):	
						flag=1	        
			if (flag==1):                 
				lines = open('/etc/cntlm.conf', 'r').readlines()
				del lines[-1]
				del lines[-1]
				del lines[-1]
				open('/etc/cntlm.conf', 'w').writelines(lines)
			
		if(passw!=None):
			f=open("/tmp/.cntlmpass", "w")
			f.close()

			cmd="cntlm -u "+ Username +" -d "+ Domain + " -p " + passw + " -H < /tmp/.cntlmpass | tail -3 >/tmp/.cntlmhash" 
			os.system(cmd)

			with open('/tmp/.cntlmhash', 'r') as fi:
				val=fi.readlines()
				with open('/etc/cntlm.conf', 'a+') as f1:
					for line in val:
						f1.write(line)
	
	if(os.path.exists('/etc/cntlm.conf')): 
		print("set_cntlm_conf() is a success \n")
		subprocess.call("/etc/init.d/cntlm restart", shell=True)
	else:
		print("set_cntlm_conf() is not a success")

def set_wgetrc(input):

	#To write the http and https proxy into the /etc/wgetrc file for wget function
	
	flag=0
	if(os.path.exists('/etc/wgetrc')):
		with open('/etc/wgetrc', 'r') as f:
			for line in f:
				if line.startswith ('use_proxy=on'):
					flag=1
				else:
					flag=2
		if(input=="reset"):
			if (flag==1):
				lines = open('/etc/wgetrc', 'r').readlines()
				del lines[-1]
				del lines[-1]
				del lines[-1]
				open('/etc/wgetrc', 'w').writelines(lines)       
		else:
			if (flag==1):
				lines = open('/etc/wgetrc', 'r').readlines()
				del lines[-1]
				del lines[-1]
				del lines[-1]
				open('/etc/wgetrc', 'w').writelines(lines)
				
				with open('/etc/wgetrc', 'a+' ) as f: 
					f.write("http_proxy=%s\n" %input)
					f.write("https_proxy=%s\n" %input)
					f.write("use_proxy=on\n") 
					print("set_wgetrc() is a success\n")
					
			if(flag==2):
				with open('/etc/wgetrc', 'a+' ) as f: 
					f.write("http_proxy=%s\n" %input)
					f.write("https_proxy=%s\n" %input)
					f.write("use_proxy=on\n") 
					print("set_wgetrc() is a success\n")
	else:
		print ("wgetrc file not found in /etc dir")
				
def set_env(input):
	
	if(input=="reset"):
		if(os.path.exists('/etc/profile.d/proxy.sh')):
			os.remove("/etc/profile.d/proxy.sh")
	else:
		if(os.path.exists('/etc/profile.d')):
			file=open("/etc/profile.d/proxy.sh", "w")
			os.chmod("/etc/profile.d/proxy.sh", 0o755)
			file.close()
			with open('/etc/profile.d/proxy.sh', 'w') as f:
				
				f.seek(0)
				f.truncate()
				f.write("export http_proxy=%s\n" %input)
				f.write("export https_proxy=%s\n" %input)
				if(os.path.exists('/etc/profile.d/proxy.sh')):
					print("set_env() is a success\n")
				else:
					print("set_env() is not a success\n")
		else:
			print("profile.d dir not found in /etc dir\n")

def set_system_wide_env (input):
	if (os.path.exists('/etc/environment')):
		temp = open("/etc/envtemp","w")
		fd = open ("/etc/environment","r")
		for line in fd:
			i = line.find("http_proxy")
			if i != -1:
				continue
			j = line.find("https_proxy")
			if j != -1:
				continue
			temp.write(line)
		if (input != "reset" and input != "direct"):
			temp.write("http_proxy=%s\n" %input)
			temp.write("https_proxy=%s\n" %input)
		fd.close()
		temp.close()
		file=open("/etc/environment", "w")
		fd = open("/etc/envtemp", "r")
		data = fd.readlines()
		file.writelines(data)
		file.close()
		fd.close()
		cmd = "rm -rf /etc/envtemp"
		os.system(cmd)
	elif (input != "reset" and input != "direct"):
		with open("/etc/environment", "w") as f :
			f.write("http_proxy=%s\n" %input)
			f.write("https_proxy=%s\n" %input)

def set_bashrc(input):

	#To write the http and https proxy into the bashrc file for bash function
	home_dir=os.getenv('HOME', None)
	flag=0
	if(os.path.exists('/etc/bash.bashrc') and os.path.exists(home_dir + '/.bashrc')):

		tmp=0
		with open('/etc/bash.bashrc', 'r') as in_file, open('temp.txt', 'w') as out_file:
			for line in in_file:
				for word in line.split():
					if "http_proxy=" in word:
						tmp=1
						break
					elif "https_proxy=" in word:
						tmp=1
						break
					else:
						tmp=2
				if (tmp==2):
					out_file.write(line)

		with open('temp.txt', 'r') as in_file, open('/etc/bash.bashrc', 'w') as out_file:
			for line in in_file:
				out_file.write(line)
		tmp=0
		with open(home_dir + '/.bashrc', 'r') as in_file, open('temp.txt', 'w') as out_file:
			for line in in_file:
				for word in line.split():
					if "http_proxy=" in word:
						tmp=1
						break
					elif "https_proxy=" in word:
						tmp=1
						break
					else:
						tmp=2
				if (tmp==2):
					out_file.write(line)
		with open('temp.txt', 'r') as in_file, open(home_dir + '/.bashrc', 'w') as out_file:
			for line in in_file:
				out_file.write(line)
			
		os.remove("temp.txt")
	
		if(input=="reset"):
			print("\t")
		else:
			with open(home_dir + '/.bashrc', 'a+') as f:
				f.write("export http_proxy=%s\n" %input)
				f.write("export https_proxy=%s\n" %input)
			with open('/etc/bash.bashrc', 'a+') as f:
				f.write("export http_proxy=%s\n" %input)
				f.write("export https_proxy=%s\n" %input)
			print("set_bashrc is a success\n")
			
	else:
		print("set_bashrc is not a success\n")
		
def set_apt(input):

	#To write the http and https proxy into the /etc/apt/apt.conf.d/90apertisproxy file for apt function
	
	if(input=="reset"):
		if(os.path.exists('/etc/apt/apt.conf.d/90apertisproxy')):
			os.remove("/etc/apt/apt.conf.d/90apertisproxy")
			print( "Proxy removed\n")
		else:
			print("Proxy not set\n")
	else:
		file=open("/etc/apt/apt.conf.d/90apertisproxy", "w")
		os.chmod("/etc/apt/apt.conf.d/90apertisproxy", 0o644)
		file.close()
		
		with open('/etc/apt/apt.conf.d/90apertisproxy', 'w') as f:
			f.write("Acquire::http::Proxy " '"%s"'";\n" %input)
			f.write("Acquire::https::Proxy " '"%s"'";\n" %input)
			
		print("set_apt is a success\n")
		
def set_connman_set_proxy(type, input):

	#To connect to the dbus and update net.connman with all the required properties
	
     print("set_connman_set_proxy called \n")

     bus=dbus.SystemBus()
     
     manager=dbus.Interface(bus.get_object('net.connman', '/'), 'net.connman.Manager')
     
     services=manager.GetServices()
     
     values={"Method": type}
     
     for entry in services:
     	path=entry[0]
     	service=dbus.Interface(bus.get_object('net.connman', path), 'net.connman.Service')

     	if(type=="manual"):
     		values["Servers"]=[input] 	
     	else:
     		values={"Method": type}
     	try:
     		service.SetProperty("Proxy.Configuration", dbus.Dictionary(values, signature='sv'))
     	except dbus.exceptions.DBusException as e_msg:
     		print(e_msg)

def set_curlrc(input):

	#This function is to write the proxy into the curlrc file for curl function
	home_dir=os.getenv('HOME', None)
	if(input=="reset"):
		if(os.path.exists(home_dir + '/.curlrc')):
			os.remove(home_dir + "/.curlrc")
			print("curlrc file removed")
		else:
			print("No curlrc file\n")
	else:
		if(os.path.exists(home_dir + '/.curlrc')):
			with open(home_dir + '/.curlrc', 'w') as f:
				f.write("proxy= %s\n" %input)
				print("set_curlrc() is a success\n")
		else:
			file=open(home_dir + "/.curlrc", "w")
			os.chmod(home_dir + "/.curlrc", 0o755)
			file.write("proxy=%s\n" %input)
			file.close()
			print("set_curlrc() is a success\n")

def set_git(input):
	home_dir=os.getenv('HOME', None)
	if (input=="reset"):
		subprocess.call("git config --global --unset http.proxy", shell=True)
		subprocess.call("git config --global --unset https.proxy", shell=True)
		print("git has successfully unset\n")
	else:
		subprocess.call("git config --global http.proxy "+input+"\n", shell=True)
		subprocess.call("git config --global https.proxy "+input+"\n", shell=True)
		print("set_git() is a success\n")

def set_docker(input):
	if(input=="reset"):
		if(os.path.exists('/etc/systemd/system/docker.service.d/http-proxy.conf')):
			os.remove("/etc/systemd/system/docker.service.d/http-proxy.conf")
			os.rmdir("/etc/systemd/system/docker.service.d")
			print("Docker Proxy removed\n")
		else:
			print("Docker Proxy not set\n")
	else:
		if(os.path.exists('/etc/systemd/system/docker.service.d')):
			os.remove("/etc/systemd/system/docker.service.d/http-proxy.conf")
		else:
			os.mkdir("/etc/systemd/system/docker.service.d")
		file=open("/etc/systemd/system/docker.service.d/http-proxy.conf", "w")
		os.chmod("/etc/systemd/system/docker.service.d/http-proxy.conf", 0o644)
		file.close()
		with open('/etc/systemd/system/docker.service.d/http-proxy.conf', 'w') as f:
			f.write("[Service]\n")
			f.write("Environment=\"HTTP_PROXY=http://localhost:3128\"\n")
			f.write("Environment=\"HTTPS_PROXY=http://localhost:3128\"\n")
			f.write("Environment=\"NO_PROXY=localhost,127.0.0.*,10.*,192.168.*\"\n")
		print("set_docker() is a success\n")

def set_proxy(input):
	set_wgetrc(input)
	set_env(input)
	set_system_wide_env(input)
	set_apt(input)
	set_bashrc(input)
	set_curlrc(input)
	set_git(input)
	set_docker(input)

search_index = 0
def search_options (option):
	global search_index
	if (len(sys.argv)>=2 and sys.argv[1].lower()==option):
		search_index=2
	if (len(sys.argv)>=4 and sys.argv[3].lower()==option):
		search_index=4
	if (len(sys.argv)>=6 and sys.argv[5].lower()==option):
		search_index=6
	if (len(sys.argv)>=8 and sys.argv[7].lower()==option):
		search_index=8


def main():
	global search_index
	if len(sys.argv) <= 1:
		print("Select the URL Option \n")
		print("1. rb-proxy-apac.bosch.com:8080\n")
		print("2. rb-proxy-de.bosch.com:8080\n")
		print("3. Enter proxy URL\n")
	
		mode=input("Enter any option : ")
		if (mode=="1"):
			proxy_cntlm="rb-proxy-apac.bosch.com:8080"
			proxy_domain="APAC"
		elif(mode=="2"):
			proxy_cntlm="rb-proxy-de.bosch.com:8080"
			proxy_domain="DE"
		elif(mode=="3"):
			proxy_cntlm=input("Enter Proxy URL:")
			proxy_domain=input("Domain : ")

		proxy_user=input("Username : ")

		if(proxy_user=='' or proxy_domain=='' or proxy_cntlm==''):
			print("Incorrect input- Requires 3 arguments\n")
			usage()
		else:	
			if(is_only_alpha(proxy_user) or isinstance(proxy_user, int)):
				print("The inputted user ID is incorrect- it should be alphanumeric")
				usage()
			else:
				if(is_only_alpha(proxy_domain)):
					if(os.path.exists('/etc/cntlm.conf') == False):
						print("cntlm.conf file not found in /etc dir\n")
			
					proxy="http://localhost:3128"
					
					set_cntlm_conf(proxy_user, proxy_domain, proxy_cntlm)
					set_proxy(proxy)
					print("Please logout and login/reboot the sdk to update the proxy on Firefox Browser !!!!!!!!")
				else:
					print("The input is incorrect- please enter the domain")
					usage()
	elif(len(sys.argv)==2 and sys.argv[1].lower()=="-r"):
		set_proxy("reset")
		set_cntlm_conf("testuser", "corp-uk", "---")

	elif(len(sys.argv)==2 and sys.argv[1].lower()=="-h"):
		usage()

	else:
		search_index=0
		search_options("-u")
		if(search_index == 0):
			proxy_cntlm = input("Enter Proxy URL : ")
		else:
			proxy_cntlm = sys.argv[search_index]
	
		search_index=0
		search_options("-n")
		if(search_index == 0):
			proxy_user = input("Enter User Name: ")
		else:
			proxy_user = sys.argv[search_index]
	
		search_index=0
		search_options("-d")
		if(search_index == 0):
			proxy_domain = input("Enter Proxy Domain: ")
		else:
			proxy_domain = sys.argv[search_index]

		if(proxy_user=='' or proxy_domain=='' or proxy_cntlm==''):
			print("Incorrect input- Requires 3 arguments\n")
			usage()
		else:
			if(is_only_alpha(proxy_user) or isinstance(proxy_user, int)):
				print("The inputted user ID is incorrect- it should be alphanumeric")
				usage()
			else:
				if(is_only_alpha(proxy_domain)):
					if(os.path.exists('/etc/cntlm.conf') == False ):
						print("cntlm.conf file not found in /etc dir\n")

					proxy="http://localhost:3128"
	
					set_cntlm_conf(proxy_user, proxy_domain, proxy_cntlm)
					set_proxy(proxy)
					print("Please logout and login/reboot the sdk to update the proxy on Firefox browser !!!!!!!!!!!!")
				else:
					print("The input is incorrect- please enter the domain")
					usage()

if __name__=="__main__":
	main()

