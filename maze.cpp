#include "maze.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <unistd.h>

int rev_dir(int dir)
{
    switch (dir)
        {
            case 1:
                return 3;
            case 2:
                return 4;
            case 3:
                return 1;
            case 4:
                return 2;
            default:
                return 0;
        }
}

bool is_in(int v, vector <int>ve)
{
    for (int i = 0; i < ve.size(); i++)
    {
        if (v == ve[i])
            return true;
    }
    return false;
}

vector <int> maze::blocked_ways(list *node)
{
    vector <int> v;
    if (node->move_left())
    {
        if (is_in(node->move_left()->get_value(), {4, 6, 12, 5, 7, 14, 13, 15}) || is_in(node->get_value(), {1, 3, 9, 5, 11, 13, 7, 15}))
            v.push_back(1);
    }
    else
        v.push_back(1);

    if (node->move_up())
    {
        if (is_in(node->move_up()->get_value(), {8, 12, 9, 10, 11, 14, 13, 15})|| is_in(node->get_value(), {2, 3, 6, 10, 11, 7, 14, 15}))
            v.push_back(2);
    }
    else
        v.push_back(2);
    
    if (node->move_right())
    {
        if (is_in(node->move_right()->get_value(), {1, 3, 9, 5, 11, 7, 13, 15}) || is_in(node->get_value(), {4, 6, 12, 5, 7, 14, 13, 15}))
            v.push_back(3);
    }
    else
        v.push_back(3);

    if (node->move_down())
    {
        if (is_in(node->move_down()->get_value(), {2, 3, 6, 10, 11, 7, 14, 15}) || is_in(node->get_value(), {8, 12, 9, 10, 11, 14, 13, 15}))
            v.push_back(4);
    }
    else
        v.push_back(4);
    return v;
}

maze::maze(string txt)
{
    ifstream in(txt);
    vector <int> nums;
    char v1;
    int v;
    int read = 8;
    int i = 0;
    while (i < read)
    {
        in >> v;
        nums.push_back(v);
        i++;
        if (i == 2)
            read += nums[0] * nums[1];
    }
    width = nums[0];
    height = nums[1];
    sizes = {width, height};
    enter = {nums[2], nums[3], nums[4]};
    exit = {nums[5], nums[6], nums[7]};
    root = new list(nums[8]);
    list *tmp = this->root;
    list *tmp1, *prev_line;
    vector <list *> roots;
    int pos = 9;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width -1; i++)
        {
            if (i == 0)
            {
                prev_line = tmp;
                roots.push_back(prev_line);
            }
            tmp -> set_right(new list(nums[pos]));
            tmp1 = tmp -> move_right();
            tmp1 -> set_left(tmp);
            tmp = tmp -> move_right();
            pos++;
        }
        if (j != height - 1)
        {
            prev_line -> set_down(new list(nums[pos]));
            tmp1 = prev_line -> move_down();
            tmp1 -> set_up(prev_line);
            prev_line = prev_line -> move_down();
            tmp = prev_line;
            pos++;
        }
    }
    pos = 0;
    while (pos < width - 1)
    {
        for (int i = 0; i < roots.size(); i++)
        {
            roots[i] = roots[i] -> move_right();
        }
        for (int j = 0; j < roots.size() - 1; j++)
        {
            roots[j] -> set_down(roots[j+1]);
            roots[j+1] -> set_up(roots[j]);
        }
        pos++;
    }
    coords = {0, 0};
    node = root;
}

int maze::finished()
{
    if (coords[0] == exit[0] && coords[1] == exit[1])
        return 0;
    else
        return 1;
}

void maze::explore()
{
    srand(time(0));
    int prev_dir = 0;
    int dir = 0;
    int step;
    vector <int> b_ways;
    while (cin>>step && finished())
    {
        system("clear");
        b_ways = blocked_ways(node);
        if (b_ways.size() == 3)
        {
            node->set_value(15);
            dir = 10 - b_ways[0] - b_ways[1] - b_ways[2];
        }
        else
        {
            while (is_in(dir, b_ways) || dir == rev_dir(prev_dir))
                dir = rand()%4+1;
        }
        switch (dir)
        {
            case 1:
                {node = node->move_left(); coords[0]--; break;}
            case 2:
                {node = node->move_up(); coords[1]--; break;}
            case 3:
                {node = node->move_right(); coords[0]++; break;}
            case 4:
                {node = node->move_down(); coords[1]++; break;}
        }
        prev_dir = dir;
        dir = rev_dir(prev_dir);
        print_map();
        cout << '*' << node -> get_value() << '*' << endl;
    }
    cout << "WE DID IT";
}

void maze::print_map()
{
    list *tmp = root;
    list *tmp_root = tmp; 
    bool flag = false;
    while (tmp_root)
    {
        while (1)
        {
            if (!flag && !tmp) 
            {
                tmp = tmp_root;
                flag = !flag;
                cout << endl;
            }
            else if (!flag && tmp)
            {
                if (is_in(tmp->get_value(), ceil))
                    cout << FLOOR;
                else
                    cout << "   ";    
                tmp = tmp -> move_right();
            }
            else if (flag && !tmp)
            {
                flag = !flag;
                cout << WALL << endl;
                break;
            }
            else
            {
                if (tmp -> get_value() >= 10)
                {
                    if (is_in(tmp->get_value(), wall))
                        cout << WALL << tmp -> get_value();
                    else
                        cout << ' ' << tmp -> get_value();
                }
                else
                {
                    if (is_in(tmp->get_value(), wall))
                        cout << WALL <<' ' << tmp -> get_value();
                    else
                        cout << "  " << tmp -> get_value();
                }
                tmp = tmp -> move_right();
            }
        }
        tmp = tmp_root -> move_down();
        tmp_root = tmp_root -> move_down();
    }
    for (int i = 0; i < sizes[0]; i++)
    {
        if (i != exit[0])
        {
            cout << FLOOR;
        }
        else
        {
            cout << "   ";
        }
    }
    cout << endl;
}