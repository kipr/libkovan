echo "Generating Wrappers..."
echo -ne "\tPython... "
swig -python -c++ -outdir python -o python/kovan_python.cpp -I../include kovan.ipp
swig -python -outdir python -o python/kovan_python.c -I../include kovan.i
echo "Done!"
echo -ne "\tJava... "
swig -java -c++ -outdir java -o java/kovan_java.cpp -I../include kovan.ipp
echo "Done!"
echo -ne "\tCLISP... "
swig -clisp -outdir clisp -o clisp/kovan_clisp.cpp -I../include kovan.i
echo "Done!"

echo -ne "\tD... "
swig -d -c++ -outdir d -o d/kovan_d.cpp -I../include kovan.ipp
echo "Done!"