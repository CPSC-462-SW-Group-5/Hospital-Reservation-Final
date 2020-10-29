#!/usr/bin/env bash
#
# Script file to compile all C++ source files in or under the
# current directory.  This has been used in the cygwin and OpenSUSE and Ubuntu
# environment with the GCC and Clang compilers and linkers
assignmentName="${1:-homework}"

# Find and display all the C++ source files to be compiled ...
# temporarily ignore spaces when globing words into file names
temp=$IFS
  IFS=$'\n'
  directories=( $(find ./ -mindepth 1 -maxdepth 1  -type d) ) # create array of source files
IFS=$temp

BuildTool="${HOME}/Desktop/C++ Development Root/Build.sh"
BaselinePath="${HOME}/Desktop/BaselineFiles"



processDelivery()
{
  echo "Setting working directory to:  $dirName"
  cd "$dirName"

  programRoot="$(find ./ -name main.cpp -exec dirname {} +)"
  programRoot="${programRoot:-.}"

  # Overwrite files that should not have been changed by the student with master file provided by instructor.
  echo -e  "\nStudent files found:"
  tree -I job.log

  #  Still working on this part.  This section has to be modified for each assignment.
  #  I'd like a way to determine or read this list of files from an external source.
  echo -e  "\nRemoving student files:"
  rm -rdf .git .vscode  # Might see this if students are submitting through GitHub Classroom.  Benign if submitting through TITANium
  # Remove everything except ...
  find . -type f -not \( -name  job.log                 -or \
                         -iname GroceryItem.cpp         -or \
                         -iname main.cpp                -or \
                         -iname  \*.pdf                 -or \
                         -iname output.txt              -or \
                         -iname README.\*                   \
                      \) -exec rm -v {} +



  if [[ -x "${BaselinePath}/prepareStudentFiles" ]]; then
    echo -e  "\nExtracting student content from:"
    # Find soultion files ...
    # temporarily ignore spaces when globing words into file names
    temp=$IFS
      IFS=$'\n'
      solutionFiles=( $(find "${BaselinePath}"/Solution -type f) ) # create array of solution source files
    IFS=$temp

    for solutionFileName in "${solutionFiles[@]}"; do
      #  See Parameter Expansion section of Bash man page for "##"'s' Remove matching prefix pattern
      #  behavior (https://linux.die.net/man/1/bash)
      studentFile=( $(find . -type f -iname "${solutionFileName##${BaselinePath}/Solution/}" ) )
      if [[ -f "${studentFile}" ]]; then
        "${BaselinePath}/prepareStudentFiles"   "${solutionFileName}"   "${studentFile}"  >  "${studentFile}".extracted_content  2>/dev/null

        Status=$?
        if [[ $Status == 0 ]]; then
          echo "extracted '${studentFile}'"
          diff -bZB "${studentFile}".extracted_content  "${studentFile}" > ${studentFile}.dif
          Status=$?
          if [[ $Status != 0 ]]; then
            echo "  ** modification to content outside designated TO-DO regions has been detected and discarded. See ${studentFile}.dif"
          else
            rm -f ${studentFile}.dif
          fi
          mv -f "${studentFile}".extracted_content  "${studentFile}"
        else
          echo "FAILED:  extraction form ${studentFile} failed"
          rm -f "${studentFile}".extracted_content
        fi
      else
        echo "MISSING:  ${solutionFileName##${BaselinePath}/Solution/} not found with delivery"
      fi
    done
    echo ""
  fi


  echo -e  "\nAdding baseline files:"
  cp -v -f -R "${BaselinePath}"/source/*  "${programRoot}"
  cp -v -f -R "${BaselinePath}"/data/*    ./

  echo -e "\nCompiling and executing delivery"
  # allowing just a few seconds (10 or so) is usually more than enough,  Sometimes it takes more
  "$BuildTool" "$assignmentName" > compile.log 2>&1   &&   timeout --foreground -k 10.5s 10.0s "./${assignmentName}_(clang++).exe" < input.txt > execution.log 2>&1;

  Status=$?
  if [[ $Status == 0 ]]; then
    echo "Compile and execution completed successfully"
  elif [[ $Status == 124 ]]; then
    echo "FATAL: Program timed out and administratively terminated - possible infinite loop"
  else
    echo "FATAL: compile or execution terminated with errors"
  fi

  # Clean up
  echo -e "\nClean up:"
  rm -frv "./${assignmentName}_(g++).exe"  "./${assignmentName}_(clang++).exe"  "./RegressionTests"
  for dataFile in $( find "${BaselinePath}"/data -type f ); do
    rm -fv ./"${dataFile##${BaselinePath}/data/}"
  done

}





echo "compiling ..."
for dirName in "${directories[@]}"; do
  echo "  $dirName"
  (processDelivery) > "${dirName}/job.log" 2>&1
done
echo ""
