#!/bin/bash
boardSize=10
turnNumber=100
pauseTime=1
x1=0 y1=0 x2=0 y2=0
ai1=""
ai2=""


while [[ $# > 0 ]]
	do
		flag="$1"

		case $flag in
			-n)
				if [[ $# < 2 ]] 
					then exit 1; fi
				boardSize="$2"
				shift 
				;;
			-k)
				if [[ $# < 2 ]]
					then exit 1; fi
				turnNumber="$2"
				shift 
				;;
			-s)
				if [[ $# < 2 ]] 
					then exit 1; fi
				pauseTime="$2"
				shift 
				;;
			-p1)
				if [[ $# < 3 ]] 
					then exit 1; fi
				x1="$2"
				y1="$3"
				shift
				shift
				;;
			-p2)
				if [[ $# < 3 ]] 
					then exit 1; fi
				x2="$2"
				y2="$3"
				shift
				shift
				;;
			-ai1)
				if [[ $# < 2 ]] 
					then exit 1; fi
				ai1="$2"
				shift
				;;
			-ai2)
				if [[ $# < 2 ]] 
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
|| ("$y1" -gt "$boardSize") || ("$y2" -gt "$boardSize") || ("$boardSize" < 0) || ("$pauseTime" < 0) || ("$turnNumber" < 0) ]]
	then
		echo "ERROR - WRONG INPUT"
		exit 1;
	fi



#randomizing the positions if both not given on the input
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
	
#randomizing the positions if one not given on the input	

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

#tymczasowe
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


init1="INIT $boardSize $turnNumber 1 $x1 $y1 $x2 $y2"
init2="INIT $boardSize $turnNumber 2 $x1 $y1 $x2 $y2"

#dwojka ludzi gra
if [[ ($ai1 == "") && ($ai2 == "") ]]
		then 
			./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh -human1 -human2 <&3 > /dev/null &
			printf "$init1\n$init2\n" >&3
			#tu zakonczyc program jeszcze
		fi

#jeden czlowiek i jedno ai
if [[ ($ai1 != "") && ($ai2 == "") ]]
	then
		./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh -human2 <&3 >&4 &
		./release/middle_ages <&5 >&6 &
		echo -e "$init1\n$init2" >&3
		echo -e "$init1" >&5
		echo -e "$init1"
		
		while [[ 1 ]]
		do
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					#to powinien wypisac 6 deskryptor
					echo $a
					#echo $a >&3
				done
			#teraz z 4 czyli wyjscia gui przesylamy do ai
		done
	fi


	
#parse result printing
echo $boardSize
echo $turnNumber
echo $pauseTime
echo $x1 $y1 
echo $x2 $y2
echo $ai1 $ai2
