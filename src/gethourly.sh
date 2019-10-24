#!/bin/sh
day=$(date +%j)
year=$(date +%Y)
yr=$(date +%y)

#cd ./BUILD/
#wget "ftp://cddis.gsfc.nasa.gov/gnss/data/daily/$year""/brdc/brdc""$day""0.$yr""n.Z"
#sleep 10 
while [ 1 ]
 do 
	ping -c 1 -W 0.7 8.8.4.4 > /dev/null 2>&1
	if [ $? -eq 0 ];then
		curl -O "ftp://cddis.gsfc.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
		uncompress -f "hour""$day""0.$yr""n.Z"; 
		rm -f "hour""$day""0.$yr""n.Z";
		mv -f "hour""$day""0.$yr""n" "hourly.n";
		echo "hourly.n";
		break;
	fi
done;

