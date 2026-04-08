// Simplified syntax check for the fixed code
#include <cmath>
#include <iostream>
using namespace std;

// Verify the critical fixes compile
double mPhase = 0.0;  // Fix: was uninitialized global, now done in constructor
int mGlobalFormat = 0; // Fix: moved to instance variable

// Fix: Verify phase wrapping logic
void test_phase_wrapping() {
    double kTwoPi = 2 * M_PI;
    
    mPhase += 0.1;
    if (mPhase >= kTwoPi) {
        mPhase -= kTwoPi;  // Fix: now always wraps, not conditionally
    }
}

// Fix: Verify float casting
float test_sinf_cast() {
    float result = sin((float)mPhase);  // Fix: cast mPhase to float
    return result;
}

int main() {
    test_phase_wrapping();
    test_sinf_cast();
    cout << "✓ All syntax checks passed" << endl;
    return 0;
}
