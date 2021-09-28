echo "--- TESTS CD & PWD ---"

pwd
mkdir ftest
cd ./ftest
pwd
cd ..
"cd .."
rm -rf ./ftest
cd ./ftest
cd .
pwd