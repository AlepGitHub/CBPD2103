#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    char name[50];
    char id[13]; // 12-digit ID + null terminator
    float monthlyIncome;
    float houseLoanInterest;
    float educationExpense;
    float charityDonation;
    char employmentStatus;
    float annualIncome;
    float taxableIncome;
    float taxLiability;
} Taxpayer;

Taxpayer taxpayers[MAX_SIZE];
int taxpayerCount = 0;
float totalTaxCollected = 0;
int taxBrackets[4] = {0};

void registerTaxpayer();
void editTaxpayer();
void calculateTax(Taxpayer *t);
void displayReport();
int validateID(char id[]);

int main() {
    int choice;
    while (1) {
        printf("=============================\n");
        printf("Sistem Pengurusan Kutipan Cukai\n");
        printf("=============================\n");
        printf("1. Daftar Pembayar Cukai\n");
        printf("2. Edit Rekod Pembayar Cukai\n");
        printf("3. Papar Laporan Cukai\n");
        printf("4. Keluar\n");
        printf("=============================\n");
        printf("Pilih: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            registerTaxpayer();
        } else if (choice == 2) {
            editTaxpayer();
        } else if (choice == 3) {
            displayReport();
        } else if (choice == 4) {
            printf("Keluar dari sistem...\n");
            break;
        } else {
            printf("Pilihan tidak sah! Sila pilih 1-4.\n");
        }
    }
    return 0;
}

void registerTaxpayer() {
    if (taxpayerCount >= MAX_SIZE) {
        printf("Maksimum pembayar cukai telah dicapai!\n");
        return;
    }

    Taxpayer *t = &taxpayers[taxpayerCount];
    
    printf("Masukkan Nama: ");
    fgets(t->name, 50, stdin);
    t->name[strcspn(t->name, "\n")] = 0;

    do {
        printf("Masukkan ID (12 digit): ");
        scanf("%12s", t->id);
        getchar();
    } while (!validateID(t->id));

    do {
        printf("Masukkan Pendapatan Bulanan (RM): ");
        scanf("%f", &t->monthlyIncome);
    } while (t->monthlyIncome <= 0);

    printf("Masukkan Faedah Pinjaman Rumah (%%): ");
    scanf("%f", &t->houseLoanInterest);
    printf("Masukkan Perbelanjaan Pendidikan (RM): ");
    scanf("%f", &t->educationExpense);
    
    do {
        printf("Masukkan Derma (RM): ");
        scanf("%f", &t->charityDonation);
    } while (t->charityDonation < 0 || t->charityDonation > (t->monthlyIncome * 12));

    do {
        printf("Masukkan Status Pekerjaan (P=Pesara, B=Bekerja): ");
        scanf(" %c", &t->employmentStatus);
    } while (t->employmentStatus != 'P' && t->employmentStatus != 'B');

    calculateTax(t);
    taxpayerCount++;
    printf("Pembayar cukai berjaya didaftarkan!\n");
}

void editTaxpayer() {
    char searchID[13];
    printf("Masukkan ID Pembayar Cukai untuk diedit: ");
    scanf("%12s", searchID);
    getchar();

    for (int i = 0; i < taxpayerCount; i++) {
        if (strcmp(taxpayers[i].id, searchID) == 0) {
            printf("Rekod Ditemui. Sila kemas kini maklumat.\n");

            printf("Nama (%s): ", taxpayers[i].name);
            fgets(taxpayers[i].name, 50, stdin);
            taxpayers[i].name[strcspn(taxpayers[i].name, "\n")] = 0;

            do {
                printf("Pendapatan Bulanan (RM) [%.2f]: ", taxpayers[i].monthlyIncome);
                scanf("%f", &taxpayers[i].monthlyIncome);
            } while (taxpayers[i].monthlyIncome <= 0);

            printf("Faedah Pinjaman Rumah (%%) [%.2f]: ", taxpayers[i].houseLoanInterest);
            scanf("%f", &taxpayers[i].houseLoanInterest);

            printf("Perbelanjaan Pendidikan (RM) [%.2f]: ", taxpayers[i].educationExpense);
            scanf("%f", &taxpayers[i].educationExpense);

            do {
                printf("Derma (RM) [%.2f]: ", taxpayers[i].charityDonation);
                scanf("%f", &taxpayers[i].charityDonation);
            } while (taxpayers[i].charityDonation < 0 || taxpayers[i].charityDonation > (taxpayers[i].monthlyIncome * 12));

            do {
                printf("Status Pekerjaan (P=Pesara, B=Bekerja) [%c]: ", taxpayers[i].employmentStatus);
                scanf(" %c", &taxpayers[i].employmentStatus);
            } while (taxpayers[i].employmentStatus != 'P' && taxpayers[i].employmentStatus != 'B');

            // Recalculate tax based on updated info
            calculateTax(&taxpayers[i]);

            printf("Rekod berjaya dikemaskini!\n");
            return;
        }
    }
    printf("Rekod tidak ditemui!\n");
}


void calculateTax(Taxpayer *t) {
    t->annualIncome = t->monthlyIncome * 12;
    
    float houseDeduction = (t->houseLoanInterest / 100 * t->annualIncome);
    float totalDeductions = houseDeduction + t->educationExpense + t->charityDonation;
    
    if (totalDeductions > t->annualIncome) totalDeductions = t->annualIncome;
    t->taxableIncome = t->annualIncome - totalDeductions;

    printf("\n===== Debugging Tax Calculation =====\n");
    printf("Annual Income: RM%.2f\n", t->annualIncome);
    printf("House Loan Interest Deduction: RM%.2f\n", houseDeduction);
    printf("Education Expense Deduction: RM%.2f\n", t->educationExpense);
    printf("Charity Donation Deduction: RM%.2f\n", t->charityDonation);
    printf("Total Deductions: RM%.2f\n", totalDeductions);
    printf("Taxable Income: RM%.2f\n", t->taxableIncome);
    
    if (t->taxableIncome <= 20000) {
        t->taxLiability = 0;
        taxBrackets[0]++;
    } else if (t->taxableIncome <= 50000) {
        t->taxLiability = 0.10 * (t->taxableIncome - 20000);
        taxBrackets[1]++;
    } else if (t->taxableIncome <= 100000) {
        t->taxLiability = 3000 + 0.20 * (t->taxableIncome - 50000);
        taxBrackets[2]++;
    } else {
        t->taxLiability = 13000 + 0.30 * (t->taxableIncome - 100000);
        taxBrackets[3]++;
    }

    printf("Final Tax Liability: RM%.2f\n", t->taxLiability);
    totalTaxCollected += t->taxLiability;
}

void displayReport() {
    printf("\n=============================\n");
    printf("Laporan Cukai Pembayar\n");
    printf("=============================\n");
    for (int i = 0; i < taxpayerCount; i++) {
        printf("Nama: %s\n", taxpayers[i].name);
        printf("ID: %s\n", taxpayers[i].id);
        printf("Pendapatan Bercukai: RM%.2f\n", taxpayers[i].taxableIncome);
        printf("Cukai Perlu Dibayar: RM%.2f\n", taxpayers[i].taxLiability);
        printf("-----------------------------\n");
    }
    printf("Jumlah Cukai Keseluruhan: RM%.2f\n", totalTaxCollected);
    printf("Bilangan pembayar cukai mengikut kategori:\n");
    printf("Pembayar Cukai : <= RM20,000 = [%d]\n", taxBrackets[0]);
    printf("Pembayar Cukai : RM20,001 - RM50,000 =[%d]\n", taxBrackets[1]);
    printf("Pembayar Cukai : RM50,001 - RM100,000 = [%d]\n", taxBrackets[2]);
    printf("Pembayar Cukai : > RM100,000 = [%d]\n", taxBrackets[3]);
}

int validateID(char id[]) {
    if (strlen(id) != 12) {
        printf("ID mesti 12 digit!\n");
        return 0;
    }
    for (int i = 0; i < 12; i++) {
        if (id[i] < '0' || id[i] > '9') {
            printf("ID hanya boleh mengandungi nombor!\n");
            return 0;
        }
    }
    return 1;
}
