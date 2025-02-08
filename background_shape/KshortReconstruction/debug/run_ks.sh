#!/bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n new  # setup sPHENIX environment in the singularity container shell. Note the shell is bash by default

# Additional commands for my local environment
export SPHENIX=/sphenix/u/xyu3
export MYINSTALL0=$SPHENIX/install
export MYINSTALL=/sphenix/u/xyu3/workarea/PhotonConv/KsNew/background_shape/KshortReconstruction/debug/install

# Setup MYINSTALL to local directory and run sPHENIX setup local script
# to adjust PATH, LD LIBRARY PATH, ROOT INCLUDE PATH, etc
source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL $MYINSTALL0

echo "sPHENIX environment setup finished"

this_script=$BASH_SOURCE
this_script=`readlink -f $this_script`
this_dir=`dirname $this_script`
echo running: $this_script $*

nEvents=-1
InDst=clusters_seeds_53744-0-0.root_dst.root
InPath=/sphenix/u/xyu3/hftg01/Full_track_DST/Reconstructed/53744/
OutPrefix=./clusters_seeds
RunNumber=53744
Segment=0
Index=0
StepSize=0

root.exe -q -b Fun4All_FullReconstruction_Ks.C\($nEvents,\"${InDst}\",\"${InPath}\",\"${OutPrefix}\",$RunNumber,$Segment,$Index,$StepSize\)
echo Script done
