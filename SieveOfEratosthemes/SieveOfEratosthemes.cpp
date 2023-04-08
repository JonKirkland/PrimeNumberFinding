#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

class PrimeSieve {
public:
    //function which takes: the number to search to (numberCap) and vector "primes" which will store prime numbers
    void generatePrimes(int numberCap, std::vector<int>& primes) {
        //initialize boolean vector
        std::vector<bool> isPrime(numberCap + 1, true);
        //make sure primes vector is clear
        primes.clear();
        //start from 2:
        isPrime[0] = false;
        isPrime[1] = false;

        //due to the algorithm, only need to check for primes  up to the square root of the numbercap 
        int sqrtCap = static_cast<int>(std::sqrt(numberCap));
        //i represents current prime number "candidate"
        for (int i = 2; i <= sqrtCap; ++i) {
            if (isPrime[i]) {
                //if i = prime, eliminate multiples of i 
                for (int j = i * i; j <= numberCap; j += i) {
                    isPrime[j] = false;
                }
            }
        }
        //go back through and add all primes to prime vector
        primes.clear();
        for (int i = 2; i <= numberCap; ++i) {
            if (isPrime[i]) { 
                primes.push_back(i);
            }
        }
    }
};
//write the vector of ints to a text file
void WriteToFile(std::string path, std::vector<int> primes) {
    std::ofstream output_file(path);
    //if can't find fail
    if (!output_file) {
        std::cerr << "Unable to open file: " << path << std::endl;
        return;
    }
    for (int prime : primes) {
        output_file << prime << std::endl;
    }
    std::cout << "primes outputted to file\n";
}
int main() {
    int numberCap;
    std::cout << "Number cap: ";
    std::cin >> numberCap;

    PrimeSieve primeSieve;
    std::vector<int> primes;
    primeSieve.generatePrimes(numberCap, primes);

    std::string filename = "primes.txt";
    WriteToFile(filename, primes);

    //std::cout << "Primes up to " << numberCap << " are:\n";
    //for (int prime : primes) {
    //    std::cout << prime << " ";
    //}

    return 0;
}