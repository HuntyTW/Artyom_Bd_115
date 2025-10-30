CPP_FILES="time.cpp generateGapSequence.cpp tools.cpp shellSort.cpp main.cpp"
APP=example

if [ -f $APP ]; then rm $APP
fi

g++ $CPP_FILES -o $APP

./$APP