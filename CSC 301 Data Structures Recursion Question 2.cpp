/*
 * CSC 301 - Data Structures
 * Question 2: Solving Problems Using Recursion
 *
 * Four problems solved with recursive functions:
 *   1. Factorial of a number
 *   2. The first n Fibonacci numbers
 *   3. Reversing a string
 *   4. Binary search on a sorted array
 *
 * Every function is tested with at least three inputs, including edge cases.
 *
 * Compile:  g++ -std=c++17 -Wall -Wextra -o q2_recursion q2_recursion.cpp
 * Run:      ./q2_recursion
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/* ==========================================================================
 * 1. FACTORIAL
 *    n! = n * (n - 1) * ... * 1, and 0! = 1 by definition.
 *
 *    Base case:      n == 0 (or n == 1) -> return 1
 *    Recursive case: n > 1              -> return n * factorial(n - 1)
 *
 *    Each call reduces n by 1, so the recursion always reaches the base case.
 *    unsigned long long holds results up to 20! (2,432,902,008,176,640,000);
 *    21! overflows, so larger inputs are rejected. Negative numbers have no
 *    factorial, so they are rejected too.
 * ========================================================================== */
unsigned long long factorial(int n) {
    if (n < 0) {
        throw invalid_argument("factorial is not defined for negative numbers");
    }
    if (n > 20) {
        throw overflow_error("factorial of " + to_string(n) +
                             " is too large for unsigned long long (max is 20!)");
    }

    // Base case: 0! = 1 and 1! = 1. This stops the recursion.
    if (n <= 1) {
        return 1;
    }

    // Recursive case: n! = n * (n - 1)!
    return static_cast<unsigned long long>(n) * factorial(n - 1);
}

/* ==========================================================================
 * 2. FIBONACCI
 *    F(0) = 0, F(1) = 1, F(k) = F(k - 1) + F(k - 2)
 *    Sequence: 0, 1, 1, 2, 3, 5, 8, 13, ...
 *
 *    fibonacci(k) returns the k-th Fibonacci number.
 *      Base cases:     k == 0 -> 0,  k == 1 -> 1
 *      Recursive case: k >= 2 -> fibonacci(k - 1) + fibonacci(k - 2)
 *
 *    generateFibonacci(n, sequence) builds the FIRST n numbers recursively.
 *      Base case:      n == 0 -> nothing to add, stop
 *      Recursive case: first generate the first (n - 1) numbers,
 *                      then append fibonacci(n - 1) as the n-th number.
 *
 *    Note: plain recursive fibonacci(k) recomputes the same values many times,
 *    so it takes exponential time. It is fine for small n, which is what this
 *    exercise demonstrates.
 * ========================================================================== */
long long fibonacci(int k) {
    // Base cases: the first two numbers of the sequence.
    if (k == 0) return 0;
    if (k == 1) return 1;

    // Recursive case: each number is the sum of the two before it.
    return fibonacci(k - 1) + fibonacci(k - 2);
}

void generateFibonacci(int n, vector<long long>& sequence) {
    if (n < 0) {
        throw invalid_argument("n must not be negative");
    }

    // Base case: zero numbers requested, so there is nothing to do.
    if (n == 0) {
        return;
    }

    // Recursive case: build the first n - 1 numbers, then add the n-th one.
    generateFibonacci(n - 1, sequence);
    sequence.push_back(fibonacci(n - 1));   // the n-th number is F(n - 1)
}

/* ==========================================================================
 * 3. STRING REVERSAL
 *    Swap the outermost pair of characters, then reverse the inside part.
 *
 *    reverseHelper(s, left, right) works on the range s[left .. right].
 *      Base case:      left >= right -> zero or one character left, stop
 *      Recursive case: swap s[left] and s[right],
 *                      then reverse s[left + 1 .. right - 1]
 *
 *    reverseString(text) makes a copy and reverses it with the helper,
 *    so the caller's original string is not changed.
 * ========================================================================== */
void reverseHelper(string& s, int left, int right) {
    // Base case: the pointers have met or crossed.
    if (left >= right) {
        return;
    }

    // Recursive case: swap the two ends, then move both ends inward.
    char temp = s[left];
    s[left] = s[right];
    s[right] = temp;
    reverseHelper(s, left + 1, right - 1);
}

string reverseString(const string& text) {
    string result = text;
    reverseHelper(result, 0, static_cast<int>(result.length()) - 1);
    return result;
}

/* ==========================================================================
 * 4. BINARY SEARCH
 *    Searches a SORTED array by comparing the target with the middle element
 *    and discarding the half that cannot contain it.
 *
 *    binarySearch(arr, target, low, high) searches arr[low .. high].
 *      Base case 1: low > high          -> range is empty, return -1 (not found)
 *      Base case 2: arr[mid] == target  -> return mid (found)
 *      Recursive case:
 *        target < arr[mid] -> search the left half  arr[low .. mid - 1]
 *        target > arr[mid] -> search the right half arr[mid + 1 .. high]
 *
 *    mid is computed as low + (high - low) / 2 to avoid integer overflow.
 *    An empty array starts with low = 0 and high = -1, so base case 1
 *    returns -1 immediately.
 * ========================================================================== */
int binarySearch(const vector<int>& arr, int target, int low, int high) {
    // Base case 1: nothing left to search.
    if (low > high) {
        return -1;
    }

    int mid = low + (high - low) / 2;

    // Base case 2: the target is at the middle.
    if (arr[mid] == target) {
        return mid;
    }

    // Recursive case: search only the half that can contain the target.
    if (target < arr[mid]) {
        return binarySearch(arr, target, low, mid - 1);
    }
    return binarySearch(arr, target, mid + 1, high);
}

// Convenience wrapper that searches the whole array.
int binarySearch(const vector<int>& arr, int target) {
    return binarySearch(arr, target, 0, static_cast<int>(arr.size()) - 1);
}

/* ------------------------------------------------------------------------
 * Test helpers
 * ---------------------------------------------------------------------- */

void printHeader(const string& title) {
    cout << endl;
    cout << "============================================================" << endl;
    cout << title << endl;
    cout << "============================================================" << endl;
}

void testFactorial(int n) {
    cout << "factorial(" << n << ") = ";
    try {
        cout << factorial(n) << endl;
    } catch (const exception& e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

void testFibonacci(int n) {
    cout << "First " << n << " Fibonacci numbers: ";
    try {
        vector<long long> sequence;
        generateFibonacci(n, sequence);
        if (sequence.empty()) {
            cout << "(none)";
        }
        for (size_t i = 0; i < sequence.size(); i++) {
            cout << sequence[i];
            if (i + 1 < sequence.size()) cout << ", ";
        }
        cout << endl;
    } catch (const exception& e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

void testReverse(const string& text) {
    cout << "reverse(\"" << text << "\") = \"" << reverseString(text) << "\"" << endl;
}

void printArray(const vector<int>& arr) {
    cout << "[";
    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < arr.size()) cout << ", ";
    }
    cout << "]";
}

void testBinarySearch(const vector<int>& arr, int target) {
    cout << "Search for " << target << " in ";
    printArray(arr);
    int index = binarySearch(arr, target);
    if (index == -1) {
        cout << " -> not found (returned -1)" << endl;
    } else {
        cout << " -> found at index " << index << endl;
    }
}

int main() {
    printHeader("1. FACTORIAL");
    testFactorial(0);     // edge case: 0! = 1
    testFactorial(1);     // edge case: smallest positive input
    testFactorial(5);     // typical: 120
    testFactorial(10);    // typical: 3628800
    testFactorial(20);    // largest value that fits in unsigned long long
    testFactorial(21);    // error: overflow
    testFactorial(-4);    // error: negative input

    printHeader("2. FIBONACCI SEQUENCE");
    testFibonacci(0);     // edge case: empty sequence
    testFibonacci(1);     // edge case: just F(0)
    testFibonacci(2);     // edge case: both base cases
    testFibonacci(10);    // typical
    testFibonacci(20);    // larger input
    testFibonacci(-3);    // error: negative input

    printHeader("3. STRING REVERSAL");
    testReverse("");               // edge case: empty string
    testReverse("A");              // edge case: single character
    testReverse("data");           // even length
    testReverse("recursion");      // odd length
    testReverse("racecar");        // palindrome reads the same
    testReverse("CSC 301 TMA!");   // spaces, digits and punctuation

    printHeader("4. BINARY SEARCH");
    vector<int> empty;
    vector<int> single = {7};
    vector<int> sorted = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    vector<int> withNegatives = {-40, -12, -3, 0, 9, 27};

    testBinarySearch(empty, 5);          // edge case: empty array
    testBinarySearch(single, 7);         // edge case: one element, found
    testBinarySearch(single, 3);         // edge case: one element, not found
    testBinarySearch(sorted, 23);        // typical: found in the middle area
    testBinarySearch(sorted, 2);         // first element
    testBinarySearch(sorted, 91);        // last element
    testBinarySearch(sorted, 50);        // not present
    testBinarySearch(withNegatives, -12);// negative numbers
    testBinarySearch(withNegatives, 0);  // zero

    cout << endl << "All recursion tests finished." << endl;
    return 0;
}
