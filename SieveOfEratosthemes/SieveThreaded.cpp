#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <chrono>

using namespace std::chrono;
class PrimeSieve {
public:
    void generatePrimes(int start, int numberCap, std::vector<int>& primes) {
        std::vector<bool> isPrime(numberCap - start + 1, true);

        int sqrtCap = static_cast<int>(std::sqrt(numberCap));

        for (int i = 2; i <= sqrtCap; ++i) {
            for (int j = std::max(i * i, (start + i - 1) / i * i); j <= numberCap; j += i) {
                isPrime[j - start] = false;
            }
        }

        if (start == 0) {
            isPrime[0] = false;
            isPrime[1] = false;
        }
        else if (start == 1) {
            isPrime[0] = false;
        }

        for (int i = 0; i <= numberCap - start; ++i) {
            if (isPrime[i]) {
                primes.push_back(i + start);
            }
        }
    }
};

std::mutex vectorMutex;

void threadedPrimeSieve(int start, int limit, std::vector<int>& allPrimes) {
    PrimeSieve primeSieve;
    std::vector<int> primes;
    primeSieve.generatePrimes(start, limit, primes);

    std::unique_lock<std::mutex> lock(vectorMutex);
    allPrimes.insert(allPrimes.end(), primes.begin(), primes.end());
}

int main() {
    int numberCap, numThreads;
    std::cout << "Enter the limit: ";
    std::cin >> numberCap;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;
    auto timer1 = high_resolution_clock::now();
    int chunkSize = (numberCap + numThreads - 1) / numThreads;

    std::vector<std::thread> threads;
    std::vector<int> allPrimes;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = std::min(numberCap, (i + 1) * chunkSize - 1);
        threads.emplace_back(threadedPrimeSieve, start, end, std::ref(allPrimes));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Sort the prime numbers and remove duplicates 
    std::sort(allPrimes.begin(), allPrimes.end());
    auto last = std::unique(allPrimes.begin(), allPrimes.end());
    allPrimes.erase(last, allPrimes.end());

    // Write the prime numbers to a file
    std::string filename = "primes.txt";
    std::ofstream output_file(filename);

    if (!output_file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return 1;
    }

    for (int prime : allPrimes) {
        output_file << prime << std::endl;
    }

    output_file.close();
    auto timer2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(timer2 - timer1);
    duration<double, std::milli> ms_double = timer2 - timer1;
    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    std::cout << "Primes up to " << numberCap << " have been written to " << filename << std::endl;

    return 0;
}