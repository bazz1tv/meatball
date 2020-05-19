while IFS= read line; do
	colnum=0
	for col in $line; do
		if [[ $colnum == 0 ]]; then
			echo -n $col
		elif [[ $colnum == 1 ]]; then
			echo -n " $col"
		elif [[ $colnum == 2 ]]; then
			echo -n " $col"
		elif [[ $colnum == 3 ]]; then
			echo -n " $((col - 100))"
		elif [[ $colnum == 4 ]]; then
			echo " $col"
		fi
		colnum=$((colnum + 1))
	done
done <test3.lvl
