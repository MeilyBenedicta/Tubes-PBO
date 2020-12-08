#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <map>
#include <iterator>

#ifdef __linux__
#define OS_TYPE 0
#elif defined(_WIN32) || defined(WIN32)
#define OS_TYPE 1
#endif

using namespace std;

void write(int);
void write(int, string);

class Kriptografi
{
public:
    string enkripsi(string str)
    {
        for (int i = 0; (i < 100 && str[i] != '\0'); i++)
            str[i] = str[i] + 2;

        return str;
    }

    string dekripsi(string str)
    {
        for (int i = 0; (i < 100 && str[i] != '\0'); i++)
            str[i] = str[i] - 2;

        return str;
    }
    
    //virtual void AdminLogin()= 0;
};

class Inisiasi
{
public:
    vector<int> row;
    vector<string> nama;
    vector<int> jumlah;
    vector<int> harga;
    Inisiasi()
    {
        this->cls();
        this->reinit();
    };
    void reinit()
    {
        row = {};
        nama = {};
        jumlah = {};
        harga = {};
        ifstream data("listBarang.csv");
        string line;
        vector<vector<string>> parsedCsv;
        int i = 0;
        while (getline(data, line))
        {
            stringstream lineStream(line);
            string cell;
            vector<string> parsedRow;
            while (getline(lineStream, cell, ','))
            {
                parsedRow.push_back(cell);
            }
            parsedCsv.push_back(parsedRow);
            row.push_back(stoi(parsedRow[0]));
            nama.push_back(parsedRow[1]);
            jumlah.push_back(stoi(parsedRow[2]));
            harga.push_back(stoi(parsedRow[3]));
        };
    };
    void cls()
    {
        if (OS_TYPE)
            system("CLS");
        else
            cout << "\033[2J\033[1;1H";
        return;
    };
    void write(int Kuantitas, string Nama)
    {
        int Harga;
        ofstream fout;
        fout.open("listBarang.csv", ios::trunc);

        for (int i = 0; i < row.size(); i)
        {
            Nama = nama[i];
            Kuantitas = jumlah[i];
            Harga = harga[i];
            fout << ++i << ","
                 << Nama << ","
                 << Kuantitas << ","
                 << Harga
                 << "\n";
        }
        fout.close();
        this->reinit();
    };

    void write(int kuantitas)
    {
        int Harga, duplicate = 0;
        this->cls();

        cout << "Masukkan detail stock barang yang baru dengan format: "
             << " Nama barang, kuantitas, harga"
             << endl;
        string name = "", nameCheck = "", nameCheck1 = "";

        cout << "Nama barang: ";
        cin.ignore();
        getline(cin, name);
        replace(name.begin(), name.end(), ' ', '_');
        for (int i = 0; i < row.size(); i++)
        {
            nameCheck = nama[i];
            nameCheck1 = name;
            for_each(nameCheck.begin(), nameCheck.end(), [](char &c) {
                c = ::tolower(c);
            });
            for_each(nameCheck1.begin(), nameCheck1.end(), [](char &c) {
                c = ::tolower(c);
            });
            if (nameCheck == nameCheck1)
            {
                duplicate = 1;
            }
        };

        if (duplicate)
        {
            cout << "Barang dengan nama yang sama sudah terdaftar. Silahkan coba lagi.\n";
            return this->write(kuantitas);
        };
        cout << "Kuantitas: ";
        cin >> kuantitas;
        cout << "Harga: Rp";
        cin >> Harga;

        nama.push_back(name);
        jumlah.push_back(kuantitas);
        harga.push_back(Harga);
        row.push_back(1);
        write(kuantitas, name);

        cout << "Produk berhasil diregistrasi!\n";
        this->reinit();
    }
};

class Admin : public Inisiasi
{

public:
    Admin()
    {
        this->cls();
        this->section();
    }

    void section()
    {
        int choice;
        cout << "1 - Masukkan produk | 2 - Tampilkan semua produk | 3 - Hapus produk | 4 - Edit produk | 5/etc - Keluar" << endl;
        cin >> choice;
        if (choice == 1)
            this->write(choice);
        else if (choice == 2)
        {
            this->read();
            return this->section();
        }
        else if (choice == 3)
            this->remove();
        else if (choice == 4)
            this->edit();
        else if (choice == 5)
            cout << "Anda berhasil Keluar dari program" << endl;
        else
        {
            cout << "Input yang anda masukkan tidak valid!!" << endl;
            return this->section();
        }
    }

    void read()
    {
        this->cls();
        int f = row.size();
        cout << "Jumlah entity: " << row.size() << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "ID Barang \t| Nama barang \t\t| Jumlah barang \t| Harga barang" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        for (int i = 0, id = 1; i < f; i++, id++)
        {
            string name = nama[i];
            replace(name.begin(), name.end(), '_', ' ');
            cout << "\t" << id << "\t| " << nama[i] << " \t\t| " << jumlah[i] << " \t\t\t| " << harga[i] << endl;
        }
    };

    void remove()
    {
        this->read();
        int choice, back = row.size() + 1, choice1;
        cout << "Pilih ID barang untuk dihapus dari daftar.\n";
        cout << "1-" << row.size() << " - Pilih barang | " << back << " - Kembali\n";
        cin >> choice;
        if (choice == back)
        {
            return this->section();
        }
        else if (choice < 1 || choice > row.size())
        {
            cout << "Input tidak valid. Silahkan coba lagi.\n";
            return this->remove();
        }
        else
        {
            cout << "Kamu yakin ingin menghapus " << nama[choice - 1] << " dari daftar barang?\n"
                 << "1 - Ya | 2/etc - Tidak/Kembali\n";
            cin >> choice1;
            if (choice1 != 1 && choice1 != 2)
            {
                cout << "Input tidak valid. Silahkan coba lagi.\n";
                return this->remove();
            }
            else if (choice1 == 2)
                return this->remove();
            else
            {
                nama.erase(nama.begin() + choice - 1);
                jumlah.erase(jumlah.begin() + choice - 1);
                harga.erase(harga.begin() + choice - 1);
                row.erase(row.begin());
                this->write(choice, nama[choice]);
                this->cls();
                cout << "Barang berhasil dihapus dari daftar!\n";
                this->reinit();
                return this->section();
            }
        };
    };

    void edit()
    {
        this->read();
        int choice, back = row.size() + 1, change = 0, choice2;
        cout << "Pilih ID barang untuk di edit harga / kuantitasnya.\n";
        cout << "1-" << row.size() << " - Pilih barang | " << back << " - Kembali\n";
        cin >> choice;
        if (choice == back)
        {
            return this->section();
        }
        else if (choice < 1 || choice > row.size())
        {
            cout << "Input tidak valid. Silahkan coba lagi.\n";
            return this->edit();
        }
        else
        {
            cout << "Apa yang akan di edit?\n";
            cout << "1 - Harga | 2 - Kuantitas | 3/etc - Kembali\n";
            cin >> choice2;
            if (choice2 == 1)
            {
                cout << "Harga " << nama[choice - 1] << " adalah Rp" << harga[choice - 1] << ". Ubah ke berapa?\nRp";
                cin >> change;
                if (change < 0)
                {
                    cout << "Input tidak valid. Silahkan coba lagi.\n";
                    return this->edit();
                }
                cout << "Akan mengubah harga " << nama[choice - 1] << " ke Rp" << change << ". Ubah?\n";
                cout << "1 - Ya | 2/etc - Kembali\n";
                cin >> choice2;
                if (choice2 != 1)
                    return this->edit();
                harga[choice - 1] = change;
                this->write(choice, nama[choice]);
                this->cls();
                cout << "Harga berhasil diubah!\n";
                return this->section();
            }
            else if (choice2 == 2)
            {
                cout << "Jumlah " << nama[choice - 1] << " adalah " << jumlah[choice - 1] << ". Ubah ke berapa?\n";
                cin >> change;
                if (change < 0)
                {
                    cout << "Input tidak valid. Silahkan coba lagi.\n";
                    return this->edit();
                }
                cout << "Akan mengubah jumlah " << nama[choice - 1] << " ke " << change << ". Ubah?\n";
                cout << "1 - Ya | 2/etc - Kembali\n";
                cin >> choice2;
                if (choice2 != 1)
                    return this->edit();
                jumlah[choice - 1] = change;
                this->write(choice, nama[choice]);
                this->cls();
                this->section();
                cout << "Jumlah berhasil diubah!\n";
            }
            else
                return this->section();
        }
    };
};

class User : public Inisiasi
{
public:
    vector<vector<int>> total;
    bool kosong = false;
    string nama_pelanggan;
    int msg = 0;

public:
    User(string name = "pelanggan")
    {
        for (int i = 0; i < row.size(); i++)
        {
            vector<int> temp = {i, 0};
            total.push_back(temp);
        }
        this->nama_pelanggan = name;
        this->section();
    };

    void status()
    {
        cls();
        string pesan = "";
        switch (msg)
        {
        case 0:
            break;
        case 1:
            pesan = "Keranjang kamu masih kosong nih! Kamu harus tambah sesuatu dulu untuk dapat mengurangi belanjaan!";
            break;
        case 2:
            pesan = "Barang berhasil ditambahkan kedalam keranjang!";
            break;
        case 3:
            pesan = "Barang berhasil dikurangi dari keranjang!";
            break;
        case 4:
            pesan = "Kamu yakin ingin keluar?\nBelanjaan kamu tidak akan tersimpan loh!";
            break;
        case 5:
            pesan = "Input tidak valid! Silahkan coba lagi!";
            break;
        case 6:
            pesan = "Stok barang tidak sebanyak itu. Silahkan coba lagi!";
            break;
        case 7:
            pesan = "Tidak ada item tersebut didalam keranjangmu! Silahkan coba lagi!";
            break;
        case 8:
            pesan = "Yang ada didalam keranjangmu tidak sebanyak itu. Silahkan coba lagi!";
            break;
        case 9:
            pesan = "Maaf, uang anda kurang! Silahkan input kembali!";
            break;
        }
        msg = 0;
        cout << "Halo, " << nama_pelanggan << "! Selamat Berbelanja\n"
             << pesan << endl;
        ;
    };

    void show()
    {
        this->status();
        int f = row.size();
        cout << "=================================================================================\n";
        cout << "List barang: " << row.size() << " item\n";
        cout << "---------------------------------------------------------------------------------" << endl;
        cout << "\tNo\t| Nama barang\t| Sisa barang\t| Harga barang" << endl;
        cout << "---------------------------------------------------------------------------------" << endl;
        for (int i = 0, d = 1; i < f; i++, d++)
        {
            cout << "\t" << d << " \t| " << nama[i] << " \t| " << jumlah[i] - total[i][1] << " \t\t| " << harga[i] << endl;
        }
        cout << "=================================================================================\n";
    };

    void keranjang()
    {
        cout << "=================================================================================\n";
        cout << "Keranjang\n";
        cout << "ID Barang\t| Nama barang\t\t| Jumlah barang\t\t| Total harga" << endl;

        int l = 0;
        for (int i = 0, id = 1; i < row.size(); i++, id++)
        {
            if (total[i][1] != 0)
            {
                l++;
                cout << id << " \t\t| " << nama[i] << " \t\t| " << total[i][1] << " \t\t\t| Rp. " << total[i][1] * harga[i] << endl;
                this->kosong = false;
            }
        }
        if (l == 0)
        {
            cout << "Wah, keranjang kamu masih kosong nih! ayo kita tambah beberapa.\n";
            this->kosong = true;
        }
        cout << "=================================================================================\n";
    };

    void section()
    {
        this->reinit();
        this->status();
        this->keranjang();
        cout << "1 - Tambah belanjaan | 2 - Kurangi belanjaan | 3 - Checkout | 4/etc - Keluar\n";
        int choice;
        cin >> choice;
        if (choice == 1)
            return this->tambah();

        else if (choice == 2)
            return this->kurang();

        else if (choice == 3)
            return this->checkout();
        else
            return this->konfirmasi_keluar();
    };

    void tambah()
    {
        this->show();
        cout << "Barang mana yang ingin kamu beli?\n";
        cout << "1-" << row.size() << " - Pilih barang | " << row.size() + 1 << " - Kembali\n";
        int choice, kuantitas, kembali = row.size() + 1;
        cin >> choice;
        if (choice == kembali)
            return this->section();
        if (choice > kembali || choice < 1)
        {
            msg = 5;
            return this->tambah();
        }
        else
        {
            int sisa = jumlah[choice - 1] - total[choice - 1][1];
            int isi = total[choice - 1][1];
            cout << "Berapa banyak yang ingin kamu ambil?\n";
            cout << "Stok barang = " << sisa << "\n";
            if (total[choice - 1][1] > 0)
                cout << "Yang ada di keranjang = " << isi << "\n";
            cin >> kuantitas;
            if (kuantitas < 0)
            {
                msg = 5;
                return this->tambah();
            }
            else if (kuantitas > sisa)
            {
                msg = 6;
                return this->tambah();
            }
            else
            {
                total[choice - 1][1] = kuantitas;
                msg = 2;
                return this->section();
            }
        }
    };

    void kurang()
    {
        if (this->kosong == true)
        {
            msg = 1;
            return this->section();
        }
        this->cls();
        this->keranjang();
        cout << "Barang mana yang ingin kamu kurangi dari keranjang?\nPilih dengan memasukkan nomor ID barang tersebut.\n";
        cout << "1-" << row.size() << " - Pilih barang | " << row.size() + 1 << " - Kembali\n";
        int choice, kuantitas, isi, kembali = row.size() + 1;
        cin >> choice;
        if (choice == kembali)
            return this->section();
        else if (choice > kembali || choice < 1)
        {
            msg = 5;
            return this->kurang();
        }
        else if (total[choice - 1][1] < 1)
        {
            msg = 7;
            return this->kurang();
        }
        else
        {
            int isi = total[choice - 1][1];
            cout << "Berapa banyak yang ingin kamu kembalikan?\n";
            cout << "Yang ada di keranjang = " << isi << "\n";
            cin >> kuantitas;
            if (kuantitas < 0)
            {
                msg = 5;
                return this->kurang();
            }
            else if (kuantitas > isi)
            {
                msg = 8;
                return this->kurang();
            }
            else
            {
                total[choice - 1][1] = total[choice - 1][1] - kuantitas;
                msg = 3;
                return this->section();
            }
        }
    };

    void checkout()
    {
        if (this->kosong == true)
        {
            msg = 1;
            return this->section();
        }
        this->status();
        int totalHarga = 0, uang = 0;
        for (int i = 0; i < total.size(); i++)
        {
            totalHarga = (total[i][1] * harga[i]) + totalHarga;
        }

        cout << "Total harga: Rp. " << totalHarga << "\n";
        cout << "Uang pembayaran: Rp. ";
        cin >> uang;
        if (totalHarga > uang)
        {
            msg = 9;
            return this->checkout();
        }
        else if (uang < 0)
        {
            msg = 5;
            return this->checkout();
        }
        else
        {
            int choice;
            cout << "Kamu akan membayar dengan uang Rp. " << uang << "\n";
            cout << "1 - Bayar | 2 - Ganti uang | 3/etc - Kembali\n";
            cin >> choice;
            if (choice == 1)
            {
                cout << "Kembalian kamu Rp. " << uang - totalHarga << "\n";
                cout << "Terimakasih telah berbelanja dengan kami! Selamat datang kembali dilain waktu! :)\n";
                for (int i = 1; i < row.size() + 1; i++)
                {
                    jumlah[i - 1] = jumlah[i - 1] - total[i - 1][1];
                }
                this->write(choice, nama[choice]);
                this->reinit();

                return;
            }
            else if (choice == 2)
            {
                return this->checkout();
            }
            else
            {
                return this->section();
            }
        }
    };

    void konfirmasi_keluar()
    {
        cout << "Kamu yakin ingin keluar?\n";
        if (kosong == false)
        {
            msg = 4;
            this->keranjang();
        }
        cout << "1 - Keluar | 2/etc - Kembali\n";
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            cout << "Terimakasih atas kunjungannya! Selamat datang kembali dilain waktu! :)\n";
            return;
        }
        else
        {
            return this->section();
        }
    };
};

class Level : public Inisiasi, Kriptografi
{
public:
    string name;
    int option;
    string userName;
    string userPassword;

    map<string, string> akun;
    map<string, string>::iterator itr;
    Level()
    {
        this->level();
    }

    void level()
    {
        this->cls();
        cout << "                       /       \\                                    " << endl;
        cout << "                     (| (.) (.) |)                                   " << endl;
        cout << ".===================.000o==()==o000.===================.             " << endl;
        cout << "|   _          _                                       |             " << endl;
        cout << "|  \\ \\        / / __   __    __  ___  _______   ___    |           " << endl;
        cout << "|   \\ \\  /\\  / / / _ \\|  | / __|/ _ \\|  _ _  | / _ \\   |       " << endl;
        cout << "|    \\ \\/  \\/ / \\  __/|  || |__ | _ || | | | |\\  __/   |        " << endl;
        cout << "|     \\__/\\__/   \\___||__| \\___|\\___/|_| | |_| \\___|   |       " << endl;
        cout << "|                                                      |             " << endl;
        cout << "|          ^^^^^^^^SELAMAT BERBELANJA^^^^^^^^          |             " << endl;
        cout << ".===================.ooo0==============================.             " << endl;
        cout << "                    (   )     0ooo.                                  " << endl;
        cout << "                     \\ (      (   )                                 " << endl;
        cout << "                      \\_)      ) /                                  " << endl;
        cout << "                              (_/                                    " << endl;
        cout << "1. User" << endl;
        cout << "2. Admin" << endl;
        cout << "Masukkan pilihan anda: ";
        cin >> this->option;
        try
        {
            cin.clear();
            if (this->option != 1 && this->option != 2)
                throw "Pilihan tidak sesuai";
            switch (this->option)
            {
            case 1:
                this->option == 1;
                this->user();
                break;

            case 2:
                this->option == 2;
                this-> AdminLogin();
                break;
            default:
                return this->level();
            }
        }
        catch (const char *message)
        {
            cout << message << endl;
        }
    }

    void user()
    {
        this->cls();
        cout << "Masukkan nama: ";
        cin >> name;
        User s(name);
    }

    void AdminLogin()
    {
        this->cls();
        cout << "Login Admin\n";
        cout << "Username:\t ";
        cin >> this->userName;
        cout << "Password:\t ";
        userPassword = "";
        char c;
        while ((c = getch()) != 13)
        {
            if (c == 8 && userPassword.length() > 0)
            {
                cout << '\b' << ' ' << '\b';
                userPassword.erase(userPassword.length() - 1, 1);
            }

            else if (c != 8 && userPassword.length() <= 24 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
            {
                userPassword += c;
                cout << '*';
            }
        }
        userPassword = enkripsi(userPassword);
        
        this->cekData();
    }

    void cekData()
    {
        ifstream data("admin.csv");
        string line;
        vector<vector<string>> parsedCsv;
        int i = 0;
        while (getline(data, line))
        {
            stringstream lineStream(line);
            string cell;
            vector<string> parsedRow;
            while (getline(lineStream, cell, ','))
            {
                parsedRow.push_back(cell);
            }
            /* Login admin menggunakan:
            1.  username: adib
                Password: passwordyangkuat
            2   username: meily
                Password: mei
            3   username: fenni
                Password: krist
            */
            
            akun.insert(pair<string, string>(parsedRow[1], parsedRow[2]));
            this->auth(this->userName, this->userPassword);
        }
    }
    void auth(string userName, string userPassword)
    {
        
        string pw;
        if (akun.find(userName)->first == userName)
        {
            pw = (akun.find(userName)->second);
            if (userPassword.compare(pw) == 0)
            {
                cout <<endl<< "Login berhasil!\n"<<endl;
                system("pause");
                Admin adm;
                return;
            }
            else if (userPassword.compare(pw) != 0)
            {
                cout <<endl<< "Password salah!\n";
            }
            }
        
    }
    void write()
    {
        ofstream fout;
        fout.open("admin.csv", ios::trunc);
        string userName;
        string userPassword;
        for (itr = akun.begin(); itr != akun.end(); ++itr)
        {
            userName = itr->first;
            enkripsi(userPassword) = itr->second;
            fout << userName << "," << userPassword << "\n";
        };
        fout.close();
    };
};

int main()
{
    Level lvl;
    return 0;
};