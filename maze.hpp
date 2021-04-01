#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "list.hpp"

#define WALL "\u2503"
#define FLOOR " \u2501\u2501"

using namespace std;

class maze
{
    private:
        list *root;
        list *node;
        int width, height;
        vector <int> enter, exit, sizes;
        vector <int> coords = {0, 0};
        vector <int> ceil = {2, 3, 6, 10, 11, 7, 14, 15};
        vector <int> wall = {1, 3, 9, 5, 11, 7, 13, 15};
        vector <int> blocked_ways(list *node);
        int finished();

    public:
        maze(string txt);
        void explore();
        void print_map();
};