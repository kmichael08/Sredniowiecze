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

#randomizing the positions if both not given on the input
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
		x2=0
		y1=0
	fi
	
if [[ $x1 == 0 ]]
	then
		losuj 1 2
		if [[ $? == 1 ]]
			then
				temp=$x2
				x2=$y2
				y2=$temp
			fi
		
		#na x sie nie da
		if [[ ($x1 < 10) && ($[$boardSize - $x1] < 8) ]]
			then
				losuj 1 $boardSize
				x1=$?
				
				#poprawic - y z przedzialu
				losuj 1 $boardSize
				y1=$?
			else
				#da sie na x
				if [[ $y1 < 10 && $[$boardSize - $y1] < 8 ]]; then echo "ERROR"; exit 1; fi
				losuj 1 $boardSize
				y1=$?
				
				#poprawic - x z przedzialu
				losuj 1 $boardSize
				x1=$?	
			
			fi
	fi

# create a temporary named pipe
PIPE=$(mktemp -u)
#mkfifo $PIPE
# attach it to file descriptor 3
exec 3<>$PIPE
# unlink (delete) the named pipe


init1="INIT $boardSize $turnNumber 1 $x1 $y1 $x2 $y2"
init2="INIT $boardSize $turnNumber 2 $x1 $y1 $x2 $y2"

printf "$init1\n$init2\n" >&3


if [[ ($ai1 == "") && ($ai2 == "") ]]
		then 
			#moze wyrzucac do devnull
			./sredniowiecze_gui_linux64_v1/sredniowiecze_gui_with_libs.sh -human1 -human2 <$PIPE 
			#sleep 1
			#pid=$!
			#echo "Podprogram ma pid $pid"
			#kill $pid

		fi
	
#parse result printing
echo $boardSize
echo $turnNumber
echo $pauseTime
echo $x1 $y1 
echo $x2 $y2
echo $ai1 $ai2
