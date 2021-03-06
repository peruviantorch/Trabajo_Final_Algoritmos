﻿#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <stdlib.h>
#include <chrono>
#include <iomanip>
#include <vector>
#include "Archivo.hpp"
#include "avl.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;
std::experimental::filesystem::recursive_directory_iterator;
using namespace std::chrono_literals;

int ConvertirMes(string fecha) {
	int mesNum = 0;
	if (fecha.substr(4, 3) == "Jan") mesNum = 01;
	if (fecha.substr(4, 3) == "Feb") mesNum = 02;
	if (fecha.substr(4, 3) == "Mar") mesNum = 03;
	if (fecha.substr(4, 3) == "Apr") mesNum = 04;
	if (fecha.substr(4, 3) == "May") mesNum = 05;
	if (fecha.substr(4, 3) == "Jun") mesNum = 06;
	if (fecha.substr(4, 3) == "Jul") mesNum = 07;
	if (fecha.substr(4, 3) == "Aug") mesNum = 8;
	if (fecha.substr(4, 3) == "Sep") mesNum = 9;
	if (fecha.substr(4, 3) == "Oct") mesNum = 10;
	if (fecha.substr(4, 3) == "Nov") mesNum = 11;
	if (fecha.substr(4, 3) == "Dec") mesNum = 12;

	else return mesNum;
}



string Imprimirfecha(string fecha) {

	int mess = ConvertirMes(fecha);

	if (mess == 8 || mess == 9) {
		string Fecha = fecha.substr(20, 4) + '0' + to_string(mess) + fecha.substr(8, 2);
		return Fecha;
	}

	else {
		string Fecha = fecha.substr(20, 4) + to_string(mess) + fecha.substr(8, 2);
		return Fecha;
	}
	return 0;
}

string ImprimirExtension(string nombre) {

	/*string ext = nombre.substr(nombre.length() - 4, 4);
	return ext;*/

	for (int i = 0; i < nombre.length(); i++) {
		if (nombre[i] == '.') return nombre.substr(i + 1, nombre.length() - i);
	}

	return "Folder";
}

string recortarnombre(string nombre) {
	for (int i = 0; i < nombre.length(); i++) {
		if (nombre[i] == '.') return nombre.substr(0, nombre.length() - (nombre.length() - i));
	}

	return nombre;
}

void insertionSortTam(vector<Archivo*> V, int n) {
	for (int i = 1; i < n; ++i) {
		int j = i;
		Archivo * e = V[i];
		while (j > 0 && V[j - 1]->gettamano() > e->gettamano()) {
			V[j--] = V[j - 1];
		}
		if (j != i) V[j] = e;
	}
}


void shellSort(vector<Archivo*> V, int n) {
	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; ++i) {
			int j = i;
			Archivo * e = V[i];
			for (; j >= gap && V[j - 1]->gettamano() > e->gettamano(); j -= gap) {
				V[j] = V[j - 1];
			}
			if (j != i) V[j] = e;
		}
	}
}


void insertionSortNom(vector<Archivo*> V, int n) {
	for (int i = 1; i < n; ++i) {
		int j = i;
		Archivo * e = V[i];
		while (j > 0 && V[j - 1]->getnombre() > e->getnombre()) {
			V[j--] = V[j - 1];
		}
		if (j != i) V[j] = e;
	}
}

int main()
{

	vector <string> nombre;
	vector <string> extension;
	vector <int> tamano;
	vector <string> fecha;

	string path = "D:\\algo";
	for (auto entry : fs::recursive_directory_iterator(path)) {
		//NOMBRE
		cout << entry.path() << endl;

		string name = recortarnombre(entry.path().filename().string());
	
		//cout << name << endl;
		nombre.push_back(name);


		//TAMA�O DE ARCHIVO. SI ES UN FOLDER ES 0 bytes
		int size;

		try {
			std::cout << "File size = " << fs::file_size(entry.path()) << " bytes" << '\n';
			size = fs::file_size(entry.path());
		}
		catch (fs::filesystem_error& e) {
			std::cout << "folder = 0 bytes" << '\n';
		}
		
		tamano.push_back(size);

		//FECHA DE ULTIMA MODIFICACION
		auto ftime = fs::last_write_time(entry.path());
		time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
		string date = Imprimirfecha(asctime(localtime(&cftime)));
		fecha.push_back(date);
		cout << date << endl;

		//EXTENSION

		string ext;
		ext = entry.path().filename().string();
		extension.push_back(ImprimirExtension(ext));
		//cout << ext << endl << endl;
	}

	vector <Archivo*> files;

	for (int i; i < nombre.size(); i++) {
		files.push_back(new Archivo(nombre[i], extension[i], tamano[i], stoi(fecha[i])));
	}

	///////////////////////////////////FIN DE ESCANEO 
	////INICIO DE ARBOLES


	typedef AVLTree <Archivo*, string, nullptr> AVLNom;
	typedef AVLTree <Archivo*, string, nullptr> AVLExt;
	typedef AVLTree <Archivo*, int, nullptr> AVLTam;
	typedef AVLTree <Archivo*, int, nullptr> AVLFec;

	auto knom = [](Archivo * A) { return A->getnombre(); };
	auto kext = [](Archivo * A) { return A->getextension(); };
	auto ktam = [](Archivo * A) { return A->gettamano(); };
	auto kfec = [](Archivo * A) { return A->getfecha(); };

	AVLNom * avlnom = new AVLNom(knom);
	AVLExt * avlext = new AVLExt(kext);
	AVLTam * avltam = new AVLTam(ktam);
	AVLFec * avlfec = new AVLFec(kfec);


	auto Mostrar = [](Archivo * A)
	{
		cout << " ( " << A->getnombre() << " , " << A->getextension() << " , " << A->gettamano() << " , " << A->getfecha() << " )" << endl;
	};

	auto add = [&](Archivo * A)
	{
		avlnom->add(A);
		avlext->add(A);
		avltam->add(A);
		avlfec->add(A);
	};


	for (int i = 0; i < files.size(); i++) {
		add(files[i]);
	}

	///////////////////////////////FIN DE ARBOLES

	cout << endl;
	cout << endl;
	cout << endl;

	cout << "Serializacion e indexado completos....." << endl;
	cout << endl;
	cout << endl;
	cout << endl;



	int x, x2, dato, ord, fl = 1;
	string dato2;
	Archivo * A;
	vector <Archivo *> V;

	do {
		system("Color 1A");
		cout << "##########   ######   ##            ############   ###########   ##        ##     ###########" << endl; system("Color 3D");
		cout << "##             ##     ##            ##             ##              #      #       ##       ##" << endl; system("Color 1B");
		cout << "##             ##     ##            ##             ##               #    #        ##       ##" << endl; system("Color 5A");
		cout << "#######        ##     ##            ##             ##                #  #         ##       ##" << endl; system("Color 8C");
		cout << "##             ##     ##            ##########     #########           #          ###########" << endl; system("Color 4B");
		cout << "##             ##     ##            ##             ##                #   #        ##         " << endl; system("Color 9A");
		cout << "##             ##     ##            ##             ##               #     #       ##         " << endl; system("Color 0A");
		cout << "##           ######   ###########   ############   ###########    ##       ##     ##         " << endl;

		cout << "Que desea hacer???" << endl;
		cout << endl;
		cout << "1)Buscar un archivo :" << endl;
		cout << "2)Filtrar por tamano:" << endl;
		cout << "3)Filtrar por nombre:" << endl;
		cout << "4)Ordenar archivos  :" << endl;
		cout << endl;
		cin >> x;

		cout << endl;	cout << endl;

		switch (x) {
		case 1:
			cout << "Por que criterio desea buscar???" << endl;
			cout << endl;
			cout << "1)Nombre   :" << endl;
			cout << "2)Extension:" << endl;
			cout << "3)Tamano   :" << endl;
			cout << "4)Fecha    :" << endl;
			cout << endl;
			cin >> x2;
			cout << endl; cout << endl;
			cout << "Agregue el criterio" << endl;
			switch (x2) {
			case 1:
				cin >> dato2;
				A = avlnom->find(dato2);
				break;
			case 2:
				cin >> dato2;
				A = avlext->find(dato2);
				break;
			case 3:
				cin >> dato;
				A = avltam->find(dato);
				break;
			case 4:
				cin >> dato;
				A = avlfec->find(dato);
				break;
			}
			cout << endl; cout << endl;
			if (A != nullptr) {
				Mostrar(A);
			}
			else {
				cout << "No encontrado" << endl;
			}

			break;
		case 2:
			cout << "Por que criterio desea filtrar???" << endl;
			cout << endl;
			cout << "1)Mayor a: " << endl;
			cout << "2)Menor a: " << endl;
			cout << "3)Igual a: " << endl;
			cout << endl;
			cin >> x2;
			cout << endl; cout << endl;
			cout << "Agregue el criterio: " << endl;
			switch (x2) {
			case 1:
				cin >> dato;

				V = avltam->FiltradoMayor(dato);
				break;
			case 2:
				cin >> dato;

				V = avltam->FiltradoMenor(dato);
				break;
			case 3:
				cin >> dato;

				V = avltam->FiltradoEqual(dato);
				break;
			default:
				break;

			}
			cout << endl; cout << endl;

			for (int i = 0; i < V.size(); i++) {
				Mostrar(V[i]);
			}

			cout << endl;
			cout << endl;
			cout << "Desea ordenar el filtrado???" << endl;
			cout << endl;
			cout << "1)Ascendentemente" << endl;
			cout << "2)Descendentemente" << endl;
			cout << "3)No" << endl;
			cin >> ord;
			cout << endl;
			cout << endl;
			switch (ord) {
			case 1:

				insertionSortTam(V, V.size());
				for (int i = 0; i < V.size(); i++) {
					Mostrar(V[i]);
				}

				break;
			case 2:
				insertionSortTam(V, V.size());

				for (int i = 0; i < V.size() / 2; i++) {
					Archivo* aux = V[i];
					V[i] = V[V.size() - 1 - i];
					V[V.size() - 1 - i] = aux;
				}


				for (int i = 0; i < V.size(); i++) {
					Mostrar(V[i]);
				}

				break;
			case 3:
				break;
			default:
				break;
			}
			break;
		case 3:
			cout << "Por que criterio desea filtrar???" << endl;
			cout << endl;
			cout << "1)Comienza con: " << endl;
			cout << "2)Termina con : " << endl;
			cout << "3)Contiene    : " << endl;
			cout << endl;
			cin >> x2;
			cout << endl; cout << endl;
			cout << "Agregue el criterio: " << endl;
			switch (x2) {
			case 1:
				cin >> dato2;
				V = avlnom->antiinorderV();
				V = avlnom->FiltradoEmpiezaCon(dato2, V);
				break;
			case 2:
				cin >> dato2;
				V = avlnom->inorderV();
				V = avlnom->FiltradoTerminaCon(dato2, V);
				break;
			case 3:
				cin >> dato2;
				V = avlnom->inorderV();
				V = avlnom->FiltradoContiene(dato2, V);
				break;
			default:
				break;
			}
			cout << endl; cout << endl;
			for (int i = 0; i < V.size(); i++) {
				Mostrar(V[i]);
			}
			break;
		case 4:

			cout << "Por que criterio desea ordenar???" << endl;
			cout << endl;
			cout << "1)Nombre" << endl;
			cout << "2)Extension" << endl;
			cout << "3)Tamano" << endl;
			cout << "4)Fecha" << endl;
			cout << endl;
			cin >> x2;
			cout << endl; cout << endl;
			cout << "Ascendente(1) o descendente(2)???" << endl;
			cin >> ord;
			cout << endl;

			if (ord == 1) {
				if (x2 == 1) {
					V = avlnom->inorderV();
					avlnom->inorder(Mostrar);
				}
				else if (x2 == 2) {
					V = avlext->inorderV();
					avlext->inorder(Mostrar);
				}
				else if (x2 == 3) {
					V = avltam->inorderV();
					avltam->inorder(Mostrar);
				}
				else if (x2 == 4) {
					V = avlfec->inorderV();
					avlfec->inorder(Mostrar);
				}

			}
			else if (ord == 2) {

				if (x2 == 1) {
					V = avlnom->antiinorderV();
					for (int i = 0; i < V.size(); i++) {
						Mostrar(V[i]);
					}
				}
				else if (x2 == 2) {

					V = avlext->antiinorderV();
					for (int i = 0; i < V.size(); i++) {
						Mostrar(V[i]);
					}
				}
				else if (x2 == 3) {

					V = avltam->antiinorderV();
					for (int i = 0; i < V.size(); i++) {
						Mostrar(V[i]);
					}
				}
				else if (x2 == 4) {
					V = avlfec->antiinorderV();
					for (int i = 0; i < V.size(); i++) {
						Mostrar(V[i]);
					}
				}
			}
			cout << endl; cout << endl;
			break;
		}
		cout << "Desea hacer otra operaci�n (1: si, 2 : no)" << endl;
		cout << endl;
		cin >> fl;
		system("cls");
	} while (fl == 1);
	system("pause");
	return 0;
}