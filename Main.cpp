
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

struct Result
{
    long long comparisons = 0;
    long long swaps = 0;
    double time = 0;
};

// SHELL SORT 

Result shellSort(vector<int> a)
{
    Result r;

    auto start = chrono::high_resolution_clock::now();

    int n = a.size();

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = a[i];
            int j = i;

            while (j >= gap)
            {
                r.comparisons++;

                if (a[j - gap] > temp)
                {
                    a[j] = a[j - gap];
                    r.swaps++;
                    j -= gap;
                }
                else
                {
                    break;
                }
            }

            a[j] = temp;
        }
    }

    auto end = chrono::high_resolution_clock::now();

    r.time = chrono::duration<double, milli>(end - start).count();

    return r;
}

//  GNOME SORT 

Result gnomeSort(vector<int> a)
{
    Result r;

    auto start = chrono::high_resolution_clock::now();

    int i = 0;

    while (i < a.size())
    {
        if (i == 0)
        {
            i++;
        }
        else
        {
            r.comparisons++;

            if (a[i] >= a[i - 1])
            {
                i++;
            }
            else
            {
                swap(a[i], a[i - 1]);
                r.swaps++;
                i--;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();

    r.time = chrono::duration<double, milli>(end - start).count();

    return r;
}

// TIMSORT 

Result timSort(vector<int> a)
{
    Result r;

    auto start = chrono::high_resolution_clock::now();

    int n = a.size();
    const int RUN = 32;

    
    for (int i = 0; i < n; i += RUN)
    {
        int left = i;
        int right = min(i + RUN - 1, n - 1);

        for (int j = left + 1; j <= right; j++)
        {
            int temp = a[j];
            int k = j - 1;

            while (k >= left)
            {
                r.comparisons++;

                if (a[k] > temp)
                {
                    a[k + 1] = a[k];
                    r.swaps++;
                    k--;
                }
                else
                {
                    break;
                }
            }
            a[k + 1] = temp;
        }
    }

    
    for (int size = RUN; size < n; size = 2 * size)
    {
        for (int left = 0; left < n; left += 2 * size)
        {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));

            
            if (mid >= right)
                continue;

            int len1 = mid - left + 1;
            int len2 = right - mid;

            vector<int> leftArr(len1);
            vector<int> rightArr(len2);

            for (int i = 0; i < len1; i++)
                leftArr[i] = a[left + i];

            for (int i = 0; i < len2; i++)
                rightArr[i] = a[mid + 1 + i];

            int i = 0, j = 0;
            int k = left;

            
            while (i < len1 && j < len2)
            {
                r.comparisons++;

                if (leftArr[i] <= rightArr[j])
                {
                    a[k] = leftArr[i];
                    i++;
                }
                else
                {
                    a[k] = rightArr[j];
                    j++;
                }
                r.swaps++; 
                k++;
            }

           
            while (i < len1)
            {
                a[k] = leftArr[i];
                r.swaps++;
                i++;
                k++;
            }

            
            while (j < len2)
            {
                a[k] = rightArr[j];
                r.swaps++;
                j++;
                k++;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();

    r.time = chrono::duration<double, milli>(end - start).count();

    return r;
}
//  МАСИВИ 

vector<int> randomArray(int n)
{
    vector<int> a(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100000);

    for (int i = 0; i < n; i++)
        a[i] = dist(gen);

    return a;
}

vector<int> sortedArray(int n)
{
    vector<int> a(n);

    for (int i = 0; i < n; i++)
        a[i] = i + 1;

    return a;
}

vector<int> reverseArray(int n)
{
    vector<int> a(n);

    for (int i = 0; i < n; i++)
        a[i] = n - i;

    return a;
}

// РЕЗУЛЬТАТ 

void printResult(string algorithm, string type, int size, Result r)
{
    cout << left
        << setw(15) << algorithm
        << setw(12) << type
        << setw(8) << size
        << setw(15) << r.comparisons
        << setw(15) << r.swaps
        << fixed << setprecision(3)
        << r.time << " ms\n";
}

// MAIN 

int main()
{
    cout << "SORTING ALGORITHMS COMPARISON\n\n";

    cout << left
        << setw(15) << "Algorithm"
        << setw(12) << "Data"
        << setw(8) << "Size"
        << setw(15) << "Comparisons"
        << setw(15) << "Swaps"
        << "Time\n";

    cout << string(80, '-') << "\n";

    int sizes[] = { 100, 1000, 10000 };

    for (int n : sizes)
    {
        vector<int> random = randomArray(n);
        vector<int> sorted = sortedArray(n);
        vector<int> reversed = reverseArray(n);

        // Випадковий масив
        printResult("Shell", "Random", n, shellSort(random));
        printResult("Gnome", "Random", n, gnomeSort(random));
        printResult("TimSort", "Random", n, timSort(random));

        // Впорядкований масив
        printResult("Shell", "Sorted", n, shellSort(sorted));
        printResult("Gnome", "Sorted", n, gnomeSort(sorted));
        printResult("TimSort", "Sorted", n, timSort(sorted));

        // Зворотний масив
        printResult("Shell", "Reverse", n, shellSort(reversed));
        printResult("Gnome", "Reverse", n, gnomeSort(reversed));
        printResult("TimSort", "Reverse", n, timSort(reversed));

        cout << "\n";
    }

    return 0;
}

