mkdir Build
cd Build
g++ -c -g ../Source/*.cpp -w
g++ -c -g ../SelbaWard/Sprite3d.cpp -w
g++ *.o -g -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o ../Product/Visualizer
cd ..
rm Build -r