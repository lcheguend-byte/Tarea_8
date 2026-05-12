#pragma once
#include <iostream>
#include <string>
#include "ConexionBD.h"
#include "Persona.h"
#include <mysql.h>
#include <regex>
#include <ctime>


using namespace std;
class Estudiante : Persona {
	//atributos
private: string codigo;
	   int id_estudiante = 0;
	   //constructor
public:
	Estudiante() {}
	Estudiante(string nom, string ape, string dir, int tel, string fn, int id_ts, string cod, int id_e) : Persona(nom, ape, dir, tel, fn, id_ts) {
		codigo = cod;
		id_estudiante = id_e;
	}
	// metodos get y set
	// set (modificar un atributo)
	void setId_estudiante(int e) { id_estudiante = e; }
	void setCodigo(string c) { codigo = c; }
	void setNombres(string n) { nombres = n; }
	void setApellidos(string a) { apellidos = a; }
	void setDireccion(string d) { direccion = d; }
	void setTelefono(int t) { telefono = t; }
	void setFecha_Nacimiento(string f) { fecha_nacimiento = f; }
	void setId_Tipo_Sangre(int ts) { id_tipo_sangre = ts; }

	// get (mostrar el contenido del atributo)

	int getId_estudiante() { return id_estudiante; }
	string getNombres() { return nombres; }
	string getApellidos() { return apellidos; }
	string getDireccion() { return direccion; }
	int getTelefono() { return telefono; }
	string getFecha_nacimiento() { return fecha_nacimiento; }
	int getId_tipo_sangre() { return id_tipo_sangre; }

	// metodos
	bool validarNombres() {
		if (nombres.empty() || nombres.length() > 60) {
			cout << "Error: Los nombres son obligatorios y deben tener un maximo de 60 caracteres." << endl;
			return false;
		}
		std::regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ\\s]+$");

		if (!std::regex_match(nombres, patron)) {
			cout << "Error: Los nombres solo pueden contener letras y espacios (sin numeros ni simbolos)." << endl;
			return false;
		}

		return true;
	}
	bool validarApellidos() {
		if (apellidos.empty() || apellidos.length() > 60) {
			cout << "Error: Los apellidos son obligatorios y maximo de 60 caracteres." << endl;
			return false;
		}
		std::regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ\\s]+$");
		if (!std::regex_match(apellidos, patron)) {
			cout << "Error: Los apellidos solo pueden contener letras y espacios." << endl;
			return false;
		}
		return true;
	}

	bool validarFormatoCodigo() {
		if (codigo.empty() || codigo.length() > 4) {
			cout << "Error: El codigo es obligatorio y debe tener maximo 4 caracteres." << endl;
			return false;
		}
		std::regex patron("^E[0-9]{3}$");
		if (!std::regex_match(codigo, patron)) {
			cout << "Error: El codigo debe tener el formato E001, E002, etc." << endl;
			return false;
		}
		return true;
	}
	bool validarDireccion() {
		if (direccion.empty() || direccion.length() > 100) {
			cout << "Error: La direccion es obligatoria y debe tener un maximo de 100 caracteres." << endl;
			return false;
		}
	
	std::regex patron("^[A-Za-z0-9ÁÉÍÓÚáéíóúÑñ#\\-.,\\s]+$");

	if (!std::regex_match(direccion, patron)) {
		cout << "Error: La direccion contiene caracteres no permitidos. Solo se aceptan letras, numeros, espacios y los simbolos # - . ," << endl;
		return false;
	}
	return true;
}
	bool validarTelefono() {
		string tel_str = to_string(telefono);
		std::regex patron("^[0-9]{8}$");
		if (!std::regex_match(tel_str, patron)) {
			cout << "Error: El telefono debe tener 8 digitos." << endl;
			return false;
		}
		return true;
	}
	bool validarFechaNacimiento() {
		std::regex patron("^\\d{4}-\\d{2}-\\d{2}$");
		if (!std::regex_match(fecha_nacimiento, patron)) {
			cout << "Error: La fecha de nacimiento es obligatoria y debe tener el formato AAAA-MM-DD." << endl;
			return false;
		}
		int year = stoi(fecha_nacimiento.substr(0, 4));
		int month = stoi(fecha_nacimiento.substr(5, 2));
		int day = stoi(fecha_nacimiento.substr(8, 2));
		if (month < 1 || month > 12) {
			cout << "Error: El mes ingresado no es valido (debe ser entre 01 y 12)." << endl;
			return false;
		}
		bool esBisiesto = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
		int diasPorMes[] = { 0, 31, (esBisiesto ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		if (day < 1 || day > diasPorMes[month]) {
			cout << "Error: El dia ingresado no es valido para ese mes/año." << endl;
			return false;
		}

		time_t t = time(nullptr);
		tm now;
		localtime_s(&now, &t);
		int anioActual = now.tm_year + 1900;
		int mesActual = now.tm_mon + 1;
		int diaActual = now.tm_mday;

		if (year > anioActual ||
			(year == anioActual && month > mesActual) ||
			(year == anioActual && month == mesActual && day > diaActual)) {
			cout << "Error: La fecha de nacimiento no puede ser una fecha futura." << endl;
			return false;
		}
		return true;
	}
	bool existeCodigoDuplicado() {
		bool existe = false;
		ConexionBD cn = ConexionBD();
		cn.abrirConexion();
		if (cn.getConexion()) {
			string consulta = "SELECT COUNT(*) FROM estudiantes WHERE codigo = '" + codigo + "'";
			const char* c = consulta.c_str();
			int q_estado = mysql_query(cn.getConexion(), c);
			if (!q_estado) {
				MYSQL_RES* resultado = mysql_store_result(cn.getConexion());
				if (resultado) {
					MYSQL_ROW fila = mysql_fetch_row(resultado);
					if (fila && atoi(fila[0]) > 0) {
						existe = true;
						cout << "Error: El codigo ya existe en la base de datos." << endl;
					}
				}
				mysql_free_result(resultado);
			}
			else {
				cout << "Error al ejecutar la consulta: " << mysql_error(cn.getConexion()) << endl;
			}
		}
		else {
			cout << "Error al conectar a la base de datos." << endl;
		}
		return existe;
	}	
	bool validarTipoSangre() {
		if (id_tipo_sangre <= 0) {
			cout << "Error: El tipo de sangre es obligatorio y el ID debe ser mayor a 0." << endl;
			return false;
		}
		bool existe = false;
		ConexionBD cn = ConexionBD();
		cn.abrirConexion();

		if (cn.getConexion()) {
			string consulta = "SELECT id_tipo_sangre FROM tipos_sangre WHERE id_tipo_sangre = " + to_string(id_tipo_sangre);
			if (!mysql_query(cn.getConexion(), consulta.c_str())) {
				MYSQL_RES* resultado = mysql_store_result(cn.getConexion());
				if (resultado != nullptr && mysql_num_rows(resultado) > 0) {
					existe = true;
				}
				else {
					cout << "Error: El tipo de sangre con ID '" << id_tipo_sangre << "' no existe en la base de datos." << endl;
				}

				if (resultado != nullptr) {
					mysql_free_result(resultado);
				}
			}
			else {
				cout << "Error al consultar la base de datos: " << mysql_error(cn.getConexion()) << endl;
			}
		}
		else {
			cout << " xxxx Error de conexion al validar el tipo de sangre xxxx " << endl;
		}
		cn.cerrarConexion();

		return existe;
	}
		void crear() {
		if (!validarFormatoCodigo()) {
			return;
		}
		if (existeCodigoDuplicado()) {
			return;
		}
		if (!validarNombres()) { 
			return;
		}
		if (!validarApellidos()) {
			return;
		}
		if (!validarDireccion()) {
			return;
		}
		if (!validarTelefono()) {
			return;
		}
		if (!validarFechaNacimiento()) {
			return;
		}
		int q_estado = 0;
		ConexionBD cn = ConexionBD();
		cn.abrirConexion();
		if (cn.getConexion()) {

			string t = to_string(telefono);
			string id_ts = to_string(id_tipo_sangre);
			string consulta = "INSERT INTO estudiantes(codigo,nombres,apellidos,direccion,telefono,fecha_nacimiento,id_tipo_sangre) VALUES  ('" + codigo + "','" + nombres + "','" + apellidos + "','" + direccion + "'," + t + ",'" + fecha_nacimiento + "'," + id_ts + "); ";
			const char* c = consulta.c_str();
			q_estado = mysql_query(cn.getConexion(), c);
			if (!q_estado) {
				id_estudiante = mysql_insert_id(cn.getConexion());
				cout << "Ingreso de Datos Exitoso...." << endl;
				cout << ">> El ID asignado automaticamente al estudiante es: " << id_estudiante << endl;
			}
			else {
				cout << "xxxxx Consulta  Fallida xxxxx" << endl;
				cout << "Error: " << mysql_error(cn.getConexion()) << endl;
			}

		}
		else {
			cout << " xxxx Conexion Falllida xxxx " << endl;
		}
		cn.cerrarConexion();
	}
	void leer() {
		int q_estado = 0;
		ConexionBD cn = ConexionBD();
		MYSQL_ROW fila;
		MYSQL_RES* resultado;
		cn.abrirConexion();
		if (cn.getConexion()) {
			cout << "___________ Datos del Estudiante ___________" << endl;
			string consulta = "select e.id_estudiante as id,e.codigo,e.nombres,e.apellidos,e.direccion,e.telefono,e.fecha_nacimiento,ts.sangre from estudiantes as e inner join tipos_sangre as ts on e.id_tipo_sangre = ts.id_tipo_sangre;";
			const char* c = consulta.c_str();
			q_estado = mysql_query(cn.getConexion(), c);
			if (!q_estado) {
				resultado = mysql_store_result(cn.getConexion());
				while (fila = mysql_fetch_row(resultado)) {
					cout << fila[0] << "," << fila[1] << "," << fila[2] << "," << fila[3] << "," << fila[4] << "," << fila[5] << "," << fila[6] << "," << fila[7] << endl;
				}
			}
			else {
				cout << " xxx Consulta Fallida xxx" << endl;
			}

		}
		else {
			cout << " xxx Fallo la Conexion xxx" << endl;
		}
		cn.cerrarConexion();
	}
	void actualizar() {
		if (!validarFormatoCodigo()) {
			return;
		}
		if (existeCodigoDuplicado()) {
			return;
		}
		if (!validarNombres()) {
			return;
		}
		if (!validarApellidos()) {
			return;
		}
		if (!validarDireccion()) {
			return;
		}
		if (!validarTelefono()) {
			return;
		}
		if (!validarFechaNacimiento()) {
			return;
		}
		int q_estado = 0;
		ConexionBD cn = ConexionBD();
		cn.abrirConexion();
		if (cn.getConexion()) {

			string t = to_string(telefono);
			string id_ts = to_string(id_tipo_sangre);
			string id_e = to_string(id_estudiante);
			string consulta = "update estudiantes set codigo = '" + codigo + "',nombres = '" + nombres + "',apellidos='" + apellidos + "',direccion='" + direccion + "',telefono=" + t + ",fecha_nacimiento = '" + fecha_nacimiento + "',id_tipo_sangre=" + id_ts + " where id_estudiante = " + id_e + "";
			const char* c = consulta.c_str();
			q_estado = mysql_query(cn.getConexion(), c);
			if (!q_estado) {
				cout << "Modificacion de Datos Exitoso...." << endl;
			}
			else {
				cout << "xxxxx Consulta  Fallida xxxxx" << endl;
			}

		}
		else {
			cout << " xxxx Conexion Falllida xxxx " << endl;
		}
		cn.cerrarConexion();
	}
	void borrar() {

		int q_estado = 0;
		ConexionBD cn = ConexionBD();
		cn.abrirConexion();
		if (cn.getConexion()) {

			string t = to_string(telefono);
			string id_ts = to_string(id_tipo_sangre);
			string id_e = to_string(id_estudiante);
			string consulta = "delete from estudiantes  where id_estudiante = " + id_e + "";
			const char* c = consulta.c_str();
			q_estado = mysql_query(cn.getConexion(), c);
			if (!q_estado) {
				cout << "Eliminacion de Datos Exitoso...." << endl;
			}
			else {
				cout << "xxxxx Consulta  Fallida xxxxx" << endl;
			}

		}
		else {
			cout << " xxxx Conexion Falllida xxxx " << endl;
		}
		cn.cerrarConexion();
	}
};