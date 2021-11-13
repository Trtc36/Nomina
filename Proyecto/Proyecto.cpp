#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>
#include <ctime>

#define MAX 80
#define VALOR_CENTINELA -1

using namespace std;

struct DPersonales {
	int noEMpleado;
	char Nombre[50];
	char APaterno[50];
	char AMaterno[50];
	char RFC[50];
	char NSS[50];
	char Dpto[50];
};

typedef struct DPersonales dpersonales;

struct Percepciones {
	float S_Base;
	int D_Pagados;
	float H_Extras;
	float S_Normal;
	float H_extra_s;
	float H_extra_d;
	float Total_P;
	float Neto;
};

Percepciones P_Empleado;

struct Deducciones {
	float ISR;
	float IMSS;
	float Total_D;
};

Deducciones D_Empleado;

FILE *bd;

void gotoxy(int x, int y);
void menu();
void plantilla();
void alta();
char existeEmpleado(int cEmpleado, DPersonales *Empleado);
char insertarEmpleado(DPersonales Empleado);
void buscar();
void eliminar();
char eliminarEmpleado(int cEmpleado);
void modificar();
void nomina();
void Fecha();
void Fecha2(struct Percepciones P_Empleado);
float Calculo(struct Percepciones P_Empleado, struct Deducciones D_Empleado);
void Convertir(float numero);
void buscar_nomina();
void menu_ordenar();
void ordenar_paterno(FILE *db);
void imprimir_p(DPersonales Empleado[500], int total);
void ordenar_materno(FILE *db);
void imprimir_m(DPersonales Empleado[500], int total);
void ordenar_nombre(FILE *db);
void imprimir_n(DPersonales Empleado[500], int total);



int main()
{
	setlocale(LC_ALL, "spanish"); /* Permite imprimir caracteres con tilde */
	menu();
}

void gotoxy(int x, int y) {
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

void menu() {
	system("color F1");
	int op;
	do
	{
		system("cls");
		plantilla();
		gotoxy(5, 9); printf_s("Menú:");
		gotoxy(7, 10); printf_s("Opción 1: Alta ------------------------> \"1\"");
		gotoxy(7, 11); printf_s("Opción 2: Consulta --------------------> \"2\"");
		gotoxy(7, 12); printf_s("Opción 3: Ordenación ------------------> \"3\"");
		gotoxy(7, 13); printf_s("Opción 4: Nomina ----------------------> \"4\"");
		gotoxy(7, 14); printf_s("Opción 5: Modificar Empleado ----------> \"5\"");
		gotoxy(7, 15); printf_s("Opción 6: Eliminar Empleado -----------> \"6\"");
		gotoxy(7, 16); printf_s("Opción 7: Eliminación Fisica ----------> \"7\"");
		gotoxy(7, 17); printf_s("Opción 8: Salir -----------------------> \"8\"");
		gotoxy(7, 19); printf_s("Opción: "); scanf_s("%d", &op);

		switch (op)
		{
		case 1:
			system("cls");
			alta();
			break;
		case 2:
			system("cls");
			buscar();
			break;
		case 3:
			system("cls");
			menu_ordenar();
			_getch();
			break;
		case 4:
			system("cls");
			nomina();
			_getch();
			break;
		case 5:
			system("cls");
			modificar();
			break;
		case 6:
			system("cls");
			eliminar();
			break;
		case 7:
			break;
		default:
			system("cls");
			plantilla();
			if(op == 8){ gotoxy(5, 9); printf_s("Tchüss"); }	
			else{ gotoxy(5, 9); printf_s("La opción %d no es valida!", op); }
			_getch();
			break;
		}
	} while (op != 8);
}

void plantilla() {
	gotoxy(5, 5); printf_s("_____________________________________________________________");
	gotoxy(5, 6); printf_s("               .::Universidad Quetzalcóatl::.");
	gotoxy(5, 7); printf_s("                  .::Estructura de Datos::.");
	gotoxy(5, 8); printf_s("_____________________________________________________________");
	gotoxy(5, 25); printf_s("____________________________________________________________");
	gotoxy(5, 26); printf_s(".::Ingeniería en Tecnologías de la Información y Negocios::.");
	gotoxy(5, 27);printf_s("             .::Francisco Javier Ramos Elizarraraz::.");
	gotoxy(5, 28); printf_s("_____________________________________________________________");
}

void alta()
{
	DPersonales Empleado;
	int cEmpleado = 0;
	char repite = 1;
	char Resp[MAX];

	do
	{
		system("cls");
		plantilla();

		gotoxy(5, 9); printf_s("ALTA EMPLEADO:");

		gotoxy(7, 10); printf_s("No. Empleado: "); scanf_s("%d", &cEmpleado);

		if (!existeEmpleado(cEmpleado, &Empleado)) {
			Empleado.noEMpleado = cEmpleado;
			system("cls");
			plantilla();
			gotoxy(7, 9); printf("No. Empleado: %d", Empleado.noEMpleado);
			gotoxy(7, 10); printf_s("Nombre(s): "); cin >> Empleado.Nombre;
			gotoxy(7, 11); printf_s("A. Paterno: "); cin >> Empleado.APaterno;
			gotoxy(7, 12); printf_s("A. Materno: "); cin >> Empleado.AMaterno;
			gotoxy(7, 13); printf_s("RFC: "); cin >> Empleado.RFC;
			gotoxy(7, 14); printf_s("NSS: "); cin >> Empleado.NSS;
			gotoxy(7, 15); printf_s("Depto: "); cin >> Empleado.Dpto;

			if (insertarEmpleado(Empleado)) {
				gotoxy(5, 17); printf_s("El Empleado fue insertado correctamente");
			}
			else {
				gotoxy(5, 17); printf_s("Ocurrió un error al intentar insertar el empleado");
				gotoxy(5, 18); printf_s("Inténtelo mas tarde.");
			}
		}
		else {
			system("cls");
			plantilla();
			gotoxy(5, 9); printf_s("El empleado de código %d ya existe.", cEmpleado);
			gotoxy(5, 10); printf_s("No puede ingresar dos empleados distintos con el mismo número.");
		}

		gotoxy(5, 18); printf_s("Desea seguir ingresando empleados? [S/N]: ");
		cin >> Resp;

		if (!(strcmp(Resp, "S") == 0 || strcmp(Resp, "s") == 0)) {
			repite = 0;
		}
	} while (repite);
}

char existeEmpleado(int cEmpleado, DPersonales *Empleado)
{
	FILE *db;
	char existe;

	db = fopen("Registros.dat", "rb");
	if (db == NULL) {
		existe = 0;
	}
	else {
		existe = 0;
		fread(&(*Empleado), sizeof(*Empleado), 1, db);
		while (!feof(db)) {
			if ((*Empleado).noEMpleado == cEmpleado) {
				existe = 1;
				break;
			}
			fread(&(*Empleado), sizeof(*Empleado), 1, db);
		}
		fclose(db);
	}
	return existe;
}

char insertarEmpleado(DPersonales Empleado)
{
	FILE *db;
	char insertar;

	db = fopen("Registros.dat", "ab");
	if (db == NULL) {
		insertar = 0;
	}
	else {
		fwrite(&Empleado, sizeof(Empleado), 1, db);
		insertar = 1;
		fclose(db);
	}
	return insertar;
}

void buscar()
{
	DPersonales Empleado;
	int cEmpleado;
	char Repite = 1;
	char Resp[MAX];

	do
	{
		system("cls");
		plantilla();
		gotoxy(5, 9); printf_s("BUSCAR EMPLEADO POR NÚMERO: ");
		gotoxy(7, 10); printf_s("No. Empleado: "); cin >> cEmpleado;

		if (existeEmpleado(cEmpleado, &Empleado)) {
			gotoxy(7, 11); printf_s("Nombre(s): %s", Empleado.Nombre);
			gotoxy(7, 12); printf_s("A. Paterno: %s", Empleado.APaterno);
			gotoxy(7, 13); printf_s("A. Materno: %s", Empleado.AMaterno);
			gotoxy(7, 14); printf_s("RFC: %s", Empleado.RFC);
			gotoxy(7, 15); printf_s("NSS: %s", Empleado.NSS);
			gotoxy(7, 16); printf_s("Depto: %s", Empleado.Dpto);
		}
		else {
			gotoxy(5, 11); printf_s("El Empleado de número %d no existe.", cEmpleado);
		}

		gotoxy(5, 18); printf_s("Desea seguir buscando algún empleado? [S/N]: ");
		cin >> Resp;

		if (!(strcmp(Resp, "S") == 0 || strcmp(Resp, "s") == 0)) {
			Repite = 0;
		}
	} while (Repite);
}

void eliminar()
{
	DPersonales Empleado;
	int cEmpleado;
	char repite = 1;
	char respuesta[MAX];

	do
	{
		plantilla();
		gotoxy(5, 9); printf_s("ELIMINAR EMPLEADO:");
		gotoxy(7, 10); printf_s("No. Empleado: "); scanf_s("%d", &cEmpleado);

		if (existeEmpleado(cEmpleado, &Empleado)) {
			gotoxy(5, 11); printf_s("Nombre(s): %s", Empleado.Nombre);
			gotoxy(5, 12); printf_s("A. Paterno: %s", Empleado.APaterno);
			gotoxy(5, 13); printf_s("A. Materno: %s", Empleado.AMaterno);
			gotoxy(5, 14); printf_s("RFC: %s", Empleado.RFC);
			gotoxy(5, 15); printf_s("NSS: %s", Empleado.NSS);
			gotoxy(5, 16); printf_s("Depto: %s", Empleado.Dpto);

			gotoxy(5, 18); printf_s("Seguro que desea eliminar el Empleado? [S/N]: "); cin >> respuesta;

			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				if (eliminarEmpleado(cEmpleado)) {
					gotoxy(5, 19); printf_s("Empleado eliminado satisfactoriamente.");
				}
				else {
					gotoxy(5, 20); printf_s("El Empleado no pudo ser eliminado");
				}
			}
		}
		else {
			gotoxy(5, 12); printf_s("El Empleado de código %d no existe.", cEmpleado);
		}

		gotoxy(5, 21); printf_s("Desea eliminar otro Empleado? [S/N]: ");
		cin>>respuesta;

		if (!(strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)) {
			repite = 0;
		}
		
	} while (repite);
}

char eliminarEmpleado(int cEmpleado)
{
	FILE *BD;
	//FILE *AUX;
	DPersonales Empleado;
	char eliminar;

	BD = fopen("Registros.dat", "r+b");
	if (BD == NULL) {
		eliminar = 0;
	}
	else {
		eliminar = 0;
		fread(&Empleado, sizeof(Empleado), 1, BD);
		while (!feof(BD))
		{
			if (Empleado.noEMpleado == cEmpleado) {
				fseek(BD, ftell(BD) - sizeof(Empleado), SEEK_SET);
				Empleado.noEMpleado = VALOR_CENTINELA;
				fwrite(&Empleado, sizeof(Empleado), 1, BD);
				eliminar = 1;
				break;
			}
			fread(&Empleado, sizeof(Empleado), 1, BD);
		}
		fclose(BD);
	}
	return eliminar;
}

void modificar()
{
	DPersonales Empleado;
	int cEmpleado;
	char repite = 1;
	char respuesta[MAX];

	do
	{
		plantilla();
		gotoxy(5, 9); printf_s("MDIFICAR EMPLEADO:");
		gotoxy(7, 10); printf_s("No. Empleado: "); scanf_s("%d", &cEmpleado);

		if (existeEmpleado(cEmpleado, &Empleado)) {
			gotoxy(7, 11); printf_s("Nombre(s): %s",Empleado.Nombre);
			gotoxy(7, 12); printf_s("A. Paterno: %s", Empleado.APaterno);
			gotoxy(7, 13); printf_s("A. Materno: %s", Empleado.AMaterno);
			gotoxy(7, 14); printf_s("RFC: %s", Empleado.RFC);
			gotoxy(7, 15); printf_s("NSS: %s", Empleado.NSS);
			gotoxy(7, 16); printf_s("Depto.: %s", Empleado.Dpto);

			gotoxy(7, 18); printf_s("A continuación elija los datos a modificar");
			_getch();

			system("cls"); plantilla();
			gotoxy(5, 9); printf_s("Desea modificar el nombre del empleado? [S/N]: "); cin >> respuesta;
			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				gotoxy(5, 10); printf_s("Nuevo nombre del Empleado: ");
				cin>>Empleado.Nombre;
			}

			system("cls"); plantilla();
			gotoxy(5, 9); printf_s("Desea modificar el apellido paterno del empleado? [S/N]: "); cin >> respuesta;
			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				gotoxy(5, 10); printf_s("Nuevo apellido paterno del Empleado: ");
				cin >> Empleado.APaterno;
			}

			system("cls"); plantilla();
			gotoxy(5, 9); printf_s("Desea modificar el apellido materno del empleado? [S/N]: "); cin >> respuesta;
			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				gotoxy(5, 10); printf_s("Nuevo apellido materno del Empleado: ");
				cin >> Empleado.AMaterno;
			}

			system("cls"); plantilla();
			gotoxy(5, 9); printf_s("Desea modificar el RFC del empleado? [S/N]: "); cin >> respuesta;
			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				gotoxy(5, 10); printf_s("Nuevo RFC del Empleado: ");
				cin >> Empleado.RFC;
			}

			system("cls"); plantilla();
			gotoxy(5, 9); printf_s("Desea modificar el NSS del empleado? [S/N]: "); cin >> respuesta;
			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				gotoxy(5, 10); printf_s("Nuevo NSS del Empleado: ");
				cin >> Empleado.NSS;
			}

			system("cls"); plantilla();
			gotoxy(5, 9); printf_s("Desea modificar el departamento del empleado? [S/N]: "); cin >> respuesta;
			if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0) {
				gotoxy(5, 10); printf_s("Nuevo departamento del Empleado: ");
				cin >> Empleado.Dpto;
			}


		}
	} while (repite);
}

void nomina()
{
	plantilla();
	gotoxy(5, 9); printf_s("Teclea el Salario base: "); scanf_s("%f", &P_Empleado.S_Base);
	gotoxy(5, 10); printf_s("Teclea los dias pagados: "); scanf_s("%d", &P_Empleado.D_Pagados);
	gotoxy(5, 11); printf_s("Teclea las horas extras: "); scanf_s("%f", &P_Empleado.H_Extras);
	buscar_nomina();
	gotoxy(5, 2);
	printf_s("MUEBLERIA HIDALGO S.A de C.V.");
	gotoxy(5, 4);
	printf_s("RAEF890204 DSA");
	gotoxy(5, 5);
	printf_s("Morelos #71, Col.Centro.");
	gotoxy(5, 6);
	printf_s("Penjamo, Guanajuato");
	gotoxy(35, 8);
	printf_s("RECIBO DE PAGO");
	gotoxy(70, 5);
	printf_s("Fecha de Pago");
	gotoxy(70, 6);
	Fecha();
	gotoxy(5, 10);
	printf_s("Nombre: ");
	gotoxy(5, 11);
	printf_s("R.F.C.: ");
	gotoxy(5, 12);
	printf_s("N.S.S.: ");
	gotoxy(5, 13);
	printf_s("Dpto.: ");
	gotoxy(45, 11);
	printf_s("Salario Diario: ");
	gotoxy(45, 12);
	printf_s("Días Pagados: ");
	gotoxy(45, 13);
	printf_s("Horas Extras: ");
	gotoxy(70, 11);
	printf_s("Periodo de Pago");
	gotoxy(70, 12);
	printf_s("Del: ");
	Fecha();
	gotoxy(70, 13);
	printf_s("Al: ");
	Fecha2(P_Empleado);
	gotoxy(5, 15);
	printf_s("P E R C E P C I O N E S");
	gotoxy(50, 15);
	printf_s("D E D U C C I O N E S");
	gotoxy(5, 16);
	printf_s("_________________________________________________________________________________________");
	gotoxy(5, 17);
	printf_s("Salario Normal");
	gotoxy(5, 18);
	printf_s("Total H. Extras Simples");
	gotoxy(5, 19);
	printf_s("Total H. Extras Dobles");
	gotoxy(5, 21);
	printf_s("_________________________________________________________________________________________");
	gotoxy(50, 17);
	printf_s("ISR");
	gotoxy(50, 18);
	printf_s("IMSS(seguro Social)");
	gotoxy(5, 22);
	printf_s("Total Percepciones");
	gotoxy(50, 22);
	printf_s("Total Deducciones");
	gotoxy(50, 23);
	printf_s("Neto Recibido");
	gotoxy(5, 26);
	printf_s("Recibí de conformidad las prestaciones correspondientes\n     al periodo que se indica arriba y liquida totalmente");
	gotoxy(5, 28);
	printf_s("hasta esta fecha mi salario ordinario, extraordinario,\n     7o día, salario de compensación y demas prestaciones, ");
	gotoxy(5, 30);
	printf_s("entendiendo que las deducciones tabto legales como de\n     caracter primadose han aplicado a cubrir los adeudos\n     respectivos.");
	gotoxy(67, 28);
	printf_s("___________________________");
	gotoxy(77, 29);
	printf_s("F I R M A");
	Calculo(P_Empleado, D_Empleado);
}

void Fecha()
{
	time_t now = time(0);
	tm *time = localtime(&now);

	int a = (1900 + time->tm_year);
	if(time->tm_mon < 10)
		printf_s("%d - 0%d - %d", time->tm_mday, time->tm_mon + 1, a);
	else
		printf_s("%d - %d - %d", time->tm_mday, time->tm_mon + 1, a);
}

void Fecha2(struct Percepciones P_Empleado)
{
	time_t now = time(0);
	tm *time = localtime(&now);

	int a = (1900 + time->tm_year);

	if (time->tm_mon < 10) {
		if (time->tm_mon == 0 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday) * -1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 1 && (time->tm_mday + P_Empleado.D_Pagados) > 28) {
			printf_s("%d - 0%d - %d", ((28 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 2 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 3 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - 0%d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 4 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 5 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - 0%d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 6 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 7 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 8 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - 0%d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 9 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 10 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - 0%d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 11 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - 0%d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else {
			printf_s("%d - 0%d - %d", (P_Empleado.D_Pagados + time->tm_mday), time->tm_mon + 1, a);
		}
	}
	else {
		if (time->tm_mon == 0 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 1 && (time->tm_mday + P_Empleado.D_Pagados) > 28) {
			printf_s("%d - %d - %d", ((28 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 2 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 3 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - %d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 4 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 5 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - %d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 6 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 7 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 8 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - %d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 9 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 10 && (time->tm_mday + P_Empleado.D_Pagados) > 30) {
			printf_s("%d - %d - %d", ((30 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else if (time->tm_mon == 11 && (time->tm_mday + P_Empleado.D_Pagados) > 31) {
			printf_s("%d - %d - %d", ((31 - time->tm_mday)*-1), time->tm_mon + 1, a);
		}
		else {
			printf_s("%d - %d - %d", (P_Empleado.D_Pagados + time->tm_mday), time->tm_mon + 1, a);
		}
	}
		
}

float Calculo(Percepciones P_Empleado, Deducciones D_Empleado)
{
	float num;
	P_Empleado.S_Normal = P_Empleado.S_Base * P_Empleado.D_Pagados;

	if (P_Empleado.H_Extras > 8) {
		P_Empleado.H_extra_s = (P_Empleado.S_Base / 8) * 8;
		P_Empleado.H_extra_d = ((P_Empleado.H_Extras - 8) * (P_Empleado.S_Base / 8)) * 2;
	}
	else {
		P_Empleado.H_extra_s = P_Empleado.H_Extras * P_Empleado.S_Base;
	}

	P_Empleado.Total_P = P_Empleado.S_Normal + P_Empleado.H_extra_s + P_Empleado.H_extra_d;

	if (P_Empleado.Total_P >= 0.01 && P_Empleado.Total_P <= 496.07)
		D_Empleado.ISR = ((P_Empleado.Total_P - 0.01) * 0.0192);
	else if (P_Empleado.Total_P >= 496.08 && P_Empleado.Total_P <= 4210.41)
		D_Empleado.ISR = ((P_Empleado.Total_P - 496.08) * 0.064) + 9.52;
	else if (P_Empleado.Total_P >= 4210.42 && P_Empleado.Total_P <= 7399.42)
		D_Empleado.ISR = ((P_Empleado.Total_P - 4210.42) * 0.1088) + 247.24;
	else if (P_Empleado.Total_P >= 7399.43 && P_Empleado.Total_P <= 8601.50)
		D_Empleado.ISR = ((P_Empleado.Total_P - 7399.43) * 0.16) + 594.21;
	else if (P_Empleado.Total_P >= 8601.51 && P_Empleado.Total_P <= 10298.35)
		D_Empleado.ISR = ((P_Empleado.Total_P - 8601.51) * 0.1792) + 786.54;
	else if (P_Empleado.Total_P >= 10298.36 && P_Empleado.Total_P <= 20770.29)
		D_Empleado.ISR = ((P_Empleado.Total_P - 10238.36) * 0.2136) + 1090.61;
	else if (P_Empleado.Total_P >= 20770.30 && P_Empleado.Total_P <= 32736.83)
		D_Empleado.ISR = ((P_Empleado.Total_P - 20770.30) * 0.2352) + 3327.42;
	else if (P_Empleado.Total_P >= 32736.84 && P_Empleado.Total_P <= 62500)
		D_Empleado.ISR = ((P_Empleado.Total_P - 32736.84) * 0.3) + 6141.95;
	else if (P_Empleado.Total_P >= 62500.01 && P_Empleado.Total_P <= 83333.33)
		D_Empleado.ISR = ((P_Empleado.Total_P - 62500.01) * 0.32) + 15070.90;
	else if (P_Empleado.Total_P >= 83333.34 && P_Empleado.Total_P <= 250000)
		D_Empleado.ISR = ((P_Empleado.Total_P - 83333.34) * 0.34) + 21737.57;
	else if (P_Empleado.Total_P >= 250000.01)
		D_Empleado.ISR = ((P_Empleado.Total_P - 250000.01) * 0.064) + 9.52;

	D_Empleado.IMSS = P_Empleado.Total_P * 0.03;
	D_Empleado.Total_D = D_Empleado.ISR + D_Empleado.IMSS;
	P_Empleado.Neto = P_Empleado.Total_P - D_Empleado.Total_D;

	gotoxy(36, 17);
	printf("%.2f", P_Empleado.S_Normal);
	gotoxy(36, 18);
	printf("%.2f", P_Empleado.H_extra_s);
	gotoxy(36, 19);
	printf("%.2f", P_Empleado.H_extra_d);
	gotoxy(36, 22);
	printf("%.2f", P_Empleado.Total_P);
	gotoxy(78, 17);
	printf("%.2f", D_Empleado.ISR);
	gotoxy(78, 18);
	printf("%.2f", D_Empleado.IMSS);
	gotoxy(78, 22);
	printf("%.2f", D_Empleado.Total_D);
	gotoxy(78, 23);
	printf("%.2f", P_Empleado.Neto);
	gotoxy(5, 24);
	Convertir(P_Empleado.Neto);
	return 0.0f;
}

void Convertir(float numero)
{
	int n1, n2, n3, n4, n5, n6, n7;
	float c;


	n1 = numero / 1000000;
	n2 = ((numero - n1 * 1000000) / 100000);
	n3 = (((numero - n1 * 1000000) - n2 * 100000) / 10000);
	n4 = ((((numero - n1 * 1000000) - n2 * 100000) - n3 * 10000) / 1000);
	n5 = (((((numero - n1 * 1000000) - n2 * 100000) - n3 * 10000) - n4 * 1000) / 100);
	n6 = ((((((numero - n1 * 1000000) - n2 * 100000) - n3 * 10000) - n4 * 1000) - n5 * 100) / 10);
	n7 = ((((((numero - n1 * 1000000) - n2 * 100000) - n3 * 10000) - n4 * 1000) - n5 * 100) - n6 * 10);
	c = ((((((((numero - n1 * 1000000) - n2 * 100000) - n3 * 10000) - n4 * 1000) - n5 * 100) - n6 * 10) - n7) * 100);


	/*printf("%d millones\n", n1);
	printf("%d cienmiles\n",n2);
	printf("%d diezmiles\n",n3);
	printf("%d miles\n",n4);
	printf("%d cientos\n",n5);
	printf("%d decenas\n",n6);
	printf("%d unidades\n",n7);
	printf("%.0f/100",c);*/

	if (numero > 9999999) {
		printf("ERROR!");
	}
	else {
		if (n1 == 1)
			cout << "Un Millon ";
		if (n1 == 2)
			cout << "Dos Millones ";
		if (n1 == 3)
			cout << "Tres Millones ";
		if (n1 == 4)
			cout << "Cuatro Millones ";
		if (n1 == 5)
			cout << "Cinco Millones ";
		if (n1 == 6)
			cout << "Seis Millones ";
		if (n1 == 7)
			cout << "Siete Millones ";
		if (n1 == 8)
			cout << "Ocho Millones ";
		if (n1 == 9)
			cout << "Nueve Millones ";

		if (n2 == 1 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0) {
			cout << "Cien Mil ";
		}
		else if (n2 == 2 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Doscientos Mil ";
		else if (n2 == 3 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Trescientos Mil ";
		else if (n2 == 4 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Cuatrocientos Mil ";
		else if (n2 == 5 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Quinientos Mil ";
		else if (n2 == 6 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Seiscientos Mil ";
		else if (n2 == 7 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Setecientos Mil ";
		else if (n2 == 8 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Ochocientos Mil ";
		else if (n2 == 9 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Novecientos Mil ";
		else {
			if (n2 == 1)
				cout << "Ciento ";
			if (n2 == 2)
				cout << "Doscientos ";
			if (n2 == 3)
				cout << "Trescientos ";
			if (n2 == 4)
				cout << "Cuatrocientos ";
			if (n2 == 5)
				cout << "Quinientos ";
			if (n2 == 6)
				cout << "Seiscientos ";
			if (n2 == 7)
				cout << "Setecientos ";
			if (n2 == 8)
				cout << "Ochocientos ";
			if (n2 == 9)
				cout << "Novecientos ";
		}

		if (n3 == 1 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Diez mil ";
		else if (n3 == 2 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Veinte mil ";
		else if (n3 == 3 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Treinta mil ";
		else if (n3 == 4 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Cuarenta mil ";
		else if (n3 == 5 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Cincuenta mil ";
		else if (n3 == 6 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Secenta mil ";
		else if (n3 == 7 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Setenta mil ";
		else if (n3 == 8 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Ochennta mil ";
		else if (n3 == 9 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
			cout << "Noventa mil ";
		else {
			if (n3 == 1) {
				if (n4 == 1)
					cout << "Once Mil";
				if (n4 == 2)
					cout << "Doce Mil";
				if (n4 == 3)
					cout << "Trece Mil";
				if (n4 == 4)
					cout << "Catorce Mil";
				if (n4 == 5)
					cout << "Quince Mil";
				if (n4 == 6)
					cout << "Diez y Seis Mil";
				if (n4 == 7)
					cout << "Diez y Siete Mil";
				if (n4 == 8)
					cout << "Diez y Ocho Mil";
				if (n4 == 6)
					cout << "Diez y Nueve Mil";
			}
			else if (n3 == 2)
				cout << "Veinte y ";
			else if (n3 == 3)
				cout << "Treinta y ";
			else if (n3 == 4)
				cout << "Cuarenta y ";
			else if (n3 == 5)
				cout << "Cincuenta y ";
			else if (n3 == 6)
				cout << "Secenta y ";
			else if (n3 == 7)
				cout << "Setenta y ";
			else if (n3 == 8)
				cout << "Ochennta y ";
			else if (n3 == 9)
				cout << "Noventa y ";


			if (n4 == 1)
				cout << "Un Mil ";
			else if (n4 == 2)
				cout << "Dos Mil ";
			else if (n4 == 3)
				cout << "Tres Mil ";
			else if (n4 == 4)
				cout << "Cuatro Mil ";
			else if (n4 == 5)
				cout << "Cinco Mil ";
			else if (n4 == 6)
				cout << "Seis Mil ";
			else if (n4 == 7)
				cout << "Siete Mil ";
			else if (n4 == 8)
				cout << "Ocho Mil ";
			else if (n4 == 9)
				cout << "Nueve Mil ";

		}

		if (n5 == 1 && n6 == 0 && n7 == 0)
			cout << " Cien ";
		else if (n5 == 1)
			cout << "Ciento ";
		else if (n5 == 2)
			cout << "Doscientos";
		else if (n5 == 3)
			cout << "Trescientos";
		else if (n5 == 4)
			cout << "Cuatrocientos";
		else if (n5 == 5)
			cout << "Quinientos";
		else if (n5 == 6)
			cout << "Seiscientos";
		else if (n5 == 7)
			cout << "Setecientos";
		else if (n5 == 8)
			cout << "Ochocientos";
		else if (n5 == 9)
			cout << "Novecientos";


		if (n6 == 1) {
			if (n7 == 1)
				cout << " Once";
			if (n7 == 2)
				cout << " Doce";
			if (n7 == 3)
				cout << " Trece";
			if (n7 == 4)
				cout << " Catorce";
			if (n7 == 5)
				cout << " Quince";
			if (n7 == 6)
				cout << " Diez y Seis";
			if (n7 == 7)
				cout << " Diez y Siete";
			if (n7 == 8)
				cout << " Diez y Ocho";
			if (n7 == 9)
				cout << " Diez y Nueve";
		}
		else if (n6 == 2)
			cout << " Veinte y";
		else if (n6 == 3)
			cout << " Treinta y";
		else if (n6 == 4)
			cout << " Cuarenta y";
		else if (n6 == 5)
			cout << " Cincuenta y";
		else if (n6 == 6)
			cout << " Sesenta y";
		else if (n6 == 7)
			cout << " Setenta y";
		else if (n6 == 8)
			cout << " Ochenta y";
		else if (n6 == 9)
			cout << " Noventa y";


		if (n7 == 1)
			cout << " Uno";
		else if (n7 == 2)
			cout << " Dos";
		else if (n7 == 3)
			cout << " Tres";
		else if (n7 == 4)
			cout << " Cuatro";
		else if (n7 == 5)
			cout << " Cinco";
		else if (n7 == 6)
			cout << " Seis";
		else if (n7 == 7)
			cout << " Siete";
		else if (n7 == 8)
			cout << " Ocho";
		else if (n7 == 9)
			cout << " Nueve";


		printf(" Pesos %.0f/100 M.N. ", c);


	}
}

void buscar_nomina()
{
	DPersonales Empleado;
	int cEmpleado;
	char Repite = 1;
	char Resp[MAX];

	
		
		gotoxy(5, 12); printf_s("No. Empleado: "); cin >> cEmpleado;
		system("cls");
		if (existeEmpleado(cEmpleado, &Empleado)) {
			gotoxy(13, 10); printf_s("%s %s %s", Empleado.APaterno, Empleado.AMaterno, Empleado.Nombre);
			gotoxy(13, 11); printf_s("%s", Empleado.RFC);
			gotoxy(13, 12); printf_s("%s", Empleado.NSS);
			gotoxy(12, 13); printf_s("%s", Empleado.Dpto);
		}
		else {
			gotoxy(5, 11); printf_s("El Empleado de número %d no existe.", cEmpleado);
			
		}
}

void menu_ordenar()
{
	int op1;
	do {
		system("cls");
		plantilla();
		gotoxy(5, 9); printf_s("Elije la opción de ordenamiento: ");
		gotoxy(7, 10); printf_s("Opción 1: Por Apellido Paterno -----> \"1\"");
		gotoxy(7, 11); printf_s("Opción 2: Por Apellido Materno -----> \"2\"");
		gotoxy(7, 12); printf_s("Opción 3: Por Nombre ---------------> \"3\"");
		gotoxy(7, 13); printf_s("Opción: "); scanf_s("%d", &op1);
	} while (op1 < 1 || op1 >3);

	switch (op1)
	{
	case 1:
		system("cls"); 
		ordenar_paterno(bd);
		break;
	case 2:
		system("cls");
		ordenar_materno(bd);
		break;
	case 3:
		system("cls"); 
		ordenar_nombre(bd);
		break;
	}
}

void ordenar_paterno(FILE *db){
	DPersonales Empleado[500], Emp, aux;
	int Cant = 0, Total = 0, i, j;
	db = fopen("Registros.dat", "r");
	fread(&Emp, sizeof(*Empleado), 1, db);
	while (!feof(db)) {
		Empleado[Cant] = Emp;
		Cant++;
		fread(&Emp, sizeof(*Empleado), 1, db);
	}
	fclose(db);
	Total = Cant - 1;

	for (int i = 0; i < Total; i++) {
		for (int j = 0; j < Total; j++) {
			if (strcmp(Empleado[j].APaterno, Empleado[j + 1].APaterno) > 0) {
				aux.noEMpleado = Empleado[j].noEMpleado;
				strcpy(aux.Nombre, Empleado[j].Nombre);
				strcpy(aux.APaterno, Empleado[j].APaterno);
				strcpy(aux.AMaterno, Empleado[j].AMaterno);
				strcpy(aux.RFC, Empleado[j].RFC);
				strcpy(aux.NSS, Empleado[j].NSS);
				strcpy(aux.Dpto, Empleado[j].Dpto);

				Empleado[j].noEMpleado = Empleado[j + 1].noEMpleado;
				strcpy(Empleado[j].Nombre, Empleado[j + 1].Nombre);
				strcpy(Empleado[j].APaterno, Empleado[j + 1].APaterno);
				strcpy(Empleado[j].AMaterno, Empleado[j + 1].AMaterno);
				strcpy(Empleado[j].RFC, Empleado[j + 1].RFC);
				strcpy(Empleado[j].NSS, Empleado[j + 1].NSS);
				strcpy(Empleado[j].Dpto, Empleado[j + 1].Dpto);

				Empleado[j + 1].noEMpleado = aux.noEMpleado;
				strcpy(Empleado[j + 1].Nombre, aux.Nombre);
				strcpy(Empleado[j + 1].APaterno, aux.APaterno);
				strcpy(Empleado[j + 1].AMaterno, aux.AMaterno);
				strcpy(Empleado[j + 1].RFC, aux.RFC);
				strcpy(Empleado[j + 1].NSS, aux.NSS);
				strcpy(Empleado[j + 1].Dpto, aux.Dpto);
			}

		}
	}
	imprimir_p(Empleado, Total);
}

void imprimir_p(DPersonales Empleado[500], int total) {
	int j = 0;
	system("cls");
	gotoxy(5, 4);
	printf("_____________________________________________________________________________________________________");
	gotoxy(5, 5);
	printf("No Emp    Apellido Paterno    Apellido Materno    Nombre(s)    RFC            NSS            Depto\n");
	gotoxy(5, 6);
	printf("_____________________________________________________________________________________________________");
	for (int i = 0; i <= total; i++) {
		if(Empleado[i].noEMpleado >0){
			gotoxy(5, j + 7);
			printf_s("%d", Empleado[i].noEMpleado);
			gotoxy(15, j + 7);
			printf("%s", Empleado[i].APaterno);
			gotoxy(35, j + 7);
			printf("%s", Empleado[i].AMaterno);
			gotoxy(55, j + 7);
			printf("%s", Empleado[i].Nombre);
			gotoxy(68, j + 7);
			printf("%s", Empleado[i].RFC);
			gotoxy(83, j + 7);
			printf("%s", Empleado[i].NSS);
			gotoxy(98, j + 7);
			printf("%s", Empleado[i].Dpto);
			j++;
		}
		
	}
	printf("\n     _____________________________________________________________________________________________________");
}

void ordenar_materno(FILE *db) {
	DPersonales Empleado[500], Emp, aux;
	int Cant = 0, Total = 0, i, j;
	db = fopen("Registros.dat", "r");
	fread(&Emp, sizeof(*Empleado), 1, db);
	while (!feof(db)) {
		Empleado[Cant] = Emp;
		Cant++;
		fread(&Emp, sizeof(*Empleado), 1, db);
	}
	fclose(db);
	Total = Cant - 1;

	for (int i = 0; i < Total; i++) {
		for (int j = 0; j < Total; j++) {
			if (strcmp(Empleado[j].AMaterno, Empleado[j + 1].AMaterno) > 0) {
				aux.noEMpleado = Empleado[j].noEMpleado;
				strcpy(aux.Nombre, Empleado[j].Nombre);
				strcpy(aux.APaterno, Empleado[j].APaterno);
				strcpy(aux.AMaterno, Empleado[j].AMaterno);
				strcpy(aux.RFC, Empleado[j].RFC);
				strcpy(aux.NSS, Empleado[j].NSS);
				strcpy(aux.Dpto, Empleado[j].Dpto);

				Empleado[j].noEMpleado = Empleado[j + 1].noEMpleado;
				strcpy(Empleado[j].Nombre, Empleado[j + 1].Nombre);
				strcpy(Empleado[j].APaterno, Empleado[j + 1].APaterno);
				strcpy(Empleado[j].AMaterno, Empleado[j + 1].AMaterno);
				strcpy(Empleado[j].RFC, Empleado[j + 1].RFC);
				strcpy(Empleado[j].NSS, Empleado[j + 1].NSS);
				strcpy(Empleado[j].Dpto, Empleado[j + 1].Dpto);

				Empleado[j + 1].noEMpleado = aux.noEMpleado;
				strcpy(Empleado[j + 1].Nombre, aux.Nombre);
				strcpy(Empleado[j + 1].APaterno, aux.APaterno);
				strcpy(Empleado[j + 1].AMaterno, aux.AMaterno);
				strcpy(Empleado[j + 1].RFC, aux.RFC);
				strcpy(Empleado[j + 1].NSS, aux.NSS);
				strcpy(Empleado[j + 1].Dpto, aux.Dpto);
			}

		}
	}
	imprimir_m(Empleado, Total);
}

void imprimir_m(DPersonales Empleado[500], int total) {
	int j = 0;
	system("cls");
	gotoxy(5, 4);
	printf("_____________________________________________________________________________________________________");
	gotoxy(5, 5);
	printf("No Emp    Apellido Materno    Apellido Paterno    Nombre(s)    RFC            NSS            Depto\n");
	gotoxy(5, 6);
	printf("_____________________________________________________________________________________________________");
	for (int i = 0; i <= total; i++) {
		if (Empleado[i].noEMpleado >0){
			gotoxy(5, j + 7);
			printf_s("%d", Empleado[i].noEMpleado);
			gotoxy(15, j + 7);
			printf("%s", Empleado[i].AMaterno);
			gotoxy(35, j + 7);
			printf("%s", Empleado[i].APaterno);
			gotoxy(55, j + 7);
			printf("%s", Empleado[i].Nombre);
			gotoxy(68, j + 7);
			printf("%s", Empleado[i].RFC);
			gotoxy(83, j + 7);
			printf("%s", Empleado[i].NSS);
			gotoxy(98, j + 7);
			printf("%s", Empleado[i].Dpto);
			j++;
		}
		
	}
	printf("\n     _____________________________________________________________________________________________________");
}

void ordenar_nombre(FILE *db) {
	DPersonales Empleado[500], Emp, aux;
	int Cant = 0, Total = 0, i, j;
	db = fopen("Registros.dat", "r");
	fread(&Emp, sizeof(*Empleado), 1, db);
	while (!feof(db)) {
		Empleado[Cant] = Emp;
		Cant++;
		fread(&Emp, sizeof(*Empleado), 1, db);
	}
	fclose(db);
	Total = Cant - 1;

	for (int i = 0; i < Total; i++) {
		for (int j = 0; j < Total; j++) {
			if (strcmp(Empleado[j].Nombre, Empleado[j + 1].Nombre) > 0) {
				aux.noEMpleado = Empleado[j].noEMpleado;
				strcpy(aux.Nombre, Empleado[j].Nombre);
				strcpy(aux.APaterno, Empleado[j].APaterno);
				strcpy(aux.AMaterno, Empleado[j].AMaterno);
				strcpy(aux.RFC, Empleado[j].RFC);
				strcpy(aux.NSS, Empleado[j].NSS);
				strcpy(aux.Dpto, Empleado[j].Dpto);

				Empleado[j].noEMpleado = Empleado[j + 1].noEMpleado;
				strcpy(Empleado[j].Nombre, Empleado[j + 1].Nombre);
				strcpy(Empleado[j].APaterno, Empleado[j + 1].APaterno);
				strcpy(Empleado[j].AMaterno, Empleado[j + 1].AMaterno);
				strcpy(Empleado[j].RFC, Empleado[j + 1].RFC);
				strcpy(Empleado[j].NSS, Empleado[j + 1].NSS);
				strcpy(Empleado[j].Dpto, Empleado[j + 1].Dpto);

				Empleado[j + 1].noEMpleado = aux.noEMpleado;
				strcpy(Empleado[j + 1].Nombre, aux.Nombre);
				strcpy(Empleado[j + 1].APaterno, aux.APaterno);
				strcpy(Empleado[j + 1].AMaterno, aux.AMaterno);
				strcpy(Empleado[j + 1].RFC, aux.RFC);
				strcpy(Empleado[j + 1].NSS, aux.NSS);
				strcpy(Empleado[j + 1].Dpto, aux.Dpto);
			}

		}
	}
	imprimir_n(Empleado, Total);
}

void imprimir_n(DPersonales Empleado[500], int total) {
	int j = 0;
	system("cls");
	gotoxy(5, 4);
	printf("_____________________________________________________________________________________________________");
	gotoxy(5, 5);
	printf("No Emp    Nombre(s)    Apellido Paterno    Apellido Materno    RFC            NSS            Depto\n");
	gotoxy(5, 6);
	printf("_____________________________________________________________________________________________________");
	for (int i = 0; i <= total; i++) {
		if(Empleado[i].noEMpleado > 0){
			gotoxy(5, j + 7);
			printf_s("%d", Empleado[i].noEMpleado);
			gotoxy(15, j + 7);
			printf("%s", Empleado[i].Nombre);
			gotoxy(28, j + 7);
			printf("%s", Empleado[i].APaterno);
			gotoxy(48, j + 7);
			printf("%s", Empleado[i].AMaterno);
			gotoxy(68, j + 7);
			printf("%s", Empleado[i].RFC);
			gotoxy(83, j + 7);
			printf("%s", Empleado[i].NSS);
			gotoxy(98, j + 7);
			printf("%s", Empleado[i].Dpto);
			j++;
		}
		
	}
	printf("\n     _____________________________________________________________________________________________________");
} 