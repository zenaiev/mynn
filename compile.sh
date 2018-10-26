#g++ run.cxx nn.cxx data.cxx -o run -g -std=c++11 `root-config --cflags --libs` -lMinuit
g++ run-titanic.cxx nn.cxx data.cxx -o run-titanic -g -std=c++11 `root-config --cflags --libs` -lMinuit
