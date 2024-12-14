#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>

class ChineseWall {
private:
    size_t subjects;
    size_t objects;
    size_t firms;
    std::vector<std::vector<bool>> accessMatrix;  // Доступы субъектов к объектам
    std::vector<int> objectOwners;  // Владелец каждого объекта
    std::vector<std::set<int>> conflictClasses;  // Конфликтующие фирмы

public:
    ChineseWall(size_t n, size_t m, size_t f) : subjects(n), objects(m), firms(f) {
        accessMatrix = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false)); // Изначально нет доступа
        objectOwners.resize(m);  // Список владельцев объектов
        conflictClasses.resize(f); // Список конфликтных классов
    }

    // Очистить все истории доступа
    void start() {
        for (auto& row : accessMatrix) {
            std::fill(row.begin(), row.end(), false);
        }
    }

    // Проверка прав на чтение
    bool read(size_t subject, size_t object) {
        if (subject >= subjects || object >= objects) {
            std::cerr << "Error: invalid subject or object index." << std::endl;
            return false;
        }

        int objectOwner = objectOwners[object];

        // Субъект должен либо не иметь доступа к объектам в этом классе конфликта,
        // либо иметь доступ к объектам той же фирмы.
        for (size_t i = 0; i < subjects; ++i) {
            if (accessMatrix[i][object] && conflictClasses[objectOwner].count(i)) {
                return false;
            }
        }

        // Разрешение на чтение объекта
        accessMatrix[subject][object] = true;
        return true;
    }

    // Проверка прав на запись
    bool write(size_t subject, size_t object) {
        if (subject >= subjects || object >= objects) {
            std::cerr << "Error: invalid subject or object index." << std::endl;
            return false;
        }

        if (!read(subject, object)) return false;

        int objectOwner = objectOwners[object];

        // Проверка, не читал ли субъект объекты другой фирмы в том же конфликтующем классе
        for (size_t i = 0; i < objects; ++i) {
            if (accessMatrix[subject][i] && objectOwners[i] != objectOwner &&
                conflictClasses[objectOwner].count(objectOwners[i])) {
                return false;
            }
        }

        // Разрешение на запись в объект
        accessMatrix[subject][object] = true;
        return true;
    }

    // Установить владельца объекта
    void setObjectOwner(size_t object, int firm) {
        if (object >= objects || firm >= firms) {
            std::cerr << "Error: invalid object or firm index." << std::endl;
            return;
        }
        objectOwners[object] = firm;
    }

    // Установить конфликтующие фирмы
    void addConflictClass(int firm1, int firm2) {
        if (firm1 >= firms || firm2 >= firms) {
            std::cerr << "Error: invalid firm index." << std::endl;
            return;
        }
        conflictClasses[firm1].insert(firm2);
        conflictClasses[firm2].insert(firm1);
    }

    // Печать отчета по субъекту
    void reportSubject(size_t subject) {
        if (subject >= subjects) {
            std::cerr << "Error: invalid subject index." << std::endl;
            return;
        }
        std::cout << "Subject " << subject << " accessed: ";
        bool first = true;
        for (size_t i = 0; i < objects; ++i) {
            if (accessMatrix[subject][i]) {
                if (!first) std::cout << ", ";
                std::cout << "Object " << i << " (Firm " << objectOwners[i] << ")";
                first = false;
            }
        }
        std::cout << std::endl;
    }

    // Печать отчета по объекту
    void reportObject(size_t object) {
        if (object >= objects) {
            std::cerr << "Error: invalid object index." << std::endl;
            return;
        }
        std::cout << "Object " << object << " was accessed by: ";
        bool first = true;
        for (size_t i = 0; i < subjects; ++i) {
            if (accessMatrix[i][object]) {
                if (!first) std::cout << ", ";
                std::cout << "Subject " << i;
                first = false;
            }
        }
        std::cout << std::endl;
    }

    // Печать портфеля фирмы
    void briefCase(int firm) {
        if (firm >= firms) {
            std::cerr << "Error: invalid firm index." << std::endl;
            return;
        }
        std::cout << "Firm " << firm << " portfolio: ";
        bool first = true;
        for (size_t i = 0; i < objects; ++i) {
            if (objectOwners[i] == firm) {
                if (!first) std::cout << ", ";
                std::cout << "Object " << i;
                first = false;
            }
        }
        std::cout << std::endl;
    }

    size_t getSubjects() const {
        return subjects;
    }

    size_t getObjects() const {
        return objects;
    }

    size_t getFirms() const {
        return firms;
    }
};

int main() {
    size_t n, m, f;

    std::cout << "Enter the number of subjects: ";
    std::cin >> n;
    std::cout << "Enter the number of objects: ";
    std::cin >> m;
    std::cout << "Enter the number of firms: ";
    std::cin >> f;

    ChineseWall wall(n, m, f);

    // Устанавливаем владельцев объектов
    for (size_t i = 0; i < m; ++i) {
        int firm;
        std::cout << "Enter the owner of object " << i << ": ";
        std::cin >> firm;
        wall.setObjectOwner(i, firm);
    }

    // Устанавливаем конфликтующие фирмы
    size_t conflictCount;
    std::cout << "Enter the number of conflict pairs: ";
    std::cin >> conflictCount;
    for (size_t i = 0; i < conflictCount; ++i) {
        int firm1, firm2;
        std::cout << "Enter conflict pair (firm1 firm2): ";
        std::cin >> firm1 >> firm2;
        wall.addConflictClass(firm1, firm2);
    }

    // Запуск команд
    while (true) {
        std::string command;
        std::cout << "Enter command (start, read, write, report, brief_case, exit): ";
        std::cin >> command;

        if (command == "start") {
            wall.start();
            std::cout << "Access histories cleared." << std::endl;
        }
        else if (command == "read") {
            size_t s, o;
            std::cin >> s >> o;
            if (wall.read(s, o)) {
                std::cout << "accepted" << std::endl;
            }
            else {
                std::cout << "refused" << std::endl;
            }
        }
        else if (command == "write") {
            size_t s, o;
            std::cin >> s >> o;
            if (wall.write(s, o)) {
                std::cout << "accepted" << std::endl;
            }
            else {
                std::cout << "refused" << std::endl;
            }
        }
        else if (command == "report") {
            std::string flag;
            std::cin >> flag;
            if (flag == "-s") {
                size_t s;
                std::cin >> s;
                wall.reportSubject(s);
            }
            else if (flag == "-o") {
                size_t o;
                std::cin >> o;
                wall.reportObject(o);
            }
        }
        else if (command == "brief_case") {
            int f;
            std::cin >> f;
            wall.briefCase(f);
        }
        else if (command == "exit") {
            break;
        }
    }

    return 0;
}
