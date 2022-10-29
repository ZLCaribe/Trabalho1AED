#include "Gestor.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCturma.h"

using namespace std;
const vector<string> explode(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}

void addEstudante(){
    string linha;
    ifstream in("classes_per_uc.csv");

    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        UCTurma ucTurma(v[2], v[3]);


    }
}

void addEstudante(){
    string linha;
    ifstream in("students_classes.csv");

    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        Estudante estudante(v[0], v[1]);



    }
}


