#!/usr/bin/env bash


# $1 - test suit
# $2 - TASKID
# $3 - field ID

test_server_build()
{
    g++ server.cpp -o server
    build_status=$?
    assertTrue "Build failed" "[ $build_status -eq 0 ]"
}

test_fail_server1()
{
    ./server "Fdwd" 
    return_status=$?
    assertTrue "Application test failed" "[ $return_status -eq 1 ]"
    assertTrue "Port is Fdwd" "[ $return_status -eq 1 ]"
}

test_fail_server2()
{
    ./server "3425" 2523
    return_status=$?
    assertTrue "Application test failed" "[ $return_status -eq 1 ]"
    assertTrue "Number of arguments bigger than 1!" "[ $return_status -eq 1 ]"
}

test_server_configuration()
{
    input_file='test/input.txt'
    output_file='test/output.txt'
    #echo $input_file
    #echo $output_file
    g++ sender.cpp -o sender
    ./server 3425 &
    process=$!
    if [[ (-n $input_file) && (-n $output_file) ]]
    then
        cat $input_file | ./sender 3425 > output1.txt
        kill ${process}
        diff_lines=$(diff $output_file output1.txt | wc -l)
        #assertTrue "Test for configuration failed. Output is not correct." "[ $diff_lines -eq 0 ]"
        if [[ $diff_lines -gt 0 ]]
        then
            echo "diff "$output_file" output1.txt"
            diff $output_file output1.txt
        fi
    else
    	echo "No input data found. Did you mess up with the code?"
        fail "No input data found. Did you mess up with the code?"
    fi
}




if [[ -d "shunit2-2.1.8" ]]
then
    echo "Found shunit2-2.1.8"
else
    echo "Downloading shunit2-2.1.8 ..."
    curl https://codeload.github.com/kward/shunit2/tar.gz/v2.1.8 --output shunit2-2.1.8.tar.gz
    tar -xzf shunit2-2.1.8.tar.gz
fi
# load shunit2
. shunit2-2.1.8/shunit2

