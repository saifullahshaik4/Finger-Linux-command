#!/bin/bash

# test cases
test_cases=(
"./test_whodat"                  # No arguments
"./whodat -h"          
"./whodat -v"             
"./whodat -l"               
"./whodat -L 5"            
"./whodat -p"               
"./whodat -P"               
"./whodat -Y"               
"./whodat invaliduser"      
"./whodat -l -p -P"       
"./whodat -v -v -l -L 5"    
"./whodat rchaney mpj dmcgrath"  
"./whodat rchaney -v -v -p -P -l" 
"./test_whodat -q" 	#invalid argument
)

compare_outputs(){ 
    
    output1=$(eval "$1")
    output2=$(eval "$2")

    if [ "$output1" = "$output2" ]; then
        echo "Test Successful"
    else
        echo "Test Failed"
    fi
}

my_whodat = "./whodat"
i=0
for test_case in "${test_cases[@]}"; do
    i=$((i + 1))    
    echo "Test Number #$i"
    
    echo "Testing: $test_case"
    compare_outputs "$my_whodat $test_case" "$my_whodat $test_case"
done


















