# rolling-hash

rolling hash based file diff algorithm

## Features

Detects changes in a file
Detects additions to a file at the end
Detects removal from a file at the end

Detects compound of the above


### Compilation

#### make
Compile the program with : <make>
Executable name is : rollhashdiff
Run : ./rollhashdiff <filename1> <filename2> <deltaFilename>



#### Test

make test
./test

will execute unit test and common scenarios