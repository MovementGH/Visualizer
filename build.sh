cd Build
g++ -c -g ../Source/*.cpp -w
g++ *.o -g -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o ../Product/Visualizer
