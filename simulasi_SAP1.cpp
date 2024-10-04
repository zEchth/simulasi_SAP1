// #include <bits/stdc++.h>
#include <iostream>
#include <bitset>
#include <string>
#include <iomanip>
using namespace std;
/*
1. Cp (Counter Pulse): Mengaktifkan penghitungan program counter (PC) sehingga program counter bertambah satu.
2. Ep (Enable Program Counter):Mengizinkan nilai dari program counter (PC) untuk dikeluarkan dan diteruskan ke bus alamat.
3. Lm (Load Memory): Mengizinkan alamat dari program counter untuk dimasukkan ke memory address register (MAR).
4. CE (Clear Enable): Menghapus atau mengatur ulang (reset) register tertentu.
5. Li (Load Instruction Register): Mengaktifkan instruksi yang sedang dibaca untuk dimuat ke instruction register (IR).
6. Ei (Enable Instruction Register):Mengizinkan keluaran dari instruction register (IR) diteruskan ke decoder atau komponen lain.
7. La (Load Accumulator): Mengaktifkan pemuatan data ke accumulator (A), register utama tempat perhitungan dilakukan.
8. Ea (Enable Accumulator): Mengizinkan isi dari accumulator dikeluarkan ke bus data, sehingga dapat digunakan oleh komponen lain.
9. Su (Subtraction): Mengaktifkan mode pengurangan pada unit aritmatika-logika (ALU).
10.Eu (Enable ALU): Mengizinkan keluaran dari unit aritmatika-logika (ALU) untuk diteruskan ke accumulator (A).
11.Lb (Load B Register): Mengaktifkan pemuatan data ke register B, yang biasanya digunakan sebagai operand kedua dalam operasi aritmatika.
12.Lo (Load Output Register): Mengaktifkan pemuatan data dari accumulator ke output register untuk ditampilkan atau dikirim ke luar.
*/

class Perintah {
    public:
    static const int LDA = 0x00;
    static const int ADD = 0x01;
    static const int SUB = 0x02;
    static const int OUT = 0x0e;
    static const int HLT = 0x0f;
};
class FetchExecute : public Perintah {
    int opcode;
    int operand;
    int Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo;
    int registerB;
    int iR;
    int currentData;
    public:
    FetchExecute() {
        registerB = 0;
        currentData = 0;
        iR = 0;
    }

    int getOpcode(){return opcode;}
    void bitReset();
    void fetch(int &pencacah);
    void execute(int &pencacah);
    void toBiner(int n);
    std::string toBinary4Digits(int n);
    void t1(int &pencacah);
    void t2(int &pencacah);
    void t3();
    void t4();
    void t5LOAD();
    void t6NOP();
    void t5ADDSUB();
    void t6ADD();
    void t6SUB();
};

const int memorySz = 16;
int MAR[memorySz];


// bit-bit kendali
void FetchExecute:: bitReset(){
    Cp = Ep = Ea = Su = Eu = 0; // tidak aktif
    Lm = CE = Li = Ei = La = Lb = Lo = 1; // tidak aktif
}

void FetchExecute:: toBiner(int n){
    int opcodeOperand[32];
    int i = 0;
    int length = 0;

    if(n == 0){
        cout << "0000";
        return;
    }
    while(n > 0){
        opcodeOperand[i] = n % 2;
        n /= 2;
        i++;
        length++;
    }

    for(int i = length - 1; i >= 0; i--){
        cout << opcodeOperand[i];
    }
}
std::string FetchExecute :: toBinary4Digits(int n) {
    std::string binary = std::bitset<4>(n).to_string(); 
    
    return binary;
}

// T1 - T6
void FetchExecute::t1(int &pencacah){
    bitReset();
    Ep = 1;
    Lm = 0;
    cout << "\n\nT1(Keadaan Alamat) : Alamat didalam pencacah program (PC) dipindahkan kepada register alamat memori (MAR)\n";
    cout << "MAR sekarang berisi: " << toBinary4Digits(pencacah); printf(" / %XH\n\n", pencacah);
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "Ep dan Lm merupakan bit-bit kendali yang aktif\n\n";
}  

void FetchExecute::t2(int &pencacah){
    bitReset();
    Cp = 1;
    cout << "\n\nT2(Keadaan Penambahan) : Hitungan pada pencacah program ditingkatkan (ditambah) selama periode ini.\n";
    cout << "Pencacah program sekarang berisi: " << toBinary4Digits(pencacah+1); printf(" / %XH\n\n", pencacah+1);
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "Cp merupakan bit kendali yang aktif\n";
}

void FetchExecute::t3(){
    bitReset();
    CE = Li = 0;
    cout << "\n\nT3(Keadaan Memori) : Instruksi pada RAM dengan alamat yang ditunjuk dipindahkan dari memori ke register instruksi\n";
    iR = getOpcode();
    cout << "Register instruksi sekarang berisi: " << toBinary4Digits(iR) <<"\n\n";
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "CE dan L1 merupakan bit-bit kendali yang aktif\n";
}


/////////////////////////////// siklus pengambilan ////////////////////////////////////
void FetchExecute:: fetch(int &pencacah){
    opcode = (MAR[pencacah] & 0xf0) >> 4;
    operand = MAR[pencacah] & 0x0f;
}



// Rutin LDA
void FetchExecute::t4(){
    cout << endl; printf("%X%X", opcode, operand);cout <<  " / " << toBinary4Digits(opcode) << " "<< toBinary4Digits(operand);
    cout << "\nMedan instruksi " << toBinary4Digits(opcode) <<" dikirim dari Register Instruksi ke Pengendali Pengurut untuk melakukan pengkodean";
    cout << "\nMedan alamat " << toBinary4Digits(operand) <<" diisikan ke MAR\n\n";
    bitReset();
    Ei = 0;Lm = 0; // bit kendali yang aktif
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t4: E1 dan Lm merupakan bit-bit kendali yang aktif\n";
}
void FetchExecute::t5LOAD(){
    bitReset();
    CE = 0;La = 0; // aktif
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t5: CE dan La merupakan bit-bit kendali yang aktif\n\n";

    currentData += MAR[operand];
    cout << "kata data yang telah dialamatkan dalam memori akan diisi kedalam akumulator, sehingga " << currentData << " ditambahkan ke ACCUMULATOR\n\n";
}
void FetchExecute::t6NOP(){
    bitReset();
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "NOP: No operation\n\n";
}

// Rutin ADD and SUB
void FetchExecute::t5ADDSUB(){
    bitReset();
    CE = 0;Lb = 0; // aktif
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t5: CE dan Lb merupakan bit-bit kendali yang aktif\n\n";

    cout << "Ini memungkinkan kata RAM yang telah ditunjuk alamatnya itu untuk mempersiapkan register B\n";
    cout << "Register B = " << registerB << endl;
    cout << "Accumulator = " << currentData << endl << endl;
}
void FetchExecute::t6ADD(){
    bitReset();
    La = 0;Eu = 1; // aktif
    
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t6: Eu dan La merupakan bit-bit kendali yang aktif\n\n";

    cout << "Isi accumulator dikirm ke ADD/SUB, isi registerB dikirim juga ke ADD/SUB, lalu kedua isi dijumlahkan\n";
    cout <<currentData << " + " << registerB << endl;
    currentData += registerB;
    cout << "Hasil penjumlahan dikirim ke accumulator dan isi accumulator sekarang: " << currentData << endl;

    registerB = 0;
}
void FetchExecute::t6SUB(){
    bitReset();
    La = 0;Eu = 1; // aktif
    
    printf("CON : Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("      %d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t6: Eu dan La merupakan bit-bit kendali yang aktif\n\n";

    cout << "Isi accumulator dikirm ke ADD/SUB, isi registerB dikirim juga ke ADD/SUB, lalu kedua isi dikurangkan\n";
    cout <<currentData << " - " << registerB << endl;
    currentData -= registerB;
    cout << "Hasil pengurangan dikirim ke accumulator dan isi accumulator sekarang: " << currentData << endl;

    registerB = 0;
}


// EXECUTE
void FetchExecute::execute(int &pencacah){
    switch(opcode){
        case LDA:
            cout << "================================================================================================================================================\n";
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nT1 Aktif:\n";t1(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nT2 Aktif:\n";t2(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nT3 Aktif:\n";t3();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nT4 Aktif:\n";t4();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------";
            cout << "\nPerintah LDA Aktif\n";
            cout << "----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT5 Aktif:\n";t5LOAD();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nT6 Aktif:\n";t6NOP();
            cout << "\n================================================================================================================================================\n\n\n";

            break;

        case ADD:
            cout << "================================================================================================================================================\n";
            registerB = MAR[operand];
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT1 Aktif:\n";t1(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT2 Aktif:\n";t2(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT3 Aktif:\n";t3();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT4 Aktif:\n";t4();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nPerintah ADD Aktif\n";
            cout << "----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT5 Aktif:\n";t5ADDSUB();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT6 Aktif:\n";t6ADD();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";

            cout << "\n================================================================================================================================================\n\n\n";
            break;

        case SUB:
            cout << "================================================================================================================================================\n";
            registerB = MAR[operand];
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT1 Aktif:\n";t1(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT2 Aktif:\n";t2(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT3 Aktif:\n";t3();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT4 Aktif:\n";t4();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nPerintah SUB Aktif\n";
            cout << "----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT5 Aktif:\n";t5ADDSUB();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT6 Aktif:\n";t6SUB();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";

            cout << "\n================================================================================================================================================\n\n\n";
            break;
        
        case OUT:
            cout << "================================================================================================================================================\n";
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT1 Aktif:\n";t1(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT2 Aktif:\n";t2(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT3 Aktif:\n";t3();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nT4 Aktif:\n";t4();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "Perintah OUT Aktif\n";
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\nIsi dari accumulator dipindahkan ke register keluaran";
            cout << "\nOutput = " << currentData << "\nOutput biner: ";
            toBiner(currentData);cout << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT5 Aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT6 Aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";

            cout << "\n===================================================================================================================================\n\n\n";
            break;

        default:
            cout << "================================================================================================================================================\n";
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT1 Aktif:\n";t1(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT2 Aktif:\n";t2(pencacah);
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT3 Aktif:\n";t3();
            cout << "\nMedan instruksi 1111 akan memberi tahu pengendali-pengurut untuk menghentikan pemrosesan data\n";
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "Perintah HLT Aktif\n";
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT4 Aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT5 Aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\nT6 Aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n\n";
            cout << "\n===============================================================================================================================================\n";
    }
}



int main()
{
    int pencacah = 0;
    FetchExecute SAP;

    // 32 + 33 + 34 - 35;
    cout << "Masukkan isi alamat Pencacah program: \n";
    for(int i = 0; i < memorySz; i++){
        cout << "masukkan alamat ";printf("%XH ", i);cout << ": ";
        scanf("%X", &MAR[i]);
    }

    cout << "\nSimple SAP-1 Simulation" <<endl<<endl;
    while(pencacah < memorySz){
        SAP.fetch(pencacah);
        SAP.execute(pencacah);
        pencacah++;

        if(SAP.getOpcode() == SAP.HLT){
            break;
        }
    }

    // Isi Memory selama program berjalan
    cout << "\n\nSelama program berjalan, isi dari MAR adalah: "<< endl;
    cout << "Memory: ";
    for(int i = 0; i < memorySz; i++){
        printf("%.2X, ", MAR[i]);
    }

    cout << "Anda telah keluar dari program" << endl;  
}