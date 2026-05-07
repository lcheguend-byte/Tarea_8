#pragma once
#include <mysql.h>
#include <iostream>
using namespace std;
class ConexionBD
{
private:
	MYSQL* conectar;
public:
	void abrirConexion() {
		conectar = mysql_init(0);
		conectar = mysql_real_connect(conectar, "localhost", "root", "Hunterxhunter.1", "db_escuela", 3306, NULL, 0);
	}
	MYSQL* getConexion() {
		return conectar;
	}
	void cerrarConexion() {
		mysql_close(conectar);
	}
};


