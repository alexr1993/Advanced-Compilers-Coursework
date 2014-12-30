echo "Running Tests..."
echo "Frontend"
./run_tests -f | tail -n 1
echo "Interpreter"
./run_tests -i | tail -n 1
echo "TAC"
./run_tests -t | tail -n 1
echo "MIPS"
./run_tests -m | tail -n 1
