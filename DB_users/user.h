#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <functional>

std::vector<std::string> firstNames = {"Иван", "Петр", "Максим", "Гоша", "Александр", "Евгений"};
std::vector<std::string> middleNames = {"Иванович", "Петрович", "Александров"};
std::vector<std::string> lastNames = {"Иванов", "Петров", "Сидоров", "Козлов", "Морозов"};
// std::vector<std::string> firstNames = {"Иван", "Иван", "Иван", "Иван", "Иван", "Иван"};
// std::vector<std::string> middleNames = {"Иванович", "Иванович", "Иванович"};
// std::vector<std::string> lastNames = {"Иванов", "Иванов", "Иванов", "Иванов", "Иванов"};

class User {
private:
    std::string fullName;
    std::string firstName;
    std::string secondName;
    std::string lastName;
    int age;
    std::string login;
    std::string password;
    bool isActive;

    static const std::map<char, std::string> translitTable;

public:
    User() = default;

    User(const std::string& fullName, int age) {
        this->fullName = fullName;
        this->age = age;
        this->isActive = true;
        generateLogin();
        generatePassword();
    }

    void generateLogin() {
        size_t spaceIndex = fullName.find(' ');
        size_t lastNameIndex = fullName.rfind(' ');
        std::cout << fullName << std::endl;
        if (spaceIndex != std::string::npos && lastNameIndex != std::string::npos) {
            login = "";
            int current_index = 0;
            for (char c : firstName)
            {
                auto it = translitTable.find(c);
                if(current_index == 0)
                {
                    if (it != translitTable.end()) 
                    {
                        login += it->second;
                        current_index++;
                    }
                    else 
                    {
                        login += c;
                    }
                }
            }
            login += ".";
            current_index = 0;
            for (char c : secondName)
            {
                if(current_index == 0)
                {
                    auto it = translitTable.find(c);
                    if (it != translitTable.end()) 
                    {
                        login += it->second;
                            current_index++;
                    }
                    else 
                    {
                        login += c;
                    }
                }
            }
            login += ".";
            for (char c : lastName) {
                auto it = translitTable.find(c);
                if (it != translitTable.end()) 
                {
                    login += it->second;
                }
                else 
                {
                    login += c;
                }
            }
        } 
        else 
        {
             std::cout << "Ошибка получения данных, для авто-генерации логина!" << std::endl;
            login = fullName;
        }
    }

    void generatePassword() {
        const std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const int passwordLength = 8;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, allowedChars.length() - 1);

        password.clear();
        for (int i = 0; i < passwordLength; ++i) {
            password += allowedChars[dist(gen)];
        }
    }

    const std::string& getFullName() const {
        return fullName;
    }
    const std::string& getLastName() const {
        return lastName;
    }
    const std::string& getFirstName() const {
        return firstName;
    }
    const std::string& getSecondName() const {
        return secondName;
    }

    int getAge() const {
        return age;
    }

    bool getIsActive() const {
        return isActive;
    }

    const std::string& getLogin() const {
        return login;
    }
    
    const std::string& getPassword() const {
        return password;
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        os << "ФИО: " << user.fullName << ", Возраст: " << user.age << ", Логин: " << user.login
           << ", Пароль: " << user.password << ", Активность: " << (user.isActive ? "Активен" : "Неактивен");
        return os;
    }

    void generateRandomFullName() {
        // Вырвал векторы ФИО отсюда

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> firstNameDist(0, firstNames.size() - 1);
        std::uniform_int_distribution<> middleNameDist(0, middleNames.size() - 1);
        std::uniform_int_distribution<> lastNameDist(0, lastNames.size() - 1);
        
        firstName = firstNames[firstNameDist(gen)];
        secondName = middleNames[middleNameDist(gen)];
        lastName = lastNames[lastNameDist(gen)];
        
        fullName = firstName + " " + secondName + " " + lastName;
    }

    static User generateRandomUser() {
        User user;
        user.generateRandomFullName();
        user.age = generateRandomAge();
        user.isActive = generateRandomActivity();
        user.generateLogin();
        user.generatePassword();
        return user;
    }

private:

    static int generateRandomAge() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> ageDist(18, 60);
        return ageDist(gen);
    }

    static bool generateRandomActivity() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> activityDist(0, 1);
        return activityDist(gen) == 1;
    }
};

const std::map<char, std::string> User::translitTable = {
    {'А', "A"}, {'Б', "B"}, {'В', "V"}, {'Г', "G"}, {'Д', "D"},
    {'Е', "E"}, {'Ё', "YO"}, {'Ж', "ZH"}, {'З', "Z"}, {'И', "I"},
    {'Й', "Y"}, {'К', "K"}, {'Л', "L"}, {'М', "M"}, {'Н', "N"},
    {'О', "O"}, {'П', "P"}, {'Р', "R"}, {'С', "S"}, {'Т', "T"},
    {'У', "U"}, {'Ф', "F"}, {'Х', "KH"}, {'Ц', "TS"}, {'Ч', "CH"},
    {'Ш', "SH"}, {'Щ', "SCH"}, {'Ъ', ""}, {'Ы', "Y"}, {'Ь', ""},
    {'Э', "E"}, {'Ю', "YU"}, {'Я', "YA"},
    {'а', "a"}, {'б', "b"}, {'в', "v"}, {'г', "g"}, {'д', "d"},
    {'е', "e"}, {'ё', "yo"}, {'ж', "zh"}, {'з', "z"}, {'и', "i"},
    {'й', "y"}, {'к', "k"}, {'л', "l"}, {'м', "m"}, {'н', "n"},
    {'о', "o"}, {'п', "p"}, {'р', "r"}, {'с', "s"}, {'т', "t"},
    {'у', "u"}, {'ф', "f"}, {'х', "kh"}, {'ц', "ts"}, {'ч', "ch"},
    {'ш', "sh"}, {'щ', "sch"}, {'ъ', ""}, {'ы', "y"}, {'ь', ""},
    {'э', "e"}, {'ю', "yu"}, {'я', "ya"}
};
