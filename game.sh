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



#sprawdzanie poprawnosci parametrow
isNumber $x1 $y1 $x2 $y2 $boardSize $pauseTime $turnNumber
if [[ $? == 0 ]] 
	then 
		echo "ERROR - not a number"
		exit 1
	fi

if [[ ("$x1" -lt 0) || ("$y1" -lt 0) || ("$x2" -lt 0) || ("$y2" -lt 0) || ("$x1" -gt "$boardSize") || ("$x2" -gt "$boardSize") \
|| ("$y1" -gt "$boardSize") || ("$y2" -gt "$boardSize") || ("$boardSize" -lt 9) || ("$pauseTime" -lt 0) || ("$turnNumber" -lt 1) \
|| ("$boardSize" -gt 2147483647) ]]
	then
		echo "ERROR - WRONG INPUT"
		exit 1;
	fi

#sprawdzanie poprawnosci sciezki AI
if [[ ($ai1 != "") && !(-x $ai1) ]]; then echo "Wrong path"; exit 1; fi
if [[ ($ai2 != "") && !(-x $ai2) ]]; then echo "Wrong path"; exit 1; fi


# randomizing the positions if both not given on the input
if [[ ($x1 == 0) && ($x2 == 0)]] 
	then
		losuj 1 $[$boardSize - 8]
		x1=$?
		losuj $[$x1 + 8] $[$boardSize - 3]
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

swapped=0
#one position not known (x1, y1)
if [[ $x2 == 0 ]]
	then
		x2=$x1
		y2=$y1
		x1=0
		y1=0
		swapped=1
	fi
	
if [[ $x1 == 0 ]]
	then
		if [[ ("$x2" -lt 9) && ("$y2" -lt 9) && ("$boardSize - $x2" -lt 11) && ("$boardSize - $y2" -lt 8) ]]
			then
				echo "ERROR - NOT ABLE TO RANDOMIZE"
				exit 1
			else
				# mamy x1, y1 nieznane a x2, y2 znane
				# wiemy ze mozna wylosowac
				# liczymy pola 4 prostokatow
				lewy=$[$x2 - 8]
				prawy=$[$boardSize - 10 - $x2]
				gorny=$[$y2 - 8]
				dolny=$[$boardSize - 7 - $y2]
				if [[ $lewy -lt 0 ]]; then lewy=0; fi
				if [[ $prawy -lt 0 ]]; then prawy=0; fi
				if [[ $gorny -lt 0 ]]; then gorny=0; fi
				if [[ $dolny -lt 0 ]]; then dolny=0; fi
				
				left=$[$boardSize * $lewy]
				right=$[$boardSize * $prawy]
				
				width=$[$boardSize - $lewy - $prawy - 3]
				
				upper=$[$gorny * $width] 
				bottom=$[$dolny * $width]
				
				echo "$left $right $upper $bottom"
				
				losuj 1 $[$left + $right + $upper + $bottom]
				position=$?
				
				if [[ $position -lt $left ]]
					then
						losuj 1 $lewy
						x1=$?
						losuj 1 $boardSize
						y1=$?
				elif [[ $position -lt $[$left + $right] ]]
					then
						losuj $[$boardSize - 2 - $prawy] $[$boardSize - 3]
						x1=$?
						losuj 1 $boardSize
						y1=$?
				elif [[ $position -lt $[$left + $right + $upper] ]]
					then
						losuj $[$lewy + 1] $[$boardSize - 3]
						x1=$?
						losuj 1 $gorny
						y1=$?
				else
					losuj $[$lewy + 1] $[$boardSize - 3]
					x1=$?
					losuj $[$boardSize + 1 - $dolny] $boardSize
					y1=$?
				fi
				
				if [[ $swapped == 1 ]]; then temp=$x1; x1=$x2; x2=$temp; temp=$y1; y1=$y2; y2=$temp; fi
				echo "$x1 $y1 $x2 $y2"
		fi		
	fi
	

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
			./sredniowiecze_gui_with_libs.sh -human1 -human2 <&3 > /dev/null &
			pidGUI=$!			
			echo -e "$init1\n$init2" >&3
			#czemu ten sleep?
			sleep 1
			#TODO tu zakonczyc program jeszcze
		fi

# ai vs human
if [[ ($ai1 != "") && ($ai2 == "") ]]
	then
		./sredniowiecze_gui_with_libs.sh -human2 <&3 >&4 &
		pidGUI=$!
		./$ai1 <&5 >&6 &
		pid=$!
		echo -e "$init1\n$init2" >&3
		echo -e "$init1" >&5
		
		tempTury=0

		while [[ 1 ]]
		do	
		
		echo "TURA"
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					echo $a >&3
					
					#blad w ai
					if !(kill -0 $pid 2> /dev/null)
						then
							wait $pid
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; pkill -P $pidGUI; exit 1; 
								fi
						fi
				done
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&4
					echo $a >&5
					
					#blad w ai
					if !(kill -0 $pid 2> /dev/null)
						then
							wait $pid
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; pkill -P $pidGUI; exit 1; 
								fi
						fi
				done
			
			((tempTury++))
			
			#czy koniec tur
			if [[ $tempTury == $turnNumber ]]; then wait $pid; pkill -P $pidGUI; break; fi
			
			#czy koniec	
			if !(kill -0 $pid 2> /dev/null); then pkill -P $pidGUI; break; fi
			
			#uzytkownik zamknal gui
			if !(kill -0 $pidGUI 2> /dev/null); then kill $pid; exit 1; fi
			
		done
	fi

# human vs ai
if [[ ($ai1 == "") && ($ai2 != "") ]]
	then
		./sredniowiecze_gui_with_libs.sh -human1 <&3 >&4 2> /dev/null &
		pidGUI=$!
		./$ai2 <&5 >&6 2> /dev/null &
		pid=$!
		
		echo -e "$init1\n$init2" >&3
		echo -e "$init2" >&5
		
		tempTury=0

		while [[ 1 ]]
		do	
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&4
					echo $a >&5
					
					#blad w ai
					if !(kill -0 $pid 2> /dev/null)
						then
							wait $pid
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; pkill -P $pidGUI; exit 1; 
								fi
						fi
				done
				
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					echo $a >&3
					
					#blad w ai
					if !(kill -0 $pid 2> /dev/null)
						then
							wait $pid
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; pkill -P $pidGUI; exit 1; 
								fi
						fi
				done
			
			((tempTury++))
			
			#czy koniec tur
			if [[ $tempTury == $turnNumber ]]; then wait $pid; pkill -P $pidGUI; break; fi
			
			#czy koniec	
			if !(kill -0 $pid 2> /dev/null); then pkill -P $pidGUI; break; fi
	
		
			#uzytkownik zamknal gui
			if !(kill -0 $pidGUI 2> /dev/null); then kill $pid; exit 1; fi

		done
	fi

# ai vs ai
if [[ ($ai1 != "") && ($ai2 != "") ]]
	then
		./sredniowiecze_gui_with_libs.sh <&3 >&4 2> /dev/null &
		pidGUI=$!
		./$ai1 <&5 >&6 2> /dev/null &
		pid1=$!
		./$ai2 <&7 >&8 2> /dev/null &
		pid2=$!
		echo -e "$init1\n$init2" >&3
		echo -e "$init1" >&5
		echo -e "$init2" >&7
		
		tempTury=0


		sleep $pauseTime

		while [[ 1 ]]
		do	
		
			a=""
			while [[ $a != "END_TURN" ]]
				do
					read a <&6
					echo $a >&7
					echo $a >&3
					
					#blad w ai
					if !(kill -0 $pid1 2> /dev/null)
						then
							wait $pid1
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; kill $pid2; pkill -P $pidGUI; exit 1; 
								fi
						fi
					
					if !(kill -0 $pid2 2> /dev/null)
						then
							wait $pid2
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; kill $pid1; pkill -P $pidGUI; exit 1; 
								fi
						fi
					
					
				done
									
			a=""
			
			while [[ $a != "END_TURN" ]]
				do
					read a <&8
					echo $a >&5
					echo $a >&3
					
					#blad w ai
					if !(kill -0 $pid1 2> /dev/null)
						then
							wait $pid1
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; kill $pid2; pkill -P $pidGUI; exit 1; 
								fi
						fi
					
					if !(kill -0 $pid2 2> /dev/null)
						then
							wait $pid2
							if [[ $? == 42 ]]; 
								then 
									echo "AI ERROR"; kill $pid1; pkill -P $pidGUI; exit 1; 
								fi
						fi
						
				done
			
			sleep $pauseTime
			
			((tempTury++))
			
			#czy koniec tur
			if [[ $tempTury == $turnNumber ]]; then wait $pid1; pkill -P $pidGUI; break; fi
									
			#czy koniec	
			if !(kill -0 $pid1 2> /dev/null)
				then
					pkill -P $pidGUI; kill $pid2; exit 0
				fi
			
			if !(kill -0 $pid2 2> /dev/null)
				then 
					pkill -P $pidGUI; kill $pid1; exit 0
				fi
			
			#uzytkownik zamknal gui
			if !(kill -0 $pidGUI 2> /dev/null); then kill $pid1; kill $pid2; exit 1; fi
			
			

		done
		
	fi
