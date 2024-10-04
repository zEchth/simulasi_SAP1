// #include <bits/stdc++.h>
#include <iostream>
#include <bitset>
#include <string>
#include <iomanip>
using namespace std;

const int memorySz = 16;
const int LDA = 0x00;
const int ADD = 0x01;
const int SUB = 0x02;
const int OUT = 0x0e;
const int HLT = 0x0f;

int currentData = 0;

int MAR[memorySz];
int registerB = 0;

// bit-bit kendali
int Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo;
void bitReset(){
    Cp = Ep = Ea = Su = Eu = 0; // tidak aktif
    Lm = CE = Li = Ei = La = Lb = Lo = 1; // tidak aktif
}

void toBiner(int n){
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
std::string toBinary4Digits(int n) {
    std::string binary = std::bitset<4>(n).to_string(); 
    
    return binary;
}


// siklus pengambilan
void fetch(int &opcode, int &operand, int &pencacah){
    opcode = (MAR[pencacah] & 0xf0) >> 4;
    operand = MAR[pencacah] & 0x0f;
    pencacah++;
}


// Rutin LDA
void t4(int &opcode, int &operand){
    cout << opcode; printf("%X", operand);cout <<  " / " << toBinary4Digits(opcode) << " "<< toBinary4Digits(operand);
    cout << "\nMedan instruksi " << toBinary4Digits(opcode) <<" dikirim ke Pengendali Pengurut untuk melakukan pengkodean";
    cout << "\nMedan alamat " << toBinary4Digits(operand) <<" diisikan ke MAR\n\n";
    bitReset();
    Ei = 0;Lm = 0; // bit kendali yang aktif
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t4: E1 dan Lm merupakan bit-bit kendali yang aktif\n";
}
void t5LOAD(int &operand){
    bitReset();
    CE = 0;La = 0; // aktif
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t5: CE dan La merupakan bit-bit kendali yang aktif\n\n";

    currentData += MAR[operand];
    cout << "kata data yang telah dialamatkan dalam memori akan diisi kedalam akumulator, sehingga " << currentData << " ditambahkan ke ACCUMULATOR\n";
}
void t6NOP(){
    bitReset();
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "NOP: No operation\n";
}

// Rutin ADD and SUB
void t5ADDSUB(){
    bitReset();
    CE = 0;Lb = 0; // aktif
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t5: CE dan Lb merupakan bit-bit kendali yang aktif\n\n";

    cout << "Ini memungkinkan kata RAM yang telah ditunjuk alamatnya itu untuk mempersiapkan register B\n";
    cout << "Register B = " << registerB << endl;
    cout << "Accumulator = " << currentData << endl;
}
void t6ADD(){
    bitReset();
    La = 0;Eu = 1; // aktif
    
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t6: Eu dan La merupakan bit-bit kendali yang aktif\n\n";

    cout << "Isi accumulator dikirm ke ADD/SUB, isi registerB dikirim juga ke ADD/SUB, lalu kedua isi dijumlahkan\n";
    cout <<currentData << " + " << registerB << endl;
    currentData += registerB;
    cout << "Hasil penjumlahan dikirim ke accumulator dan isi accumulator sekarang: " << currentData;

    registerB = 0;
}
void t6SUB(){
    bitReset();
    La = 0;Eu = 1; // aktif
    
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t6: Eu dan La merupakan bit-bit kendali yang aktif\n\n";

    cout << "Isi accumulator dikirm ke ADD/SUB, isi registerB dikirim juga ke ADD/SUB, lalu kedua isi dikurangkan\n";
    cout <<currentData << " - " << registerB << endl;
    currentData -= registerB;
    cout << "Hasil pengurangan dikirim ke accumulator dan isi accumulator sekarang: " << currentData;

    registerB = 0;
}

// Rutin OUT
void t4OUT(){
    bitReset();
    Lo = 0;Ea = 1; // aktif
    
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t4: Ea dan Lo merupakan bit-bit kendali yang aktif\n";
    cout << "\nIsi dari accumulator dipindahkan ke register keluaran\n";
}
void t5NOP(){
    bitReset();
    printf("Cp Ep (/L)M (/C/E)  ||  (/L)1 (/E)1 (/L)A EA  ||  Su Eu (/L)B (/L)o\n");
    printf("%d  %d    %d     %d     ||    %d     %d     %d   %d   ||   %d  %d   %d    %d\n", Cp, Ep, Lm, CE, Li, Ei, La, Ea, Su, Eu, Lb, Lo);
    cout << "t5: NOP: No operation\n";
}


// EXECUTE
void execute(int &opcode, int &operand){
    switch(opcode){
        case LDA:
            cout << "====================================================================================================================================\n";
            cout << "\n----------------------------------------------------------------\n";
            cout << "t4 aktif:\n";t4(opcode, operand);
            cout << "\n----------------------------------------------------------------";
            cout << "\nPerintah LDA aktif\n";
            cout << "----------------------------------------------------------------\n\n";
            cout << "t5 aktif:\n";t5LOAD(operand);
            cout << "\n----------------------------------------------------------------\n";
            cout << "t6 aktif:\n";t6NOP();
            cout << "\n====================================================================================================================================\n\n\n";

            break;

        case ADD:
            cout << "====================================================================================================================================\n";
            registerB = MAR[operand];
            cout << "\n----------------------------------------------------------------\n";
            cout << "t4 aktif:\n";t4(opcode, operand);
            cout << "\n----------------------------------------------------------------";
            cout << "\nPerintah ADD aktif\n";
            cout << "----------------------------------------------------------------\n\n";
            cout << "t5 aktif:\n";t5ADDSUB();
            cout << "\n----------------------------------------------------------------\n";
            cout << "t6 aktif:\n";t6ADD();
            cout << "\n----------------------------------------------------------------\n";

            cout << "\n====================================================================================================================================\n\n\n";
            break;

        case SUB:
            cout << "====================================================================================================================================\n";
            registerB = MAR[operand];
            cout << "\n----------------------------------------------------------------\n";
            cout << "t4 aktif:\n";t4(opcode, operand);
            cout << "\n----------------------------------------------------------------";
            cout << "\nPerintah SUB aktif\n";
            cout << "----------------------------------------------------------------\n\n";
            cout << "t5 aktif:\n";t5ADDSUB();
            cout << "\n----------------------------------------------------------------\n";
            cout << "t6 aktif:\n";t6SUB();
            cout << "\n----------------------------------------------------------------\n";

            cout << "\n====================================================================================================================================\n\n\n";
            break;
        
        case OUT:
            cout << "====================================================================================================================================\n";
            cout << "Perintah OUT aktif\n";
            cout << "\n----------------------------------------------------------------\n";
            cout << "t4 aktif:\n";t4OUT();
            cout << "\n----------------------------------------------------------------";
            cout << "\nOutput = " << currentData << "\nOutput biner: ";
            toBiner(currentData);cout << endl;
            cout << "----------------------------------------------------------------\n\n";
            cout << "t5 aktif:\n";t5NOP();
            cout << "\n----------------------------------------------------------------\n";
            cout << "t6 aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------\n";

            cout << "\n===================================================================================================================================\n\n\n";
            break;

        default:
            cout << "====================================================================================================================================\n";
            cout << "Perintah HLT aktif\n";
            cout << "\n----------------------------------------------------------------\n";
            cout << "t4 aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------\n";
            cout << "t5 aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------\n";
            cout << "t6 aktif:\n";t6NOP();
            cout << "\n----------------------------------------------------------------\n";
            
            cout << "\n===================================================================================================================================\n";
    }
}



int main()
{
    int opcode, operand;
    int pencacah = 0;

    // 32 + 33 + 34 - 35;
    cout << "Masukkan alamat memory: \n";
    for(int i = 0; i < memorySz; i++){
        cout << "masukkan alamat ";printf("%XH ", i);cout << ": ";
        scanf("%X", &MAR[i]);
    }

    cout << "\nSimple SAP-1 Simulation" <<endl<<endl;

    while(pencacah < memorySz){
        fetch(opcode, operand, pencacah);
        execute(opcode, operand);

        if(opcode == HLT){
            break;
        }
    }


    // Isi Memory selama program berjalan
    cout << "\n\nSelama program berjalan, isi dari RAM adalah: "<< endl;
    cout << "Memory: ";
    for(int i = 0; i < memorySz; i++){
        printf("%.2X, ", MAR[i]);
    }

    cout << "Anda telah keluar dari program" << endl;  
}