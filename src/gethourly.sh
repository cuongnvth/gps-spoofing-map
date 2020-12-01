#!/bin/sh
day=$(date +%j)
year=$(date +%Y)
yr=$(date +%y)
count=0
#cd ./BUILD/
#wget "ftp://cddis.gsfc.nasa.gov/gnss/data/daily/$year""/brdc/brdc""$day""0.$yr""n.Z"
#sleep 10 
while [ 1 ]
 do 
	ping -c 1 -W 0.7 8.8.4.4 > /dev/null 2>&1
	if [ $? -eq 0 ];then
		wget --timeout=1 --waitretry=0 --tries=20 --retry-connrefused --ftp-user anonymous --ftp-password cuongnvth@gmail.com "ftps://gdc.cddis.eosdis.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
		# wget --timeout=1 --waitretry=0 --tries=20 --retry-connrefused "ftps://gdc.cddis.eosdis.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
		#wget --auth-no-challenge "https://cddis.gsfc.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
		# curl --connect-timeout 15 --retry 10 -u anonymous:cuongnvth@gmail.com -O --ftp-ssl "ftp://gdc.cddis.eosdis.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
		#wget --timeout=1 --waitretry=0 --tries=5 --retry-connrefused "ftp://cddis.gsfc.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
		if [ -f "hour""$day""0.$yr""n.Z" ]; then
			echo "hour""$day""0.$yr""n.Z exists."
			uncompress -f "hour""$day""0.$yr""n.Z"; 
			mv -f "hour""$day""0.$yr""n" "hourly.n";
			echo "hourly.n";
			return;
		else
			day=`expr $(date +%j) - 1`
			echo "$day"
		fi
	fi
done;

