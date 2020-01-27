#!/bin/bash

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

input_dir="test_deliverable/test_cases"

working="tmp/deliverable"
mkdir -p ${working}

for i in ${input_dir}/*.c ; do
    if [[ "$i" != *"_driver"* ]]; then
        base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");
        #| is delimiter here because / is used as directory separater. Replaces ${input_dir}/([^.]+)[.]c with ([^.]+)

        # # Compile the reference C version
        # gcc $i $input_dir/${base}_driver.c -o $working/$base
        #
        # # Run the reference C version
        # $working/$base
        # REF_C_OUT=$?



        if [[ ${have_compiler} -eq 0 ]] ; then

            # Create the DUT python version by invoking the compiler with translation flags
            $compiler -S $i -o ${working}/mips/$base-got.s

            # Run the mips code compiled by our own compiler
            mips-linux-gnu-gcc -mfp32 -o test_program.o -c ${working}/mips/$base-got.s

            mips-linux-gnu-gcc -mfp32 -static -o test_program test_program.o ${input_dir}/${base}_driver.c

            qemu-mips test_program
            GOT_C_OUT=$?
        fi


        if [[ ${have_compiler} -ne 0 ]] ; then
            echo "$base, Fail, No C compiler/translator"
        elif [[ 0 -ne $GOT_C_OUT ]] ; then
            echo "$base, Fail, Expected ${REF_C_OUT}, got ${GOT_C_OUT}"
        else
            echo "$base, Pass"
        fi
    fi
done
