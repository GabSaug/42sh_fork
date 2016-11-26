#!/bin/sh

# Show line longer than 80 characters
# Show fuction whose body span over more than 25 lines
#    -empty line and single line comment are skipped
#    -multi line comment are skipped if they are at the begining of the line
# Warn when more than 10 functions are declared in on file
# Show trailing space

## TODO
# Whitespace after ( and before )
# logical operator at end of line && ||

#if [ $# -eq 0 ]; then
#  echo "Usage: give file to inspect in argument"
#  exit 1
#fi
WHITE_ITALIC='\033[2;39m'
RED='\033[0;31m'
GREEN='\033[0;32m'
GREEN_I='\033[3;32m'
BLUE='\033[1;34m'
LIGHT_CYAN='\033[0;36m'
PURPLE='\033[0;35m'
YELLOW='\033[0;33m'
ORANGE='\033[7;33m'
BLACK_on_WHITE='\033[0;30;47m'
NC='\033[0m' # No Color

#0: normal  1:bold 2: thin 3: Italic 4: Underline 5: Blink 7: Higllight
#8: Strike

FILE_NAME=${BLACK_on_WHITE}
LINE_NO=${WHITE_ITALIC}

TRAIL_SP=${YELLOW}

SL_COM=${LIGHT_CYAN}
ML_COM=${LIGHT_CYAN}

LINE_TOO_LONG=${GREEN}
MORE_10_FUN=${BLUE}
FUN_TOO_LONG=${RED}


show_sl_com=0
show_ml_com=0
show_len_in_limit_fun=0

if [ "$1" == "-v" ]; then
  show_sl_com=1
  show_ml_com=1
  show_len_in_limit_fun=1
  shift
fi

print_line()
{
  echo -ne "${LINE_NO}${1-${no_line}}${NC}\t"
}

check_file()
{
  echo -e "\t\t${FILE_NAME} $1 ${NC}"
  no_line=0
  size_fun=-1
  nb_fun=0
  ml_com=0
  while IFS= read -r line; do
    no_line=$(( no_line + 1 ))
    #echo "line ${no_line} ${fun_name} has ${size_fun} lines: ${line}"
    
    if [ ${#line} -gt 80 ]; then # 80 columns
      print_line; echo -e "${LINE_TOO_LONG} (${#line} c) : ${line}${NC}"
    fi
    echo ${line} | egrep "^ */\*" &> /dev/null
    if [ $? -eq 0 ]; then
      if [ ${show_ml_com} -eq 1 ]; then
        print_line; echo -e "${ML_COM}MULTILINE COMMENT ${line}${NC}"
      fi
      ml_com=1
    fi

    echo ${line} | egrep "\*/" &> /dev/null
    if [ $? -eq 0 ]; then
      if [ ${show_ml_com} -eq 1 ]; then
        echo -e "${ML_COM}MULTILINE COMMENT END ${line}${NC}"
      fi
      ml_com=0
      continue
    fi
    if [ ${ml_com} -eq 1 ]; then
      continue
    fi

    echo "${line}" | egrep "\( " &> /dev/null
    if [ $? -eq 0 ]; then
      print_line; echo -e "${TRAIL_SP}SPACE AFTER ( ${line%size}${NC}"
    fi
    echo "${line}" | egrep " \)" &> /dev/null
    if [ $? -eq 0 ]; then
      print_line; echo -e "${TRAIL_SP}SPACE BEFORE ) ${line}${NC}"
    fi


    echo "${line}" | egrep " +$" &> /dev/null
    if [ $? -eq 0 ]; then
      print_line; echo -e "${TRAIL_SP}TRAILING SPACE ${line}${NC}"
    fi
    


    if [ "${line}" == "};" ]; then # not a function (struct, static tab)
      nb_fun=$(( nb_fun - 1 ))
      size_fun=0
    fi

    if [ "${line}" == "}" ]; then ## 25 lines per function
      if [ $show_len_in_limit_fun -eq 1 ] || [ ${size_fun} -gt 25 ] || [ ${size_fun} -le 0 ]; then
        print_line ${line_start_fun}
        if [ ${size_fun} -gt 25 ] || [ ${size_fun} -le 0 ]; then
          echo -en "${FUN_TOO_LONG}"
        fi
        if [ ${size_fun} -gt 1 ]; then
          plural=s
        else
          plural=
        fi
        echo -e "${fun_name} has ${size_fun} line${plural}${NC}"
      fi
      # (end at line ${no_line})"
      #fi
      size_fun=-1
    fi
    if [ ${size_fun} -ge 0 ]; then
      #line_no_space="$(echo -e "${line}" | tr -d '[[:space:]]')"
      #echo "${line_no_space}"
      #if [ "${line_no_space}" != "" ]; then

      #if [ ${line}
      echo ${line} | egrep "^ *(//|$)" &> /dev/null #continue
      if [ $? -eq 1 ] && [ ${ml_com} -eq 0 ]; then
        size_fun=$(( size_fun + 1 ))
      fi
      echo ${line} | egrep "//" &> /dev/null #continue
      if [ $? -eq 0 ] && [ ${show_sl_com} -eq 1 ]; then
        print_line; echo -e "${SL_COM}SINGLE LINE COMMENT ${line}${NC}"
      fi

    fi
    if [ "${line}" == "{" ]; then
      #echo "START_OF_FUNCTION ${prev_line}"
      fun_name1=${prev_line%%(*}
      fun_name=${fun_name1##* }
      line_start_fun=${no_line}
      nb_fun=$(( nb_fun + 1 ))
      size_fun=0
    fi
    
    
    echo "${line}" | egrep "^ +" &> /dev/null
    if [ $? -eq 1 ]; then
      prev_line=${line}
    fi
  done < $1
  if [ ${nb_fun} -gt 10 ]; then
    echo -e "${MORE_10_FUN}MORE THAN 10 FUNCTIONS : ${nb_fun} functions${NC}"
  fi
}

if [ $# -eq 0 ]; then
  for file in $(find -name '*.c'); do
    check_file "${file#./}"
  done
  exit 0
fi

for arg in $@; do
  if [ -d ${arg} ]; then
    #echo "dir"
    for file in $(find ${arg} -name "*.c"); do
      #echo "file = ${file}"
      check_file "${file}"
    done
  elif [ -f ${arg} ]; then
    #echo "file"
    check_file "${arg}"
  else
    echo "${arg} no such file or directory"
  fi
done
