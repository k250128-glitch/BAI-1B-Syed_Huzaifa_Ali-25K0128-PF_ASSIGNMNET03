#include <stdio.h>

/* Recursive function to calculate loan repayment */
double calculateRepayment(double loan, double interestRate, int year, int totalYears, double installment, double extraPayment) {
    if (loan <= 0 || year > totalYears) {  // Base case: loan fully repaid or no years left
        return 0;
    }

    // Apply yearly interest
    loan += loan * interestRate / 100;

    // Subtract regular installment and extra payment
    loan -= (installment + extraPayment);

    // If loan goes below zero, adjust the last payment
    if (loan < 0) {
        loan = 0;
    }

    printf("Year %d: Remaining loan = %.2lf\n", year, loan);

    // Recursive case: calculate repayment for next year
    double paidThisYear = installment + extraPayment;
    return paidThisYear + calculateRepayment(loan, interestRate, year + 1, totalYears, installment, extraPayment);
}

int main() {
    printf(" CODE BY HUZAIFA");
    double loan = 100000;       // Initial loan
    double interestRate = 5;    // Annual interest rate in percent
    int years = 3;              // Total number of years
    double installment = 35000; // Fixed yearly installment
    double extraPayment = 0;    // Extra payment (can be adjusted)

    double totalRepayment = calculateRepayment(loan, interestRate, 1, years, installment, extraPayment);

    printf("Total repayment over %d years = %.2lf\n", years, totalRepayment);
printf("COde by HUZAIFA");
    return 0;
}
