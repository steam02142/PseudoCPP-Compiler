#include <vector>
#include <string>
#include <iostream>

using namespace std;

void swap(vector<double>& arr, long i, long j) {
   double temp = arr[i];
   arr[i] = arr[j];
   arr[j] = temp;
}


void bubbleSort(vector<double>& arr, long size) {
   long outerBound = (size - 1);
   for (int i = 0; i < outerBound; i++) {
         long innerBound = (outerBound - i);
         for (int j = 0; j < innerBound; j++) {
               long nextPos = (j + 1);
               if (arr[j] > arr[nextPos]) {
                     swap(arr, j, nextPos);
               }
         }

   }

}


void printArray(vector<double>& arr, long size) {
   for (int i = 0; i < size; i++) {
         cout << arr[i] << endl;
   }

}

int main() {
   vector<double> numbers = {5.2, 3.1, 8.4, 1.7, 9.3};
   long arrSize = 5;
   cout << "Original array:" << endl;
   printArray(numbers, arrSize);
   bubbleSort(numbers, arrSize);
   cout << "Sorted array:" << endl;
   printArray(numbers, arrSize);
}
