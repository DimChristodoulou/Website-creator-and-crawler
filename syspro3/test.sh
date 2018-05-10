#old if
	if [ ! -d "site$i" ]; then
		mkdir "site$i"
		cd "site$i"
		for (( j=0; j<$p;j++ ));
		do
			file="page($i)_${entries[(($i*$p))+$j]}.html"
			touch $file
			arr[$((($i*$p)+$j))]=$file
		done
		cd ".."
	else
		cd "site$i"
		for file in *; do
  			arr[$index]=${file##*/}
  			index=$((index+1))
  		done
  		cd ".."
	fi