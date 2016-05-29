#!/bin/bash
boardSize=10
turnNumber=100
pauseTime=1
x1=0 y1=0 x2=0 y2=0
ai1=""
ai2=""


while [[ $# -gt 0 ]]
	do
		flag="$1"
		echo "$flag"

		case $flag in
			-n)
				if [[ $# -lt 2 ]] 
					then exit 1; fi
				boardSize="$2"
				shift 
				;;
			-k)
				if [[ $# -lt 2 ]]
					then exit 1; fi
				turnNumber="$2"
				shift 
				;;
			-s)
				if [[ $# -lt 2 ]] 
					then exit 1; fi
				pauseTime="$2"
				shift 
				;;
			-p1)
				if [[ $# -lt 2 ]] 
					then exit 1; fi
				input="$2"	
				lista="$(echo $input | tr "," "\n")"	
				i=0
				for arg in $lista; do
					tablica[$i]=$arg
					i=$[$i+1]
				done
				x1=${tablica[0]}
				y1=${tablica[1]}
				shift
				;;
			-p2)
				if [[ $# -lt 2 ]] 
					then exit 1; fi
				input="$2"	
				lista="$(echo $input | tr "," "\n")"	
				i=0
				for arg in $lista; do
					tablica[$i]=$arg
					i=$[$i+1]
				done
				x2=${tablica[0]}
				y2=${tablica[1]}
				
				shift
				;;
			-ai1)
				if [[ $# -lt 2 ]] 
					then exit 1; fi
				ai1="$2"
				shift
				;;
			-ai2)
				if [[ $# -lt 2 ]] 
					then exit 1; fi
				ai2="$2"
				shift
				;;
			*) 
				echo "ERROR"
				exit 1
				;;
		esac
		shift
done

#losuje liczbe z podanego przedzialu
losuj() {
	roznica=$[$2 - $1 + 1]
	return $[$1 + $[RANDOM % $roznica]]
}

isNumber() {
	re='^[0-9]+$'
	if [[ ($1 =~ $re) && ($2 =~ $re) && ($3 =~ $re) && ($4 =~ $re) && ($5 =~ $re) && ($6 =~ $re) && ($7 =~ $re) ]]
		then 
			return 1
		else
			return 0
	fi
}



#sprawdzanie poprawnosci
isNumber $x1 $y1 $x2 $y2 $boardSize $pauseTime $turnNumber
if [[ $? == 0 ]] 
	then 
		echo "ERROR - not a number"
		exit 1
	fi

if [[ ("$x1" -lt 0) || ("$y1" -lt 0) || ("$x2" -lt 0) || ("$y2" -lt 0) || ("$x1" -gt "$boardSize") || ("$x2" -gt "$boardSize") \
|| ("$y1" -gt "$boardSize") || ("$y2" -gt "$boardSize") || ("$boardSize" -lt 9) || ("$pauseTime" -lt 0) || ("$turnNumber" -lt 1) ]]
	then
		echo "ERROR - WRONG INPUT"
		exit 1;
	fi

#TODO - sprawdzanie poprawnosci sciezki AI

#TODO randomizing the positions if both not given on the input
#poprawic - musi dac sie krola i inne jednostki rozmiescic
if [[ ($x1 == 0) && ($x2 == 0)]] 
	then
		losuj 1 $[$boardSize - 8]
		x1=$?
		losuj $[$x1 + 8] $boardSize
		x2=$?
		losuj 1 $boardSize
		y1=$?
		losuj 1 $boardSize
		y2=$?
		#x lub y
		losuj 1 2
		if [[ $? == 1 ]]
			then
				temp=$x1
				x1=$y1
				y1=$temp
			
				temp=$x2
				x2=$y2
				y2=$temp				
			fi
	fi
	
#TO DO randomizing the positions if one not given on the input	

#one position not known (x1, y1)
if [[ $x2 == 0 ]]
	then
		x2=$x1
		y2=$y1
		x1=0
		y1=0
	fi
	
if [[ $x1 == 0 ]]
	then
		if [[ ("$x2" -lt 9) && ("$y2" -lt 9) && ("$boardSize - $x2" -lt 8) && ("$boardSize - $y2" -lt 8) ]]
			then
				echo "ERROR - NOT ABLE TO RANDOMIZE"
				exit 1
			else
				# mamy x1, y1 nieznane a x2, y2 znane
				# wiemy ze mozna wylosowac
				echo "NOT READY YET"
		fi
	fi

#TO DELETE - tymczasowe
x1=1
y1=1
x2=7
y2=9



# create a temporary named pipe - script input
PIPE=$(mktemp -u)
mkfifo $PIPE
# attach it to file descriptor 3
exec 3<>$PIPE
# unlink (delete) the named pipe
rm $PIPE

# create a temporary named pipe - script output
PIPE=$(mktemp -u)
mkfifo $PIPE
# attach it to file descriptor 4
exec 4<>$PIPE
# unlink (delete) the named pipe
rm $PIPE


# create a temporary named pipe - ai input
PIPE=$(mktemp -u)
mkfifo $PIPE
# attach it to file descriptor 5
exec 5<>$PIPE
# unlink (delete) the named pipe
rm $PIPE

# create a temporary named pipe - ai output
PIPE=$(mktemp -u)
mkfifo $PIPE
# attach it to file descriptor 6
exec 6<>$PIPE
# unlink (delete) the named pipe
rm $PIPE

# create a temporary named pipe - ai output
PIPE=$(mktemp -u)
mkfifo $PIPE
# attach it to file descriptor 7
exec 7<>$PIPE
# unlink (delete) the named pipe
rm $PIPE

# create a temporary named pipe - ai output
PIPE=$(mktemp -u)
mkfifo $PIPE
# attach it to file descriptor 8
exec 8<>$PIPE
# unlink (delete) the named pipe
rm $PIPE

init1="INIT $boardSize $turnNumber 1 $x1 $y1 $x2 $y2"
init2="INIT $boardSize $turnNumber 2 $x1 $y1 $x2 $y2"

# human vs human
if [[ ($ai1 == "") && ($ai2 == "") ]]
		then 
			./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh -human1 -human2 <&3 > /dev/null &
			pidGUI=$!
			printf "$init1\n$init2\n" >&3
			#TODO tu zakonczyc program jeszcze
		fi

# ai vs human
if [[ ($ai1 != "") && ($ai2 == "") ]]
	then
		./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh -human2 <&3 >&4 &
		pidGUI=$!
		./release/middle_ages <&5 >&6 &
		pid=$!
		echo -e "$init1\n$init2" >&3
		echo -e "$init1" >&5
		echo -e "$init1"

		while [[ 1 ]]
		do	
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					echo $a >&3
				done
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&4
					echo $a >&5
				done
			
			#czy koniec	
			if !(kill -0 $pid); then echo $?; kill $pidGUI; break; fi
		done
	fi

# human vs ai
if [[ ($ai1 == "") && ($ai2 != "") ]]
	then
		./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh -human1 <&3 >&4 &
		pidGUI=$!
		./release/middle_ages <&5 >&6 &
		pid=$!
		
		echo -e "$init1\n$init2" >&3
		echo -e "$init2" >&5
		echo -e "$init2"

		while [[ 1 ]]
		do	
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&4
					echo $a >&5
				done
				
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					echo $a >&3
				done
		
			#czy koniec	
			if !(kill -0 $pid); then kill $pidGUI; break; fi
		done
	fi

# ai vs ai
if [[ ($ai1 != "") && ($ai2 != "") ]]
	then
		./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh <&3 >&4 &
		pidGUI=$!
		./release/middle_ages <&5 >&6 &
		pid1=$!
		./release/middle_ages <&7 >&8 &
		pid2=$!
		echo -e "$init1\n$init2" >&3
		echo -e "$init1" >&5
		echo -e "$init2" >&7

		while [[ 1 ]]
		do	
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					echo $a >&7
					echo $a >&3
				done
				
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&8
					echo $a >&5
					echo $a >&3
				done
			
			sleep $pauseTime
			
			echo $pidGUI
			
			#czy koniec	
			if !(kill -0 $pid1) || !(kill -0 $pid2); then kill $pidGUI; break; fi

		done
	fi
	
#TODELETE parse result printing
echo $boardSize
echo $turnNumber
echo $pauseTime
echo $x1 $y1 
echo $x2 $y2
echo $ai1 $ai2
