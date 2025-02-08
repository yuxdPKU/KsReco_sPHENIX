#!/bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n new  # setup sPHENIX environment in the singularity container shell. Note the shell is bash by default

# Additional commands for my local environment
export SPHENIX=/sphenix/u/xyu3
#export MYINSTALL=$SPHENIX/install
export MYINSTALL=/sphenix/u/xyu3/workarea/PhotonConv/KsNew/background_shape/KshortReconstruction/debug/install

# Setup MYINSTALL to local directory and run sPHENIX setup local script
# to adjust PATH, LD LIBRARY PATH, ROOT INCLUDE PATH, etc
source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

echo "sPHENIX environment setup finished"

this_script=$BASH_SOURCE
this_script=`readlink -f $this_script`
this_dir=`dirname $this_script`
echo running: $this_script $*

nEvents=10
InDst=DST_STREAMING_EVENT_run2pp_ana441_2024p007-00052911-00000.root
InPath=/sphenix/lustre01/sphnxpro/physics/slurp/streaming/physics/ana441_2024p007/run_00052900_00053000/
OutPrefix=./clusters_seeds
Index=0
StepSize=0

root.exe -q -b Fun4All_FullReconstruction.C\($nEvents,\"${InDst}\",\"${InPath}\",\"${OutPrefix}\",$Index,$StepSize\)
echo Script done
