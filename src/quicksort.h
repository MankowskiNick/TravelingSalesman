// Nick Mankowski
// 9/8/22
// Quicksort Library

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>

// "Static class" rather than separate functions so that we can keep all of our "typename T"'s in order
// without the syntax getting less readable
template<typename T>
class QuickSort {
    public:

        // Public sort function
        // Pre:  The unsorted list of type T
        // Post: Void, but sorts the list of type T listed
        //       in the parameter.
        static void Sort(std::vector<T>& list) {
            Partition(0, list.size(), list);
        }
        
    private: 

        // Pre:  Two objects of type T to swap the value of
        // Post: Void, simply swaps the two parameter values.
        static void Swap(T& swap1, T& swap2) {
            T temp = swap1;
            swap1 = swap2;
            swap2 = temp;
        }

        // Recursive quicksort helper
        // Pre:  The list to partition, the 'low' index value of the list, the 'high'  
        //       index value of the list, these variables defint the segment of the list we are focusing on.
        // Post: Returns void, eventually will sort the list in the parameter 'list'
        static void Partition(int low, int high, std::vector<T>& list) {

            // If we are looking at a valid range
            if (low < high) {

                // Pick a pivot element
                T pivot = list[low];

                // The last element <= pivot
                int last_small = low;

                // Look through all the unseen elements
                for (int i = low + 1; i < high; i++) {

                    // If the item is less than the pivot, increment last_small and
                    // swap the element at that position for the one we are looking at
                    if (list[i] <= pivot) {
                        last_small++;
                        Swap(list[i], list[last_small]);
                    }

                    // If the item is greater than the pivot, we don't it can stay to the
                    // right, we don't need to do anything
                }

                // Put the pivot element in it's proper place
                Swap(list[last_small], list[low]);

                // Recursively partition the half < the pivot element
                Partition(low, last_small, list);

                // Recursively partition the half > the pivot element
                Partition(last_small + 1, high, list);
            }
        }
};

#endif