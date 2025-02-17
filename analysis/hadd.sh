runs=(53744)

for ((k=0; k<${#runs[@]}; k++))
do
  #hadd -f -k -v all${runs[$k]}_kfp.root root/*${runs[$k]}*kfp.root
  hadd -f -k -v all${runs[$k]}_ksreco.root root/clusters_seeds_${runs[$k]}-*.root_ks_ksreco.root
done
