#! /usr/bin/env python3.6

# Find  the main directory of the event and add path to lib:
import sys, os, re
r = os.getcwd().split("/")
maindir = "/".join(r[:r.index("run")])
sys.path.append(maindir + '/lib/')

import poet_1event  as p1
import poet_2badpix as p2
import poet_3center as p3
import poet_4photom as p4
import poet_5checks as p5
import poet_denoise as pd

# argument 1:
func = sys.argv[1]
# Dictionary of what data file look for, in each case:
# (if argument 1 is p1, then look for ...)
pattern = {"p1" : "[\n]?(.+[^photom|^center]\.pcf)[\n]?", # the event pcf
           "p2" : "[\n]?(.+_ini)\.dat[\n]?",   # data file ended in "_ini.dat"
           "pd" : "[\n]?(.+_bpm)\.dat[\n]?",   #                    "_bpm.dat"
           "p3" : "[\n]?(.+[_den|_bpm])\.dat[\n]?", # "_den.dat" or "_bpm.dat"
           "p4" : "[\n]?(.+_ctr)\.dat[\n]?",   #                    "_ctr.dat"
           "p5" : "[\n]?(.+[_ctr|_pht])\.dat[\n]?"} # "_ctr.dat" or "_pht.dat"

# Folder where the data is located
try:
  directory = maindir + "/run/" + sys.argv[2]
except:
  directory = maindir + "/run/"
files = '\n'.join(os.listdir(directory))


# Get necessary file:
r = re.search(pattern[func], files)
try:
  arg = r.groups(1)[0]
except:
  print('wrong directory')

# Run:
if   func == "p1":
  e = p1.Event(arg)
  
elif func == "p2":
  p2.badpix(arg)
  
elif func == "pd":
  pd.run_denoising(arg, 'denoise.pcf')
  
elif func == "p3":
  p3.run_centering(arg, 'center.pcf', cwd=directory)

elif func == "p4":
  p4.run_photometry(arg, 'photom.pcf', cwd=directory)  
elif func == "p5":
  p5.checks(arg, cwd=directory)
