#!/bin/bash

root=$1
txt=$2
w=$3
p=$4
DIR=$(dirname "${txt}")
echo "${DIR}"
shufNum=$(($p*$w))
entries=($(shuf -i 0-10000 -n $shufNum))

#Check if root directory and text file exist
[ -d $root ]&&echo "Directory $root exists"||echo "Directory $root does not exist"
[ -e "$txt" ]&&echo "File $txt exists"||echo "File $txt does not exist"

#Check if w and p are integers
re='^[0-9]+$'
if ! [[ $w =~ $re ]] ; then
   echo "error: Not a number" >&2; exit 1
fi

if ! [[ $p =~ $re ]] ; then
   echo "error: Not a number" >&2; exit 1
fi

#Check if text file lines are more than 10.000
linesInText=$(cat $txt | wc -l)

if [[ $linesInText<10000 ]]; then
	echo "error: File lines under 10000" >&2; exit 1
fi

if test "$(ls -A "$root")"; then
    echo "Warning: Directory webpages is not empty, purging..."
else
    echo "Directory webpages is empty, creating specified subdirectories"
fi

cd $root


f=$((($p / 2)+1))
q=$((($w / 2)+1))
index=0

for (( i=0; i<$w; i++ ));
do
	#If site$i already exists, then remove it and all its contents
	if [ -d "site$i" ]; then
		echo "removing subfolder site$i and all its contents"
		rm -rf "site$i"
	fi
	#Create subdirectory and enter it
	mkdir "site$i"
	cd "site$i"
	for (( j=0; j<$p;j++ ));
	do
		#Create the files in the subdirectory and store them in an array
		file="page$i"_"${entries[(($i*$p))+$j]}.html"
		touch $file
		arr[$((($i*$p)+$j))]=$file
	done
	#Go back to the webpages directory
	cd ".."
done

for (( i=0; i<$w; i++ ));
do
	cd "site$i"
  echo "Creating web site $i"
	#Get all files in the subdirectory site$i
	files=(*)
	for (( h=0; h<$p; h++ )); do
		k=$(( ( RANDOM % ($linesInText-2000) )  + 1 ))
		m=$(( ( RANDOM % 1000 )  + 1000 ))
		#Emptying the linkSet array because we don't want the links from the previous website to be copied to the current website.
		unset linkSet
		#Deleting file contents before initializing HTML headers
		currentFile="${arr[(($i*$p))+$h]}"
    echo "  Creating page `pwd`/$currentFile with $m lines starting at line $k ..."
		truncate -s 0 $currentFile
		#Initial HTML Headers
		echo "<!DOCTYPE html>" >> $currentFile
		echo "<html>" >> $currentFile
		echo -e "\t<body>" >> $currentFile
		#Write the links inside each file named page($i)_${entries[(($i*$p))+$j]}.html
		for (( j=0; j<$f; j++ ));
		do
			#Links pointing in the same folder
			#Getting a random link from the files variable, which contains all files in the subfolder, then if the random link is the same as the current file, don't do anything.
			randFile="${files[RANDOM % ${#files[@]}]}"
			if [[ "$currentFile" != "$randFile" ]]; then
				linkSet[(($i*$p))+$j]="$randFile"
			else
				j=$((j-1))
			fi

		done
		for (( j=0; j<$q; j++ ));
		do
			#Links pointing in another folder
				randFile="${arr[RANDOM % ${#arr[@]}]}"
				if [[ ${randFile:4:1} = ${currentFile:4:1} ]]; then
					j=$((j-1))
				else
					linkSet[(($i*$p))+$j+$f]=$randFile
				fi
		done
    #Defining the point where the script will start copying the text
		startpoint=$k
    #Defining the point where the script will stop copying the text
		endpoint=$(($k+($m/($f+$q))))
    #While the starting point line count is less than the number of lines in the file, keep copying
		while [[ "$startpoint" -lt "$linesInText" ]]; do
			selectedRandFile=${linkSet[$((RANDOM % ${#linkSet[@]}))]}
			diff=$(($endpoint - $startpoint))
			sed -n "$startpoint","$endpoint"p "../../$txt" >> $currentFile
      echo "    Adding link to `pwd`/$currentFile"
			echo -e "\t\t<a href=\"$selectedRandFile\">link$i"_"text</a>" >> $currentFile
      #Update start and end point
			startpoint=$((startpoint+diff))
			endpoint=$(($endpoint+($m/($f+$q))))
		done

		echo -e "\t</body>" >> $currentFile
		echo "</html>" >> $currentFile
	done
	cd ".."
done
echo "Done"
