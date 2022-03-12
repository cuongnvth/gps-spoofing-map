#!/bin/sh
day=$(date +%j)
year=$(date +%Y)
yr=$(date +%y)
count=0

if [ ! -e ".wgetrc" ]; then
  echo "http_user=cuongnvth@gmail.com\nhttp_password=Cuongthoa106" > .wgetrc
  echo "Enter your CDDIS portal credentials into .wgetrc and then re-run"
  exit
fi

export WGETRC=./.wgetrc

while [ 1 ] 
do
	if ping -q -c 1 -W 1 8.8.8.8 >/dev/null; then #kiem tra ket noi mang
		if [ $? -eq 0 ];then
			if [ `date +%H` -lt 8 ]; then 
				day='0'`expr $(date +%j) - 1`
				echo "$year""/$day""/hour""$day""0.$yr"
			fi
			#wget --timeout=1 --waitretry=0 --tries=20 --retry-connrefused --ftp-user anonymous --ftp-password cuongnvth@gmail.com "ftps://cddis.nasa.gov/archive/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.gz";
			#wget --timeout=1 --waitretry=0 --tries=20 --auth-no-challenge "https://cddis.nasa.gov/archive/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.gz";
			# wget --timeout=1 --waitretry=0 --tries=20 --retry-connrefused "ftps://gdc.cddis.eosdis.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
			#wget --auth-no-challenge "https://cddis.gsfc.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";
			#curl --connect-timeout 15 --retry 10 --ciphers DEFAULT@SECLEVEL=1 --user cuongnvth:Cuongthoa106 -c [file] -n -L -O "https://cddis.nasa.gov/archive/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.gz";
			curl --connect-timeout 15 --retry 10 -u anonymous:cuongnvth@gmail.com -O --ftp-ssl "ftp://gdc.cddis.eosdis.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.gz";
			#wget --timeout=1 --waitretry=0 --tries=5 --retry-connrefused "ftp://cddis.gsfc.nasa.gov/gnss/data/hourly/$year""/$day""/hour""$day""0.$yr""n.Z";

			if [ -f "hour""$day""0.$yr""n.gz" ]; then
				echo "hour""$day""0.$yr""n.gz exists."
				uncompress -f "hour""$day""0.$yr""n.gz"; 
				mv -f "hour""$day""0.$yr""n" "hourly.n";
				echo "hourly.n";
				return;
			fi
		fi
	fi
done;
