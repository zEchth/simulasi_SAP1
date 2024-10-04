Menggunakan bahasa C++ dengan konsep OOP, maaf kalau ada salah dalam codenya / ketidakefisienan dalam penulisan code

Code yang saya buat adalah Menu Driven dari SAP-1, tujuannya adalah mengsimulasikan cara kerja dari SAP-1:
function yang paling penting disini adalah fetch(pengambilan) dan execute(eksekusi), kemudian variable opcode dan opereand. 
Fetch: T1->Mengirim alamat dari pencacah program ke MAR (alamat dimulai dari 0H/0000). 
       T2->Increament +1 alamat, sehingga alamat menjadi 1H. 
       T3->Mengirim opcode/medan instruksi ke REGISTER INSTRUKSI. 
Execute: T4->Mengirim operand/medan alamat ke RAM, sedangkan register instruksi yang berisi medan instruksi dikirim ke pengendali-pengurut untuk melakukan pengkodean. 
         T5(LDA)->Kata data yang telah dialamatkan dalam RAM akan diisi kedalam ACCUMULATOR, sehingga data dengan alamat tsb akan dikirim ke ACCUMULATOR. 
         T6(LDA)->NOP(No Operation). 
         T5(ADD/SUB)->Data dari alamat yg ditunjuk dalam RAM akan dikirim ke ADD/SUB. 
         T6(ADD/SUB)->Menjumlahkan/mengurangkan kedua angka, yaitu angka yang berada dalam accumulator dan angka yang berada di ADD/SUB, hasilnya akan dikirim kembali ke ACCUMULATOR. 
         T4(OUT)->Mengirim data dari ACCUMULATOR ke REGISTER KELUARAN. 
         T5(OUT)->NOP = T6(OUT)->NOP. 
         T4(HLT) = T5(HLT) = T6(HLT) = NOP. 
Untuk lebih jelasnya, lihat output dari kode yang saya buat. 
