#! /usr/bin/sh
#
# @Author: Kevin van As, PhD student at TU Delft
# @Date: 27/06/2017
#
# This script will obtain the "Make" directory from your solver,
# and then alter Make/files and Make/options such that your solver
# may be recompiled without copying the source of your solver,
# and linked with the kva_interfaceProperties library.
# The advantage of not copying your source code, is that you can
# easily make changes to your solver, while not having to keep
# this copy of your solver in sync.
# Then you can recompile this copy at any time by just a simple "wmake" call.
#
#
# Usage:
#  Using the hard-coded variables set below:
#   getSolverMake.sh
#  Using command-line arguments (solverPATH=$1, optional newsolverName=$2)
#   getSolverMake.sh /full/path/to/solver optional_new_solver_name
#


#/*****************************\
#|                             |
#| Please set these variables. |
#|                             |
#\*****************************/

# FULL path to the directory that contains the solver code:
solverPATH="$FOAM_SOLVERS/multiphase/interFoam"

# Name of the executable (solver) that will be written to the directory $FOAM_USER_APPBIN:
newsolverName=""
# Leave 'newsolverName' blank (empty string) to retain the same name.
#  Advantage: all your existing cases will automatically use the new solver.
#  Disadvantage: more difficult to use the old solver, as you'll need its full path to call it.





#/*********************\
#|                     |
#| Do NOT touch below. | Unless you are a hacker, of course.
#|                     |
#\*********************/

## Parse arguments
# Overwrite hard-coded directories by command-line settings:
if [ "$1" != "" ]; then
	solverPATH="$1"
fi
if [ "$2" != "" ]; then
	newsolverName="$2"
fi

## Work from this directory
cd "$(dirname "$0")"

## Perform checks on input
if [ ! -d "$solverPATH" ]; then
	# If solver source does not exist, error.
	echo "Directory \"$solverPATH\" does not exist. Please specify the valid FULL PATH to the directory that contains the solver source code." >&2
	exit 1
fi
if [ ! -d "$solverPATH/Make" ]; then
	# If solver source does not have a "Make" directory, then error.
	echo "Directory \"$solverPATH/Make\" does not exist. Please specify the valid FULL PATH to the directory that contains the solver source code." >&2
	exit 1
fi

## Prepare a directory for the solver
if [ "$newsolverName" == "" ]; then
	newsolverDir="$(basename "$solverPATH")"
else
	newsolverDir="$(basename "$newsolverName")"
fi
[ -d "$newsolverDir" ] && rm -rf "$newsolverDir"
mkdir "$newsolverDir"
cd "$newsolverDir" > /dev/null

## Prepare solverName for sed
if [ "$newsolverName" == "" ]; then
	# If no solver name is specified, then retain the original name.
	newsolverName="\2" # Note: "\2" is used by sed to recall a group that contains the name of the original solver.
fi

## Copy the "Make" folder from the solver
cp -r "$solverPATH/Make" .

## Now we adapt Make/files to write to a custom name and to $FOAM_USER_APPBIN
sed -i -r 's/(EXE\s*=\s*).*\/([^\/]+)/\1\$\(FOAM_USER_APPBIN\)\/'"$newsolverName"'/' "Make/files"
## Then we adapt Make/files to link to the original source's .C files (no reason to copy those, right? We do not change the solver!)
solverPATH2=$(echo "$solverPATH" | sed 's/\//\\\//g') # First dodge "/", before we can use it in sed
sed -i -r 's/(.*\.C)/'"$solverPATH2"'\/\1/g' "Make/files"

## Replace relative paths with absolute paths
#     For example, "../lnInclude" should become "fullPath/../lnInclude"
# Delete "-I. \"
linenum_INC=$( grep -n -P '\-I\.\s*\\$' "Make/options" | sed -r 's/([0-9]+):.*/\1/' )
if [ "$linenum_INC" != "" ]; then
	head -n$((linenum_INC-1)) "Make/options" > Make/options2
	tail -n+$((linenum_INC+1)) "Make/options" >> Make/options2
	## Then apply these changes to the main file
	mv "Make/options2" "Make/options"
fi
# Replace "-I.." by "-I/full/path/.."
sed -i -r 's/-I\.\./-I'"$solverPATH2"'\/../g' "Make/options"

## Insert the solver directory as include search path (-I) 
## And add the lnInclude of kva_interfaceProperties (../../lnInclude)
linenum_INC=$( grep -n -E ".*_INC\s*=\s*"  "Make/options" | sed -r 's/([0-9]+):.*/\1/')
head -n$linenum_INC "Make/options" > Make/options2
echo "    -I$solverPATH \\" >> Make/options2
echo "    -I../../lnInclude \\" >> Make/options2
tail -n+$((linenum_INC+1)) "Make/options" >> Make/options2
# Note: we don't have to remove OF's interfaceProperties/lnInclude.
# In fact, we need it because of "surfaceTensionModels".
# We only have to make sure that "../../lnInclude" appears before it, we it does, as we just added "../../lnInclude" to the top!
 #    "-I$(LIB_SRC)/transportModels/interfaceProperties/lnInclude \" --> "../../lnInclude \"
 #sed -i -r 's/-I.*interfaceProperties(\/lnInclude .*)/-I..\/..\1/' "Make/options2"
## Then apply these changes to the main file
mv "Make/options2" "Make/options"

## Make the executable link against kva_interfaceProperties, which is stored in $FOAM_USER_LIBBIN.
linenum_LIBS=$(grep -n -E ".*_LIBS\s*=\s*" "Make/options" | sed -r 's/([0-9]+):.*/\1/')
head -n$linenum_LIBS "Make/options" > Make/options2
echo '    -L$(FOAM_USER_LIBBIN) \' >> Make/options2
echo '    -lkva_interfaceProperties \' >> Make/options2
tail -n+$((linenum_LIBS+1)) "Make/options" >> Make/options2
## Then apply these changes to the main file
mv "Make/options2" "Make/options"


# And now you can compile (wmake).



#EOF
