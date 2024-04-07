#include "warrior.h"
#include <chrono>
#include <ctime>
#include <memory>
#include <array>
#include <algorithm>
#include <string> 
#include <cmath>

const int N = 10; // Размер поля N x N
const int M = 10; // Кол-во бойцов в одной комманде

/*Ф-ция ожидания
*@param seconds секунд ожидания
*/
void sleep(float seconds){
    clock_t startClock = clock();
    float secondsAhead = seconds * CLOCKS_PER_SEC;
    // do nothing until the elapsed time has passed.
    while(clock() < startClock+secondsAhead);
    return;
}

/* Функция пользовательского ввода данных
*@param limit_left передача минимально допустимого числа
*@param limit_right передача максимально допустимого числа
*return x возвращает, полученное от пользователя число
*/
int user_input(int limit_left, int limit_right){
    int x = 0; // переменная для ввода числовых данных
    bool good = true;
    do
    {
        good = true;
        cin >> x;
        if (!(good = cin.good()) || x < limit_left || x > limit_right){
            cout << "\nIncrorrect number" << endl;
            good = false;
        }
            
        cin.clear();
        cin.ignore();
    } while (!good);
    return x;
}

/*Ф-ция заполнения группы бойцов
*@param &melees рота ближних бойцов
*@param &wizards рота магических бойцов
*@param &ranges рота дистанционных бойцов
*@param &team флаг команды
*/
void fill_team(vector<shared_ptr<Warrior>> &warriors, Team team)
{
    warriors.clear();
    int melee_count = M/2;
    int range_count = M/5 + M/10;
    int wizard_count = M - (melee_count + range_count);

    for(int i = 0; i < melee_count; i++)
    {
        Melee melee_temp(team);
        warriors.push_back(make_unique<Melee>(melee_temp));
    }
    for(int i = 0; i < range_count; i++)
    {
        Range range_temp(team);
        warriors.push_back(make_unique<Range>(range_temp));
    }
    for(int i = 0; i < wizard_count; i++)
    {
        Wizard wizard_temp(team);
        warriors.push_back(make_unique<Wizard>(wizard_temp));
    }
}

/*Ф-ция вывода статистики о воине
*@param &warrior ссылка на воина
*@param index номер воина
*/
void print_warrior_stats(Warrior &warrior, int index)
{
    cout << "Warrior #" << index << endl
    <<"\tHP= " << warrior.hp << endl
    <<"\tArmor= " << warrior.armor << endl
    <<"\tBonus damage= " << warrior.bonus_damage << endl
    <<"\tWarrior type: " << static_cast<char>(warrior.warrior_class) << endl
    <<"\tTeam: " << static_cast<char>(warrior.team) << endl;
}

/* Бой 1 на 1 
*@param &warrior1 ссылка на воина номер 1
*@param &warrior2 ссылка на воина номер 2
*/
void round(Warrior &warrior1, Warrior &warrior2)
{
    int winner;
    cout << "=============== Бой 1 на 1 ===============" << endl;
    print_warrior_stats(warrior1, 1);
    print_warrior_stats(warrior2, 2);

    while(warrior1.hp > 0 && warrior2.hp > 0)
    {
        // Урон первого воина
        int damage1 = warrior1.attack() - warrior2.armor;
        if(damage1 <= 0){
            damage1 = 1;
        }
        warrior2.hp -= damage1;

        // Урон первого воина
        int damage2 = warrior2.attack() - warrior1.armor;
        if(damage2 <= 0){
            damage2 = 1;
        }
        if(warrior2.hp > 0)
            warrior1.hp -= damage2;

        print_warrior_stats(warrior1, 1);
        print_warrior_stats(warrior2, 2);
        
        winner = (warrior1.hp <= 0) ? 2 : ((warrior2.hp <= 0) ? 1 : 0);
        sleep(1);

    }

    cout << "WINNER IS #" << winner << endl;
}

/* Ф-ция вывода статистики о команде
*@param &warriors команда воинов
*@param team название команды
*/
void print_team_stat(vector<shared_ptr<Warrior>> &warriors, Team team)
{
    string team_name = (team == Team::Blue) ? "Blue" : "Red";
    cout << "=== "<< team_name << " Team ===" << endl << endl;

    int melee_counter=0, range_counter=0, wizard_counter=0;

    for(int i = 0; i < warriors.size(); i++)
    {
        print_warrior_stats(*warriors[i], i);

        if(warriors[i]->warrior_class == Warriors_class::Melee)
            melee_counter++;

        if(warriors[i]->warrior_class == Warriors_class::Range)
            range_counter++;

        if(warriors[i]->warrior_class == Warriors_class::Wizard)
            wizard_counter++;
    }

    cout << "Melee warriors: " << melee_counter << endl
    << "Range warriors: " << range_counter << endl
    << "Wizard warriors: " << wizard_counter << endl;
    
}

/*Ф-ция поиска дистанции
*@param x1 координата х атакующего
*@param y1 координата у атакующего
*@param x2 координата у атакуемого
*@param y2 координата у атакуемого
*/
double distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

/*Ф-ция поиска самого близкого противника 
*@param x координата "х" атакующего
*@param y координата "у" атакующего
*@param &plate поле боя
*@param &enemy_team вражеская команда
*/
int find_enemy(int x, int y, Warrior *** &plate, vector<shared_ptr<Warrior>> &enemy_team)
{
    int current_pos_x = x;
    int current_pos_y = y;


    double minDist = distance(current_pos_x, current_pos_y, enemy_team[0]->x, enemy_team[0]->y);
    int closest_x = enemy_team[0]->x;
    int closest_y = enemy_team[0]->y;
    int closest_enemy = 0;

    for(int i=0; i<enemy_team.size(); i++)
    {
        double dist = distance(current_pos_x, current_pos_y, enemy_team[i]->x, enemy_team[i]->y);
        if(dist < minDist)
        {
            minDist = dist;
            closest_enemy = i;
        }
    }

    return closest_enemy;
}

/*Ф-ция размещения команды 
*@param _team команда бойцов
*@param &plate поле боя
*/
void place_team(vector<shared_ptr<Warrior>> &_team, Warrior *** &plate)
{
    int x, y;
    for(int i=0; i < M; i++)
    {
        bool placed = false;
        while(!placed)
        {
            x = randomInt(0, (N-1));
            y = randomInt(0, (N-1));

            if(plate[x][y] == NULL)
            {
                _team[i]->x = x;
                _team[i]->y = y;
                plate[x][y] = _team[i].get();
                placed = true;
            }
        }
    }
}

/*Ф-ция размещения команды 
*@param &blue_team команда бойцов синего флага
*@param &red_team команда бойцов красного флага
*@param &plate поле боя
*/
void team_fight(vector<shared_ptr<Warrior>> &blue_team, vector<shared_ptr<Warrior>> &red_team, Warrior *** &plate)
{
    place_team(blue_team, plate);
    place_team(red_team, plate);

    // Задаем начальную инициативу бойцов каждой команды
    for(int i = 0; i < M; i++)
    {
        blue_team[i]->bonus_initiative = randomInt(1, 20);
        red_team[i]->bonus_initiative = randomInt(1, 20);
    }

    vector<shared_ptr<Warrior>> warriors = blue_team;
    warriors.insert(warriors.end(), red_team.begin(), red_team.end());

    // Сортируем бойцов по инициативе
    int sort_counter = 0;
    do
    {
        sort_counter=0;
        for(int i=0; i<warriors.size() - 1; i++)
        {
            if(warriors[i]->bonus_initiative < warriors[i+1]->bonus_initiative)
            {
                swap(warriors[i], warriors[i+1]);
                sort_counter++;
            }
        }
    }while(sort_counter != 0);
    
    vector<string> stats;
    for(int i=0; i<warriors.size(); i++)
        stats.push_back("");
    
    // Начинаем бой
    bool win = false;
    int blue_counter = M;
    int red_counter = M;
    int all_damage = 0;
    int all_kills = 0;
    string winner;
    while(!win)
    {
        for(int i=0; i<warriors.size(); i++)
        {
            // Ищем близжайшего врага
            int index_enemy = find_enemy(warriors[i]->x, warriors[i]->y, plate, (warriors[i]->team == Team::Blue) ? red_team : blue_team);
            // Высчитываем дистанцию до врага
            double distance_enemy = distance(warriors[i]->x, warriors[i]->y, warriors[index_enemy]->x, warriors[index_enemy]->y);
            // Если хватает радиуса атаки - совершает удар
            if(ceil(distance_enemy) <= warriors[i]->attack_range)
            {
                // Высчитываем урон
                int damage1 = warriors[i]->attack() - warriors[index_enemy]->armor;
                if(damage1 <= 0){
                    damage1 = 1;
                }
                stats[i] += "deal " + to_string(damage1) + " damage to enemy #" + to_string(index_enemy) + "\n";

                // Если атакует маг
                if(warriors[i]->warrior_class == Warriors_class::Wizard)
                {
                    // Область атаки
                    int area_x[] = {warriors[index_enemy]->x, warriors[index_enemy]->x-1, warriors[index_enemy]->x+1};
                    int area_y[] = {warriors[index_enemy]->y, warriors[index_enemy]->y-1, warriors[index_enemy]->y+1};
                    
                    for(int i=0; i < 3; i++)
                    {
                        for(int j=0; j < 3; j++)
                        {
                            if((area_x[i] < 0 || area_x[i] > N-1) || (area_y[j] < 0 || area_y[j] > N-1))
                            {
                                continue;
                            }
                            else
                            {
                                if(plate[area_x[i]][area_y[j]] != NULL)
                                {
                                    if(plate[area_x[i]][area_y[j]]->team != warriors[i]->team)
                                    {
                                        plate[area_x[i]][area_y[j]]->hp -= damage1;
                                        all_damage += damage1;
                                        
                                        // Если противник нафидонил(лох), то удаляем с поля боя
                                        if(plate[area_x[i]][area_y[j]]->hp <= 0)
                                        {
                                            all_kills++;
                                            stats[i] += "Kill enemy\n";
                                            plate[area_x[i]][area_y[j]] = NULL;

                                            // Уменьшаем счетчик воинов в команде противников, если у противника "выпала кишка"
                                            if(warriors[index_enemy]->team == Team::Blue)
                                            {
                                                blue_counter--;
                                            }
                                            if(warriors[index_enemy]->team == Team::Red)
                                            {
                                                red_counter--;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
                // Если атакует не маг
                else
                {
                    warriors[index_enemy]->hp -= damage1;
                    all_damage += damage1;

                    // Если противник нафидонил(лох), то удаляем с поля боя
                    if(warriors[index_enemy]->hp <= 0)
                    {
                        all_kills++;
                        plate[warriors[index_enemy]->x][warriors[index_enemy]->y] = NULL;
                        stats[i] += "Kill enemy\n";

                        // Уменьшаем счетчик воинов в команде противников, если у противника "выпала кишка"
                        if(warriors[index_enemy]->team == Team::Blue)
                            blue_counter = (warriors[index_enemy]->hp <= 0) ? blue_counter-1 : blue_counter;
                        if(warriors[index_enemy]->team == Team::Red)
                            red_counter = (warriors[index_enemy]->hp <= 0) ? red_counter-1 : red_counter;

                    }
                }
            }
            // Если не хватает радиуса атаки
            else
            {
                // Задаем новые координаты для перемещения
                int new_x = warriors[i]->x;
                int new_y = warriors[i]->y;

                // Цикл поиска
                bool find = false;
                int walk_dist = 0; // Кол-во шагов
                while(!find)
                {
                    // Добавляем по 1 шагу
                    new_x += (warriors[index_enemy]->x > warriors[i]->x) ? 1 : -1;
                    new_y += (warriors[index_enemy]->y > warriors[i]->y) ? 1 : -1;
                    walk_dist++;
                    if(new_x > N-1 || new_x < 0)
                    {
                        new_x = (new_x > N-1) ? N-1 : 0;
                    }
                    if(new_y > N-1 || new_y < 0)
                    {
                        new_y = (new_y > N-1) ? N-1 : 0;
                    }

                    // Если прошёл больше возможного цикл завершается
                    if(walk_dist > warriors[i]->walk_range)
                    {
                        break;
                    }
                    // Если на новых координатах нет никого
                    if(plate[new_x][new_y] == NULL)
                    {
                        distance_enemy = distance(new_x, new_y, warriors[index_enemy]->x, warriors[index_enemy]->y);
                        // И дистанция позволяет нанести урон, то перемещаемся
                        if(ceil(distance_enemy) <= warriors[i]->attack_range)
                        {
                            find = true;
                            swap(plate[warriors[i]->x][warriors[i]->y], plate[new_x][new_y]);
                            warriors[i]->x = new_x;
                            warriors[i]->y = new_y;
                            stats[i] += "Move to (" + to_string(new_x) + ";" + to_string(new_y) + ")\n";
                            break;
                        }
                    }                
                }
            }
            
            // Если синих не осталось, то победа за красными
            if(blue_counter <= 0)
            {
                win = true;
                break;
            }
            // Если красных не осталось, то победа за синими
            else if(red_counter <= 0)
            {
                win = true;
                break;
            }
        }
        // Если синих не осталось, то победа за красными
        if(blue_counter <= 0)
        {
            winner = "==== RED TEAM WIN ====";
            cout << "==== RED TEAM WIN ====" << endl;
            win = true;
            break;
        }
        // Если красных не осталось, то победа за синими
        else if(red_counter <= 0)
        {
            winner = "==== BLUE TEAM WIN ====";
            cout << "==== BLUE TEAM WIN ====" << endl;
            win = true;
            break;
        }
    }

    // Выводим статистику каждого бойца
    for(int i=0; i<warriors.size(); i++)
    {
        cout << "Warrior #" << i << endl << stats[i] << endl;
    }

    // Выводим статистику боя
    cout << "Fight stats:" << endl
    << "Total damage(both team): " << all_damage << endl
    << "Total kills(both team): " << all_kills << endl
    << winner << endl;
}

/*Ф-ция меню управления игрой
*@param &blue_team команда бойцов синего флага
*@param &red_team команда бойцов красного флага
*@param &plate поле боя
*/
void menu(vector<shared_ptr<Warrior>> &blue_team, vector<shared_ptr<Warrior>> &red_team, Warrior *** plate)
{
    bool offline = false;
    while(!offline){
        cout << "=== Control menu ===" << endl;
        cout << "1. Generate teams" << endl
        << "2. Team stats" << endl
        << "3. Play 1vs1" << endl
        << "4. Team fight" << endl
        << "5. Exit" << endl;

        int user_setting_check = user_input(1, 5);
        switch (user_setting_check)
        {
            // 1. Generate teams
            case 1:
                fill_team(blue_team, Team::Blue);
                fill_team(red_team, Team::Red);
                break;
            
            // 1. 2. Team stats
            case 2:
                if(blue_team.empty() || red_team.empty())
                {
                   fill_team(blue_team, Team::Blue);
                    fill_team(red_team, Team::Red); 
                }
                print_team_stat(blue_team, Team::Blue);
                print_team_stat(red_team, Team::Red);

                cout << "Blue Team members= " << blue_team.size() << endl
                << "Red Team members= " << red_team.size() << endl;
                break;

            // 3. Play 1vs1
            case 3:
                if(blue_team.empty() || red_team.empty())
                {
                   fill_team(blue_team, Team::Blue);
                    fill_team(red_team, Team::Red); 
                }
                int warrior1_index;
                warrior1_index = randomInt(0, blue_team.size());
                cout << "warrior1_index: " << warrior1_index << endl;

                int warrior2_index;
                warrior2_index = randomInt(0, red_team.size());
                cout << "warrior2_index: " << warrior2_index << endl;

                round(*blue_team[warrior1_index], *red_team[warrior2_index]);
                break;

            // 4. Team fight
            case 4:
                if(blue_team.empty() || red_team.empty())
                {
                   fill_team(blue_team, Team::Blue);
                    fill_team(red_team, Team::Red); 
                }
                team_fight(blue_team, red_team, plate);
                break;

            // 5. Exit
            case 5:
                offline = true;
                break;
            default:
                break;
        }
    }
}

int main()
{
    srand(time(NULL));
    
    // Создаем поле N x N
    Warrior *** plate;
    plate = new Warrior**[N];
    for(int i=0; i < N; i++)
    {
        plate[i] = new Warrior*[N]();
    }
    
    
    // Команда синих
    vector<shared_ptr<Warrior>> warriors_blue;
    //Команда красных
    vector<shared_ptr<Warrior>> warriors_red;
    menu(warriors_blue, warriors_red, plate);

    // Удаляем динамический массив
    for(int i=0; i < N; i++)
    {
        delete[] plate[i];
    }
    return 0;
}


