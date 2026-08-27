#pragma once

#include<iostream>
#include<vector>

using namespace std;

template<typename T>
class PQ
{
private:
    vector<T> data;
    int (*ptr2func)(T,T);
    int (*ptr2search_func)(T,T);

public:

    PQ(int (*ptr2func)(T,T), int (*ptr2search_func)(T,T))
    {
        this->ptr2func = ptr2func;
        this->ptr2search_func = ptr2search_func;
    }

    void push(T element)
    {
        data.push_back(element);

        int child_index = data.size() - 1;
        int parent_index;
        T g;

        while(child_index > 0)
        {
            parent_index = (child_index - 1) / 2;

            if(!ptr2func(data[child_index], data[parent_index]))
            {
                g = data[child_index];
                data[child_index] = data[parent_index];
                data[parent_index] = g;

                child_index = parent_index;
            }

            else
            {
                break;
            }
        }
    }

    void pop()
    {
        if(data.size() == 0)
            throw string("queue is empty");

        if(data.size() == 1)
        {
            data.resize(0);
            return;
        }

        int upper_bound = data.size() - 1;
        data[0] = data[upper_bound];
        data.resize(data.size() - 1);

        int parent_index = 0;
        int left_child_index;
        int right_child_index;
        int compare_with_index;
        T g;

        while(parent_index < data.size())
        {
            left_child_index = parent_index * 2 + 1;
            right_child_index = left_child_index + 1;

            if(left_child_index >= data.size())
                break;

            if(right_child_index >= data.size())
            {
                compare_with_index = left_child_index;
            }

            else
            {
                if(!ptr2func(data[left_child_index], data[right_child_index]))
                {
                    compare_with_index = right_child_index;
                }

                else
                {
                    compare_with_index = left_child_index;
                }
            }

            if(!ptr2func(data[compare_with_index], data[parent_index]))
            {
                g = data[compare_with_index];
                data[compare_with_index] = data[parent_index];
                data[parent_index] = g;

                parent_index = compare_with_index;
            }

            else
            {
                break;
            }
        }
    }

    T top()
    {
        if(data.size() == 0)
            throw string("queue is empty");

        return data[0];
    }

    bool empty()
    {
        return data.size() == 0;
    }

    T at(int index)
    {
        return data[index];
    }

    int find(T element)
    {
        int idx;

        for(idx = 0; idx < data.size(); ++idx)
        {
            if(this->ptr2search_func == NULL)
                return -1;

            if(this->ptr2search_func(data[idx], element))
                return idx;
        }

        return -1;
    }

    void update(T element, int idx)
    {
        if(idx < 0 || idx >= data.size())
            return;

        data[idx] = element;

        int child_index = idx;
        int parent_index;
        T g;

        while(child_index > 0)
        {
            parent_index = (child_index - 1) / 2;

            if(!ptr2func(data[child_index], data[parent_index]))
            {
                g = data[child_index];
                data[child_index] = data[parent_index];
                data[parent_index] = g;

                child_index = parent_index;
            }

            else
            {
                break;
            }
        }
    }
};