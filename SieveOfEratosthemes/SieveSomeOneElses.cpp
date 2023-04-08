//NOTE THIS CODE WAS TAKEN FROM https://iq.opengenus.org/multi-thread-cpp-find-all-prime-numbers/
//AS A BENCHMARK AGAINST MY OWN CODE

//THIS CODE HAS BEEN MODIFIED BY ME TO ONLY USE 5 THREADS, SEE RESULTS.TXT

#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <math.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

class Prime
{
    int** primes;
    int N;
public:
    Prime(int N)
    {
        this->N = N;

        primes = new int* [N];

        for (int i = 0; i < N; i++)
            primes[i] = new int[2],
            primes[i][0] = i,
            primes[i][1] = 1; // mark all as prime numbers
    }
    void eliminate(int start, int end)
    {
        for (int p = start; p < end; ++p) {
            int i = p + p;
            while (i < N)
                primes[i][1] = 0, // mark the multiplicies of p as not primes 
                i += p;
        }
    }
    void display()
    {
        for (int i = 1; i < N; i++)
        {
            if (primes[i][1] == 1)
                cout << primes[i][0] << "\n";
        }
        cout << endl;
    }
    void save(string file)
    {
        ofstream fout(file);

        if (fout.is_open())
        {
            for (int i = 1; i < N; i++)
            {
                if (primes[i][1] == 1)
                    fout << primes[i][0] << " ";

                if (i % 10 == 0) fout << endl;
            }
            fout << endl;

            fout.close();
        }

        else
            cout << "cannot open file " << file;
    }
};

int main()
{

    vector<thread> v;

    int N;
    cout << "genereate primes numbers less than N = ";
    cin >> N;
    auto timer1 = high_resolution_clock::now();
    Prime p(N);
    int numThreads = 5;
    int sqrtN = static_cast<int>(sqrt(N));
    int rangeSize = (sqrtN - 2 + numThreads - 1) / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = 2 + i * rangeSize;
        int end = min(sqrtN, 2 + (i + 1) * rangeSize);
        v.push_back(thread(&Prime::eliminate, ref(p), start, end));
    }

    for (auto& th : v) th.join();

    //p.display();
    p.save("primes.txt");
    auto timer2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(timer2 - timer1);
    duration<double, std::milli> ms_double = timer2 - timer1;
    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";

    return 0;
}