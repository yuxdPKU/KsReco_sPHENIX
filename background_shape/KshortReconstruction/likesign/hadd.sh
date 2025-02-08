#runs=(52911)
runs=(53744)

for ((k=0; k<${#runs[@]}; k++))
do
  hadd -f -k -v all${runs[$k]}_kfp.root root/*${runs[$k]}*kfp.root
  hadd -f -k -v all${runs[$k]}_tony.root root/*${runs[$k]}*tony.root
done
