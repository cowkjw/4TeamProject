#pragma once
#include "Include.h"

template <typename T>
class CJsonManager
{
public:
    static bool Save_File(const T& data, const string& fileName)
    {
        filesystem::path dir("../Data");
        if (!filesystem::exists(dir))
            filesystem::create_directories(dir);

        string fullFileName = fileName + ".json";
        ofstream file("../Data/" + fullFileName);

        if (!file) return false;

        json j = data;
        file << j.dump(4);  // Pretty-print with 4 spaces indentation
        file.close();
        return true;
    }

    static bool Load_File(T& data, const string& fileName) 
    {
        filesystem::path dir("../Data");
        if (!filesystem::exists(dir))
            filesystem::create_directories(dir);

        ifstream file("../Data/" + fileName+".json");
        if (!file) return false;

        json j;
        file >> j;
        data = j.get<T>();
        return true;
    }
};

