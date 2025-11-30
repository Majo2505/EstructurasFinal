#pragma once
#include <iostream>
#include <string>
using namespace std;

template <class T>
class Arista
{
private:
	T origen;
	T destino;
public:
	Arista() {}
	Arista(const T& o, const T& d) : origen(o), destino(d) {}

	// Getters
	T getOrigen() const { return origen; }
	T getDestino() const { return destino; }

	// Setters 
	void setOrigen(const T& o) { origen = o; }
	void setDestino(const T& d) { destino = d; }
};


///version antigua
/*

template <class T>
class Arista
{
private:
	T origen;
	T destino;
public:
	//Arista(T o, T d) : origen(o), destino(d) {}
	T getOrigen() const { return origen; }
	T getDestino() const { return destino; }
	void setOrigen(T o) { origen = o; }
	void setDestino(T d) { destino = d; }
};
*/