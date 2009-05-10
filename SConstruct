# ==============================================================
#  Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
# 
#  This software is released under the BSD License
#  <http://www.opensource.org/licenses/bsd-license.php>
# ==============================================================

# Based on a Linux Ogre/CEGUI/OIS/OgreOde build Script by Grey
# (http://www.ogre3d.org/wiki/index.php/User:Grey), with many thanks to keir
# from #scons on freenode.
import os

# Run the created program
def runProg(env, target, source):
    os.spawnl(os.P_WAIT, './' + str(source.pop()))

# Find the library pointed to by 'lib' and place relevant information in 'env'
def getLib(env, lib):
    # For now, depend on pkg-config
    env.ParseConfig('pkg-config --silence-errors --libs --cflags ' + lib)

# Get arguments
mode = ARGUMENTS.get('mode', 'debug')

# Object files go in obj/<Debug|Release>
if mode == 'debug':
    buildDir = 'obj/Debug'
else:
    buildDir = 'obj/Release'
BuildDir(buildDir, 'src', duplicate = 0)

# Set up compilation flags
env = Environment(CCFLAGS = ['-ansi', '-Wall', '-fexceptions'])
if mode == 'debug':
    env.AppendUnique(CCFLAGS = ['-ggdb', '-D_DEBUG'])
else:
    env.AppendUnique(CCFLAGS = ['-s', '-O2', '-DNDEBUG'])

# get external libraries
libs = ['OGRE', 'OIS', 'CEGUI-OGRE']
for lib in libs:
#    # If we are in debug mode, first try to use debug libraries
#    if mode == 'debug':
#        try:
#            getLib(env, lib + '_d')
#            continue
#        except OSError:
#            print 'Error finding debug library ' + lib + '_d; using ' + lib
    try:
        getLib(env, lib)
    except OSError:
        print 'Error finding library ' + lib
        Exit(1)
    
source = [Glob(buildDir + '/*.cpp')]
target = 'tachyon'

# Build our main program
env.Program(target = target, source = source)

# Run the program
env.Command(target + '.run', target, runProg)
