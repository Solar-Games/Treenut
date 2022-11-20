# Treenut
 Treenut - A scripting language.

## Goals
 Our goal is to simply make a scripting language that is based on C.
 
## Current Issues
 You cannot re-run a output file from the compiler with all the tokens.
 You cannot do essential features like definitions and printing, as the execution function is not finished yet.
 Compiling files to an output then rereading the inputs will not give the same result. (Simply EOF problems after writing tokens to bytes.)
 
## Intentional Design
 This: ~| (Print) / ~! (Error).