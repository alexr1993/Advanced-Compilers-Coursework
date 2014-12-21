echo "Frontend"
./run_tests -f | tail -n 1
echo "Interpreter"
./run_tests -i | tail -n 1
