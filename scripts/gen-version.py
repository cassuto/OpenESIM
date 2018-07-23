#
# Script to generate a C header file with version information.
#

#
# Usage:
# gen-version.py {Name} {NameShort} {VersionMajor} {VersionMinor} {VersionBuild} {out filename}
#


#
#  OpenDSIM (an a/d mixed circuit simulator)
#
#  Copyleft (C) 2016, No.1 Middle School in Yongsheng,Lijiang,China
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser
#  General Public License(GPL) as published by the Free Software
#  Foundation; either version 2.1 of the License, or (at your option)
#  any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#


import os
import sys
import time

# get the formatted timestamp.
def getDateTime():
	timeFormat = '%Y%m%d_%H%M%S'
	return time.strftime( timeFormat, time.localtime(time.time()) )

# get the hostname.
def getHostname():
	sys = os.name
	
	if sys == 'nt':
		hostname = os.getenv('computername')
		return hostname
	elif sys == 'posix':
		host = os.popen('echo $HOSTNAME')
		try:
			hostname = host.read()
			return hostname
		finally:
			host.close()
	else:
		return '[Unkwon host]'

# delete the newline char
def fmtString( str ):
    tmp = str.replace('\n', '')
    tmp = tmp.replace('\r', '')
    return tmp


# check the count of args
if(len(sys.argv) != 7):
	print 'gen-version.py: Err: Wrong parameters!'
	sys.exit(1)


# define the basic infomation
appName = sys.argv[1]
appNameShort = sys.argv[2]
appVersionMajor = sys.argv[3]
appVersionMinor = sys.argv[4]
appVersionBuild = sys.argv[5]
appVersionRaw = '%s.%s.%s' %(appVersionMajor,appVersionMinor,appVersionBuild)
appBuiltTime = getDateTime()
appBuiltHost = getHostname()

# generate the banner string.
banner = appName + ' '
banner += '(v' + appVersionRaw + ' '
banner += 'built ' + appBuiltTime
banner += '-' + appBuiltHost + ')'

# generate c code
hcode = \
'''#ifndef ___version_generated_h___
#define ___version_generated_h___

#define appName "''' + fmtString(appName) + '''"
#define appNameShort "''' + fmtString(appNameShort) + '''"
#define appVersionMajor ''' + fmtString(appVersionMajor) + '''
#define appVersionMinor ''' + fmtString(appVersionMinor) + '''
#define appVersionBuild ''' + fmtString(appVersionBuild) + '''
#define appVersionRaw "''' + fmtString(appVersionRaw) + '''"
#define appBuiltTime "''' + fmtString(appBuiltTime) + '''"
#define appBuiltHost "''' + fmtString(appBuiltHost) + '''"
#define appBanner "''' + fmtString(banner) + '''"

#endif
'''


# Write the header file
f = open(sys.argv[6], 'w')
f.write(hcode)
f.close()



print('gen-version.py: ' + banner)
