#!/bin/bash


blk=$'\x1b[90m' # Sets text to black
red=$'\x1b[31m' # Sets text to red
grn=$'\x1b[92m' # Sets text to green
ylw=$'\x1b[93m' # Sets text to yellow
blu=$'\x1b[94m' # Sets text to blue
pur=$'\x1b[95m' # Sets text to purple
cyn=$'\x1b[96m' # Sets text to cyan
wht=$'\x1b[97m' # Sets text to white
rst=$'\x1b[0m'  # resets to default terminal color


if [[ "$1" != "" ]] ; then
    compiler="$1"
else
    compiler="bin/c_compiler"
fi

have_compiler=0
if [[ ! -f bin/c_compiler ]] ; then
    >&2 echo "Warning : cannot find compiler at path ${compiler}. Only checking C reference against python reference."
    have_compiler=1
fi

input_dir="ourtest/test"

working="tmp/compiler"
mkdir -p ${working}

for i in ${input_dir}/*.c ; do
    if [[ "$i" != *"_driver"* ]]; then
        base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");
        #| is delimiter here because / is used as directory separater. Replaces ${input_dir}/([^.]+)[.]c with ([^.]+)

        # Compile the reference C version
        gcc -std=c89 -ansi $i $input_dir/${base}_driver.c -o $working/$base

        # Run the reference C version
        $working/$base
        REF_C_OUT=$?



        if [[ ${have_compiler} -eq 0 ]] ; then

            # Create the DUT python version by invoking the compiler with translation flags
            $compiler -S $i -o ${working}/mips/$base-got.s

            # Run the mips code compiled by our own compiler
            timeout 3 mips-linux-gnu-gcc -mfp32 -o test_program.o -c ${working}/mips/$base-got.s

            timeout 3 mips-linux-gnu-gcc -mfp32 -static -o test_program test_program.o ${input_dir}/${base}_driver.c

            timeout 10 qemu-mips test_program
            GOT_C_OUT=$?
        fi


        if [[ ${have_compiler} -ne 0 ]] ; then
            echo "$base, ${red}Fail, No C compiler/translator"
        elif [[ $REF_C_OUT -ne $GOT_C_OUT ]] ; then
            echo "$base, ${red}Fail, ${wht}Expected ${REF_C_OUT}, got ${GOT_C_OUT}"
        else
            echo "$base, ${grn}Pass, Expected ${REF_C_OUT}, got ${GOT_C_OUT}${wht}"
        fi
    fi
done
