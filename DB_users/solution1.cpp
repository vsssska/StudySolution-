#include "user.h"

/* Функция подсчета статистики
@param<User> users вектор с пользователями
return возвращает пару активные:неактивные
*/
std::pair<int, int> countActiveInactiveUsers(const std::vector<User>& users) {
    int activeCount = std::count_if(users.begin(), users.end(), [](const User& user) {
        return user.getIsActive();
    });

    int inactiveCount = users.size() - activeCount;
    return std::make_pair(activeCount, inactiveCount);
}

/* Функция подсчета однофамильцев
@param<User> users вектор с пользователями
@param lastName искомая фамилия
*/
int countSameLastName(const std::vector<User>& users, const std::string& lastName) {
    return std::count_if(users.begin(), users.end(), [lastName](const User& user) {
        return user.getLastName() == lastName;
    });
}

/* Функция сравнения для сортировки по полю "ФИО"
@param users1 пользователь для сравнения
@param user2 пользователь для сравнения
*/
bool compareByFullName(const User& user1, const User& user2) {
    return user1.getFullName() < user2.getFullName();
}

/* Функция сравнения для сортировки по полю "Возраст"
@param users1 пользователь для сравнения
@param user2 пользователь для сравнения
*/
bool compareByAge(const User& user1, const User& user2) {
    return user1.getAge() < user2.getAge();
}

/* Функция сравнения для сортировки по полю "Логин"
@param users1 пользователь для сравнения
@param user2 пользователь для сравнения
*/
bool compareByLogin(const User& user1, const User& user2) {
    return user1.getLogin() < user2.getLogin();
}

/* Функция для сортировки по выбранным полям
@param<User> users вектор пользователей
@param<function> вектор условий сравнений
*/
void sortUsers(std::vector<User>& users, std::vector<std::function<bool(const User&, const User&)>> comparators) {
    for (const auto& comparator : comparators) {
        std::sort(users.begin(), users.end(), comparator);
    }
}

/* Функция для создания среза
@param<User> users вектор пользователей
*/
void srez(const std::vector<User>& users)
{
    int ageThreshold;
    std::cout << "Введите возраст для формирования среза: ";
    std::cin >> ageThreshold;

    std::vector<User> usersAboveThreshold;
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersAboveThreshold),
                 [ageThreshold](const User& user) { return user.getAge() > ageThreshold; });

    std::cout << "\nСрез пользователей старше " << ageThreshold << " лет:\n";
    for (const auto& user : usersAboveThreshold) {
        std::cout << user << std::endl;
        std::cout << "-----------------------------------\n";
    }
}

/* Подсчет статистики активности
@param<User> users вектор с пользователями
*/
void activnost(const std::vector<User>& users)
{
    std::cout << "-----------------------------------\n";
    auto [activeCount, inactiveCount] = countActiveInactiveUsers(users);
    std::cout << "Активных пользователей: " << activeCount << std::endl;
    std::cout << "Неактивных пользователей: " << inactiveCount << std::endl;
}

/* Подсчет статистики совпадений
@param<User> users вектор с пользователями
*/
void sovpadeniya(const std::vector<User>& users)
{
    // Подсчет статистики совпадений
    std::cout << "-----------------------------------\n";
    std::cout << "Количество однофамильцев: " << std::endl;
    for (auto name : lastNames)
    {
        int sameLastNameCount = countSameLastName(users, name);
        if(sameLastNameCount == 1)
            sameLastNameCount = 0;
        std::cout << name << " : " << sameLastNameCount << std::endl;
    }

    std::cout << "-----------------------------------\n";
    std::cout << "Количество повторений полных ФИО: " << std::endl;

    std::map<std::string, int> namesakes;
    for(auto user : users)
    {
        namesakes[user.getFullName()] = std::count_if(users.begin(), users.end(), [&user](User another_user) {
                int count = 0;
                if (another_user.getFullName() == user.getFullName()) count++;
                return count;
            });
    }
    for (auto person_name : namesakes)
            std::cout << "Имя: " << person_name.first << "\nКоличество тёзок: " << person_name.second << "\n" << std::endl;
    
}

/*Функция сортировки данных
@param<User> users вектор с пользователями
*/
void sort(std::vector<User>& users)
{
    int user_sort_choice = 0;
    std::cout << "Введите поля по которым хотите сделать сортировку:" << std::endl;
    std::cout << "1. ФИО\n2. Возраст\n3. Логин" << std::endl;
    while(true)
    {
        std::cin >> user_sort_choice;
        if(user_sort_choice == 1)
        {
            sortUsers(users, {compareByFullName});
            break;
        }
        else if(user_sort_choice == 2)
        {
            sortUsers(users, {compareByAge});
            break;
        }
        else if(user_sort_choice == 3)
        {
            sortUsers(users, {compareByLogin});
            break;
        }
        else
        {
            std::cout << "Введено не верное значение" << std::endl;
        }
    }

    // Выводим отсортированные объекты
    for (const auto& user : users) {
        std::cout << "ФИО: " << user.getFullName() << ", Возраст: " << user.getAge()
                   << ", Логин: " << user.getLogin() << ", Пароль: " << user.getPassword()
                   << ", Активность: " << (user.getIsActive() ? "Активен" : "Неактивен") << std::endl;
    }
}

/*Функция поиска пользователя
@param<User> users вектор с пользователями
@param fullName искомое имя пользователя
*/
std::vector<User>::const_iterator findUserByFullName(const std::vector<User>& users, const std::string& fullName) {
    return std::find_if(users.begin(), users.end(), [&fullName](const User& user) {
        std::cout << user.getFullName() << " : " << fullName << std::endl;        
        return user.getFullName() == fullName;
    });
}

/* Работа с пользователем по поиску данных
@param<User> вектор с пользователями
*/
void findUser(std::vector<User>& users){
    std::cout << "Введите ФИО для поиска: ";
    std::string searchName;
    std::cout << std::endl;
    std::getline(std::cin >> std::ws, searchName);

    auto it = findUserByFullName(users, searchName);

    if (it != users.end()) {
        std::cout << "Пользователь найден. ФИО: " << it->getFullName() << std::endl;
    } else {
        std::cout << "Пользователь не найден." << std::endl;
    }
}


int main() {
    const int N = 15;
    std::vector<User> users(N);
    std::generate_n(users.begin(), N, User::generateRandomUser);

    for (const auto& user : users) {
        std::cout << user << std::endl;
        std::cout << "-----------------------------------\n";
    }

    srez(users);
    activnost(users);
    sovpadeniya(users);
    sort(users);
    findUser(users);

    return 0;
}
