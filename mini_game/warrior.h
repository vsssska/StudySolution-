#include <iostream>
#include <vector>

using namespace std;

const int MIN_DAMAGE = 1;
const int MAX_DAMAGE = 20;

int randomInt(int min, int max) 
{
	return rand() % (max - min + 1) + min;
}

enum class Warriors_class : char {Melee = 'm', Range = 'r', Wizard = 'w'};
enum class Team : char {Blue = 'b', Red = 'r'};

class Warrior{
    public:
    int hp;
    int armor;
    int bonus_damage;
    int attack_range;
    int bonus_initiative;
    int walk_range;
    int x;
    int y;

    Team team;
    Warriors_class warrior_class;

    virtual int attack() =0;
};

class Melee : public Warrior{
    public:
    Melee(Team choosen_team)
    {
        int minHp = 10;
        int maxHp = 20;

        int minArmor = 5;
        int maxArmor = 20;

        int minBonus_damage = 1;
        int maxBonus_damage = 2;

        int minWalk_range = 5;
        int maxWalk_range = 10;
        
        walk_range = randomInt(minWalk_range, maxWalk_range);
        hp = randomInt(minHp, maxHp);
        armor = randomInt(minArmor, maxArmor);
        bonus_damage = randomInt(minBonus_damage, maxBonus_damage);
        attack_range = 1;

        team = choosen_team;
        warrior_class = Warriors_class::Melee;
    }

    int attack() override
    {
        return randomInt(MIN_DAMAGE, MAX_DAMAGE) + bonus_damage;
    }
};

class Range : public Warrior{
    public:
    
    Range(Team choosen_team)
    {
        int minHp = 5;
        int maxHp = 15;

        int minArmor = 1;
        int maxArmor = 10;

        int minBonus_damage = 5;
        int maxBonus_damage = 10;

        int minWalk_range = 2;
        int maxWalk_range = 6;
        
        walk_range = randomInt(minWalk_range, maxWalk_range);
        hp = randomInt(minHp, maxHp);
        armor = randomInt(minArmor, maxArmor);
        bonus_damage = randomInt(minBonus_damage, maxBonus_damage);
        attack_range = 10;

        team = choosen_team;
        warrior_class = Warriors_class::Range;
    }

    int attack() override
    {
        return randomInt(MIN_DAMAGE, MAX_DAMAGE) + bonus_damage;
    }
};

class Wizard : public Warrior{
    public:
    
    Wizard(Team choosen_team)
    {
        int minHp = 1;
        int maxHp = 12;

        int minArmor = 1;
        int maxArmor = 5;

        int minBonus_damage = 4;
        int maxBonus_damage = 8;

        int minWalk_range = 1;
        int maxWalk_range = 4;
        
        walk_range = randomInt(minWalk_range, maxWalk_range);
        hp = randomInt(minHp, maxHp);
        armor = randomInt(minArmor, maxArmor);
        bonus_damage = randomInt(minBonus_damage, maxBonus_damage);
        attack_range = 8;

        team = choosen_team;
        warrior_class = Warriors_class::Wizard;
    }

    int attack() override
    {
        return randomInt(MIN_DAMAGE, MAX_DAMAGE) + bonus_damage;
    }
};