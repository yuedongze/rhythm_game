#! /bin/bash

data1=($(cut -d ',' -f1 $1))
data2=($(cut -d ',' -f3 $1))
data3=($(cut -d ',' -f6 $1 | cut -d ':' -f1))
len=$(cat $1 | wc -l)
max1=0
max2=0
prev1=0
prev2=0
for (( i = 0; i < len; i++ )); do
    
    t1=$((${data2[i]}/1000))
    t2=$((${data2[i]}%1000))
    tmp=1

    if [[ ${data1[i]} -eq 448 ]]; then
        t4="2"
    fi
    if [[ ${data1[i]} -eq 64 ]]; then
        t4="1"
    fi
    if [[ ${data1[i]} -eq 192 ]]; then
        t4="1"
    fi
    if [[ ${data1[i]} -eq 320 ]]; then
        t4="2"
    fi
    
    # if it is not a slider
    if [[ $t4 == "1" ]]; then
        
        if [[ ${data3[i]} -ne 0 ]]; then
            tmp=$(((${data3[i]}-${data2[i]}+1)/30))
            max1=${data3[i]}
        fi
        t3=$tmp
        
        # try to send output
        if [[ $t3 -eq 1 ]]; then
            if [[ ${data2[i]} -gt $max1 ]]; then
                echo $t1,$t2,$t3,$t4 >> $2
            else 
                # try other track
                if [[ ${data2[i]} -gt $max2 ]]; then
                    echo $t1,$t2,$t3,2 >> $2
                fi
            fi
        else
            # it is a slider, use previous value
            if [[ ${data2[i]} -gt $prev1 ]]; then
                echo $t1,$t2,$t3,$t4 >> $2
            else 
                # try other track
                if [[ ${data2[i]} -gt $prev2 ]]; then
                    echo $t1,$t2,$t3,2 >> $2
                    max2=$max1
                    max1=$prev1
                fi
            fi
        fi
        
        
    else if [[ $t4 == "2" ]]; then
        
        if [[ ${data3[i]} -ne 0 ]]; then
            tmp=$(((${data3[i]}-${data2[i]}+1)/30))
            max2=${data3[i]}
        fi
        t3=$tmp
        
        # try to send output
        if [[ $t3 -eq 1 ]]; then
            if [[ ${data2[i]} -gt $max2 ]]; then
                echo $t1,$t2,$t3,$t4 >> $2
            else 
                # try other track
                if [[ ${data2[i]} -gt $max1 ]]; then
                    echo $t1,$t2,$t3,2 >> $2
                fi
            fi
        else
            # it is a slider, use previous value
            if [[ ${data2[i]} -gt $prev2 ]]; then
                echo $t1,$t2,$t3,$t4 >> $2
            else 
                # try other track
                if [[ ${data2[i]} -gt $prev1 ]]; then
                    echo $t1,$t2,$t3,1 >> $2
                    max1=$max2
                    max2=$prev2
                fi
            fi
        fi
        
    fi
    fi
    prev1=$max1
    prev2=$max2
done
