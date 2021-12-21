#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include <algorithm>
#include <limits>

//Developed by Kaaleyah, bgs, sevenilx
//Credits to RedHook Studios

int d100() {
    return 1 + rand() % 100;
}

int dCustom(int a, int b) {
    return a + rand() % (b - a + 1);
}

int dExceptionCustom(int a, int b, int ex) {
    int result = dCustom(a, b);
    while (result == ex) {
        result = dCustom(a, b);
    }
    return result;
}

class Unit {
protected:
    double maxHP;
    double currentHP;
    double dodge;
    double prot;
    double speed;
    double tempSpeed;
    double accMod;
    double baseCrit;
    double minDmg;
    double maxDmg;
    //deathBlowResist;
    double stunResist;
    int position;
    bool isStunned;
    bool isDead;
    bool isDeathDoor;
    std::string name;
    std::string type;
    std::string unitType;
    int protLeft;

public:
    Unit (double MaxHP, double CurrentHP, double Dodge, double Prot, double Speed, double AccMod, double BaseCrit, double MinDmg, double MaxDmg, double StunResist, int Position,
          bool IsStunned, bool IsDead, bool IsDeathDoor, std::string Name, std::string Type, std::string UnitType):
            maxHP(MaxHP), currentHP(CurrentHP), dodge(Dodge), prot(Prot),
            speed(Speed), accMod(AccMod), baseCrit(BaseCrit),
            minDmg(MinDmg), maxDmg(MaxDmg), stunResist(StunResist), position(Position),
            isStunned(IsStunned), isDead(IsDead), isDeathDoor(IsDeathDoor), name(Name), tempSpeed(0), type(Type),
            protLeft(0), unitType(UnitType){}

    Unit() {};

    void setPosition(int position) {
        this->position = position;
    };

    int getPosition() {
        return position;
    }

    bool isFullHP() {
        return currentHP == maxHP;
    }

    double getAccMod() {
        return accMod;
    }

    double getDodge() {
        return dodge;
    }

    double getBaseCrit() {
        return baseCrit;
    }

    double getMinDmg() {
        return minDmg;
    }

    double getMaxDmg() {
        return maxDmg;
    }

    double getProt() {
        return prot;
    }

    double getHP() {
        return currentHP;
    }

    double getStunResist() {
        return stunResist;
    }

    double getSpeed() {
        return speed;
    }

    double getTempSpeed() {
        return tempSpeed;
    }

    std::string getName() {
        return name;
    }

    void giveProt(int protBonus, int round) {
        if (protLeft > 0) {
            protLeft = round;
        }
        else {
            prot += protBonus;
            protLeft += round;
        }
    }

    void decreaseProt(int protBonus) {
        if (protLeft > 1) {
            protLeft--;
        }
        else if (protLeft == 1) {
            protLeft--;
            prot -= protBonus;
        }
    }

    std::string getType() {
        return type;
    }

    std::string getUnitType() {
        return unitType;
    }

    void setSpeed(double newSpeed) {
        tempSpeed = newSpeed;
    }


    void stunOn() {
        isStunned = true;
    }

    void stunOff() {
        isStunned = false;
    }

    bool getStun() {
        return isStunned;
    }

    virtual void takeDamage() {}

    virtual void print() {
        std::cout << "[" << position << "] " << name << " HP: " << currentHP << " protection: " << prot << " ";
    }

    void DIE() {
        isDead = true;
        currentHP = 0;
        std::cout << name << " is done for now" << std::endl;
    }

    void teq() {
        currentHP = 0;
    }

    void OpenDeathsDoor() {
        isDeathDoor = true;
        std::cout << "Death's Door is opened for " << name << std::endl;
    }

    void CloseDeathsDoor() {
        isDeathDoor = false;
        std::cout << "Death's Door is closed for " << name << std::endl;
    }

    bool getIsDeathDoor() {
        return isDeathDoor;
    }

    void healUnit(int potion) {
        currentHP += potion;
        if (currentHP > maxHP) {
            currentHP = maxHP;
        }
        if (isDeathDoor) {
            CloseDeathsDoor();
        }
    }

    bool getIsDead() {
        return isDead;
    }

    bool operator < (const Unit &operand) {
        return tempSpeed > operand.tempSpeed;
    }

};

class Hero : public Unit {
protected:
    double deathBlowResist;

public:
    Hero (double MaxHP, double CurrentHP, double Dodge, double Prot, double Speed, double AccMod, double BaseCrit, double MinDmg, double MaxDmg, double DeathBlowResist, double StunResist, int Position,
          bool IsStunned, bool IsDead, bool IsDeathDoor, std::string Name, std::string UnitType):
            deathBlowResist(DeathBlowResist), Unit(MaxHP, CurrentHP, Dodge, Prot, Speed, AccMod, BaseCrit, MinDmg, MaxDmg, StunResist, Position, IsStunned, IsDead, IsDeathDoor, Name, "Hero", UnitType) {}

    Hero() {};

    void takeDamage(int damage) {
        if (damage >= 0) {
            currentHP -= damage;

            if (isDeathDoor) {
                if (damage > 0) {
                    if (d100() > deathBlowResist) {
                        DIE();
                    }
                    else {
                        std::cout << name << " has escaped grasps of death!" << std::endl;
                    }
                }
            }
            if (currentHP < 0) {
                currentHP = 0;
            }
        }
    }

    void print() {
        std::cout << "[" << position << "] " << name << " HP: " << currentHP << " protection: " << prot << " ";
        if (isDeathDoor) {
            std::cout << "|.| ";   //Death's Door is open
        }
        if (isStunned) {
            std::cout << " Stunned! ";
        }
    }

};

class Crusader : public Hero {
public:
    Crusader(std::string Name, int currentPosition): Hero(33, 33, 5, 0, 1, 0, 3,
                                                          6, 12, 67, 40, currentPosition, false, false, false, Name, "Crusader") {}

};

class Vestal : public Hero {
public:
    Vestal(std::string Name, int currentPosition): Hero(24, 24, 0, 0, 4, 0, 1,
                                                        4, 8, 77, 30, currentPosition, false, false, false, Name, "Vestal") {}

};

class Monster : public Unit {
public:
    Monster (double MaxHP, double CurrentHP, double Dodge, double Prot, double Speed, double AccMod, double BaseCrit, double MinDmg, double MaxDmg, double StunResist, int Position,
             bool IsStunned, bool IsDead, bool IsDeathDoor, std::string Name, std::string UnitType):
            Unit(MaxHP, CurrentHP, Dodge, Prot, Speed, AccMod, BaseCrit, MinDmg, MaxDmg, StunResist, Position, IsStunned, IsDead, IsDeathDoor, Name, "Monster", UnitType) {}

    Monster() {};

    void takeDamage(int damage) {
        if (damage >= 0) {
            currentHP -= damage;

            if (currentHP < 0) {
                currentHP = 0;
            }
        }
    }

    void print() {
        std::cout << "[" << position << "] " << name << " HP: " << currentHP << " protection: " << prot << "";
        if (isStunned) {
            std::cout << " Stunned!";
        }
        std::cout << " ";
    }
};

class BoneSoldier : public Monster {
public:
    BoneSoldier(std::string Name, int currentPosition): Monster(10, 10, 10, 15, 2, 0, 0,
                                                                3, 8, 25, currentPosition, false, false, false, Name, "BoneSoldier") {}

};

class BoneDefender : public Monster {
public:
    BoneDefender (std::string Name, int currentPosition): Monster(22, 22, 8, 45, 1, 0, 0,
                                                                  2, 4, 45, currentPosition, false, false, false, Name, "BoneDefender") {}
};


class StunSkill{
protected:
    double stunBase;

public:
    StunSkill(double StunBase): stunBase(StunBase) {}

    template <typename T>
    void Stun(int bonus, typename std::vector<T>::iterator defender) {
        int stunChance = bonus + stunBase - defender->getStunResist();

        if (d100() <= stunChance && defender->getIsDead() == false) {
            defender->stunOn();
            std::cout << defender->getName() << " is stunned!" << std::endl;
        }
    }
};

class Skill {
protected:
    std::vector<int> position;
    std::vector<int> target;
    std::string type;

public:
    Skill(std::vector<int> &Position, std::vector<int> &Target, std::string t): position(Position), target(Target), type(t) {}

    Skill(std::string t, std::vector<int> Position): type(t), position(Position) {}

    Skill() = default;

    //attack
    virtual bool attack(std::shared_ptr<std::vector<Monster>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Hero>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        return false;
    };
    virtual bool attack(std::shared_ptr<std::vector<Hero>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Monster>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        return false;
    };

    //utility
    virtual bool effect(std::vector<Unit>::iterator healer, std::vector<Hero>::iterator wounded, std::shared_ptr<std::vector<Hero>> woundedTeam) {
        return false;
    };
    virtual bool effect(std::vector<Unit>::iterator healer, std::vector<Monster>::iterator wounded, std::shared_ptr<std::vector<Monster>> woundedTeam) {
        return false;
    };

    //move
    virtual bool move(std::shared_ptr<std::vector<Hero>> Units, std::vector<Unit>::iterator attacker, int toWHere, std::shared_ptr<std::vector<Unit>> order) {
        return false;
    };
    virtual bool move(std::shared_ptr<std::vector<Monster>> Units, std::vector<Unit>::iterator attacker, int toWHere, std::shared_ptr<std::vector<Unit>> order) {
        return false;
    };

    void print() {
        std::cout << type << "  ";
    }

    void printTargets(std::shared_ptr<std::vector<Monster>> monsterPosition) {
        std::cout << "{ ";
        for (auto i = target.begin(); i < target.end(); i++) {
            for (auto j = monsterPosition->begin(); j < monsterPosition->end(); j++) {
                if ((*i) == j->getPosition()) {
                    std::cout << *i << " ";
                }
            }
        }
        std::cout << "}";
    }

    void printPosition() {
        std::cout << "{";
        for (auto i = position.begin(); i < position.end(); i++) {
            if (i == position.end() - 1) {
                std::cout << *i;
            }
            else {
                std::cout << *i << "  ";
            }
        }
        std::cout << "}";
    }

    bool check(std::vector<Unit>::iterator attacker, int targetPosition) {
        if (type == "BulwarkOfFaith") {
            for (auto i = position.begin(); i < position.end(); i++) {
                if (attacker->getPosition() == *i) {
                    return true;
                }
            }
        }
        else {
            for (auto i = position.begin(); i < position.end(); i++) {
                if (attacker->getPosition() == *i) {

                    for (auto i = target.begin(); i < target.end(); i++) {
                        if (targetPosition == *i) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool checkPosition(std::vector<Unit>::iterator attacker) {
        for (auto i = position.begin(); i < position.end(); i++) {
            if (attacker->getPosition() == *i) {
                return true;
            }
        }
        return false;
    }

    template <typename T>
    bool checkTargets(typename std::vector<T>::iterator defender) {
        for (auto i = target.begin(); i < target.end(); i++) {
            if (defender->getPosition() == *i) {
                return true;
            }
        }
        return false;
    }

    std::string getType() {
        return type;
    }
};

class MoveSkill : public Skill {
public:
    MoveSkill(): Skill("Move", {1, 2, 3, 4})  {}

    template <typename T>
    bool move(std::shared_ptr<std::vector<T>> Units, std::vector<Unit>::iterator attacker, int toWHere, std::shared_ptr<std::vector<Unit>> order) {
        if (toWHere == attacker->getPosition()) {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
        else {
            std::string attackerType = attacker->getType();
            auto moving = std::make_shared<std::vector<T>>();

            moving->reserve(1);

            auto targetPosition = Units->begin() + toWHere -1;

            auto movingOne = Units->begin() + attacker->getPosition() - 1;

            moving->push_back(*movingOne);

            Units->erase(movingOne);

            Units->insert(targetPosition, moving->at(0));

            std::cout << attacker->getName() << " moved from " << attacker->getPosition() << " to " << toWHere << std::endl;

            int j = 1;
            for (auto i = Units->begin(); i < Units->end(); i++) {
                i->setPosition(j);
                j++;
            }

            for (auto k = order->begin(); k < order->end(); k++) {
                for (auto l = Units->begin(); l < Units->end(); l++) {
                    if (l->getName() == k->getName()) {
                        k->setPosition(l->getPosition());
                    }
                }
            }
        }
        return true;
    }
};

class AttackSkill : public Skill {
protected:
    double dmgMod;
    double baseAcc;
    double critMod;
    bool stunEffect;
    bool isCrit;

public:
    AttackSkill(std::vector<int> Position, std::vector<int> Target, double DmgMod, double BaseAcc, double CritMod, bool StunEffect, std::string  t):
            Skill(Position, Target, t), dmgMod(DmgMod), baseAcc(BaseAcc), critMod(CritMod), stunEffect(StunEffect) {}


    AttackSkill() = default;

    template <typename T>
    int calculateAttack(std::vector<Unit>::iterator attacker, typename std::vector<T>::iterator defender) {
        int hitChance = baseAcc + attacker->getAccMod() - defender->getDodge();

        std::cout << attacker->getName() << " used " << type << " on " << defender->getName() << std::endl;

        if (hitChance < d100()) {
            std::cout << "Miss" << std::endl;
            return -10;
        }
        else {
            int critChance = attacker->getBaseCrit() + critMod;
            int actualDmg;

            if (critChance >= d100()) {
                actualDmg = 1.5 * attacker->getMaxDmg();
                isCrit = true;
                std::cout << "Critical Hit!" << std::endl;
            }
            else {
                int dmg = dCustom(attacker->getMinDmg(), attacker->getMaxDmg());

                int rawDmg = dmg * ((100 + dmgMod) / 100);
                actualDmg = rawDmg - rawDmg * (attacker->getProt() / 100);
            }
            std::cout << "Hit, " << actualDmg << " damage" << std::endl;
            return actualDmg;
        }
    }
};

class Smite : public AttackSkill {
public:
    Smite(): AttackSkill({1, 2}, {1, 2}, 0, 85, 0, false, "Smite") {};

    bool attack(std::shared_ptr<std::vector<Hero>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Monster>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            defender->takeDamage(calculateAttack<Monster>(attacker, defender));
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class StunningBlow : public AttackSkill, public StunSkill {
public:
    StunningBlow() :AttackSkill( {1, 2}, {1, 2}, -50, 90, 0, true, "StunningBlow"), StunSkill(100) {}

    bool attack(std::shared_ptr<std::vector<Hero>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Monster>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            int damage = calculateAttack<Monster>(attacker, defender);
            defender->takeDamage(damage);
            int bonus = 0;
            if (isCrit) {
                bonus = 20;
                isCrit = false;
            }
            if (damage >= 0) {
                Stun<Monster>(bonus, defender);
            }
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class HolyLance : public AttackSkill {
protected:
    std::shared_ptr<MoveSkill> m = std::shared_ptr<MoveSkill>();
public:
    HolyLance():AttackSkill({3, 4}, {2, 3, 4}, 0, 85, 6.5, false, "HolyLance") {}

    bool attack(std::shared_ptr<std::vector<Hero>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Monster>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            defender->takeDamage(calculateAttack<Monster>(attacker, defender));

            int position = attacker->getPosition();

            m->move<Hero>(attackerTeam, attacker, position - 1, order);  //move
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class MaceBash : public AttackSkill {
public:
    MaceBash(): AttackSkill({1,2}, {1,2}, 0, 85, 0, false, "MaceBash") {}

    bool attack(std::shared_ptr<std::vector<Hero>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Monster>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            defender->takeDamage(calculateAttack<Monster>(attacker, defender));
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class DazzlingLight : public AttackSkill, public StunSkill {
public:
    DazzlingLight(): AttackSkill({2,3,4}, {1,2,3}, -75, 90, 5, true, "DazzlingLight"), StunSkill(100) {}

    bool attack(std::shared_ptr<std::vector<Hero>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Monster>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            int damage = calculateAttack<Monster>(attacker, defender);
            defender->takeDamage(damage);
            int bonus = 0;
            if (isCrit) {
                bonus = 20;
                isCrit = false;
            }
            if (damage >= 0) {
                Stun<Monster>(bonus, defender);
            }
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class GraveyardSlash : public AttackSkill {
public:
    GraveyardSlash(): AttackSkill({1,2,3}, {1,2,3}, 0, 85, 6, false, "GraveYardSlash") {}

    bool attack(std::shared_ptr<std::vector<Monster>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Hero>::iterator defender, std::shared_ptr<std::vector<Unit>> order)   {
        if (check(attacker, defender->getPosition())) {
            defender->takeDamage(calculateAttack<Hero>(attacker, defender));
            return true;
        }
        else {
            return false;
        }
    }
};

class GraveyardStumble : public AttackSkill {
protected:
    std::shared_ptr<MoveSkill> m = std::shared_ptr<MoveSkill>();
public:
    GraveyardStumble(): AttackSkill({4}, {1, 2}, -50, 45, 0, false, "GraveyardStumble") {}

    bool attack(std::shared_ptr<std::vector<Monster>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Hero>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            defender->takeDamage(calculateAttack<Hero>(attacker, defender));

            int position = attacker->getPosition();

            m->move(attackerTeam, attacker, position - 1, order);    //move
            return true;
        }
        else {
            return false;
        }
    }
};

class AxeBlade : public AttackSkill {
public:
    AxeBlade(): AttackSkill({1, 2}, {1, 2}, 0, 72, 6, false, "AxeBlade") {}

    bool attack(std::shared_ptr<std::vector<Monster>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Hero>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            defender->takeDamage(calculateAttack<Hero>(attacker, defender));
            return true;
        }
        else {
            return false;
        }
    }
};

class DeadWeight : public AttackSkill, StunSkill {
public:
    DeadWeight(): AttackSkill({1, 2, 3}, {1, 2}, -25, 82, 6, true, "DeadWeight"), StunSkill(100) {}

    bool attack(std::shared_ptr<std::vector<Monster>> attackerTeam, std::vector<Unit>::iterator attacker, std::vector<Hero>::iterator defender, std::shared_ptr<std::vector<Unit>> order) {
        if (check(attacker, defender->getPosition())) {
            int damage = calculateAttack<Hero>(attacker, defender);
            defender->takeDamage(damage);
            int bonus = 0;
            if (isCrit) {
                bonus = 20;
                isCrit = false;
            }
            if (damage >= 0) {
                Stun<Hero>(bonus, defender);
            }
            return true;
        }
        else {
            return false;
        }
    }

};

class UtilitySkill : public Skill {
protected:
    std::vector<int> potion;
public:
    UtilitySkill(std::vector<int> Position, std::vector<int> Target, std::vector<int> Potion, std::string t): Skill(Position, Target, t), potion(Potion) {}

    template <typename H>
    void heal(std::vector<Unit>::iterator healer, typename std::vector<H>::iterator wounded) {
        if (checkPosition(healer)) {
            auto iT = potion.begin();
            int pot = dCustom(*iT, *(iT+1));
            wounded->healUnit(pot);
            std::cout << healer->getName() << " has healed " << wounded->getName() << " by " << pot << std::endl;
        }
        else {
            std::cout << "Invalid" << std::endl;
        }
    }
};

class BulwarkOfFaith : public UtilitySkill {
protected:
    int roundDuration;
public:
    BulwarkOfFaith(): UtilitySkill({1, 2}, {0}, {20, 20}, "BulwarkOfFaith"), roundDuration(3){}

    bool effect(std::vector<Unit>::iterator healer, std::vector<Hero>::iterator wounded, std::shared_ptr<std::vector<Hero>> woundedTeam) {
        if (checkPosition(healer)) {
            int pot = potion.at(0);
            wounded->giveProt(pot, roundDuration);
            std::cout << healer->getName() << " has given " << pot <<  " protection to themselves for " << roundDuration << " rounds"  << std::endl;
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class DivineGrace : public UtilitySkill {
public:
    bool effect(std::vector<Unit>::iterator healer, std::vector<Hero>::iterator wounded, std::shared_ptr<std::vector<Hero>> woundedTeam) {
        if (checkPosition(healer)) {
            heal<Hero>(healer, wounded);
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }

    DivineGrace(): UtilitySkill({3, 4}, {1, 2, 3, 4}, {4, 5}, "DivineGrace"){}
};

class DivineComfort : public UtilitySkill {
public:
    DivineComfort(): UtilitySkill({2, 3, 4}, {1, 2, 3, 4}, {1, 3}, "DivineComfort") {}

    bool effect(std::vector<Unit>::iterator healer, std::vector<Hero>::iterator wounded, std::shared_ptr<std::vector<Hero>> woundedTeam) {
        if (checkPosition(healer)) {
            for (auto i = woundedTeam->begin(); i < woundedTeam->end(); i++) {
                heal<Hero>(healer, i);
            }
            return true;
        }
        else {
            std::cout << "Invalid use" << std::endl;
            return false;
        }
    }
};

class KnittingBones : public UtilitySkill {
public:
    KnittingBones(): UtilitySkill({3,4}, {1,2,3,4}, {2,3}, "KnittingBones") {}

    bool effect(std::vector<Unit>::iterator healer, std::vector<Monster>::iterator wounded, std::shared_ptr<std::vector<Monster>> woundedTeam) {
        if (checkPosition(healer)) {
            heal<Monster>(healer, wounded);
            return true;
        }
        else {
            return false;
        }
    }
};

template <typename T>
void checkDying(typename std::vector<T>::iterator isDying) {
    if (isDying->getHP() <= 0) {
        if (isDying->getType() == "Hero") {
            if (isDying->getIsDeathDoor() == false) {
                isDying->OpenDeathsDoor();
            }
        }
        else {
            isDying->DIE();
        }
    }
}

template <typename T>
void grave(std::shared_ptr<std::vector<T>> team, std::shared_ptr<std::vector<Unit>> order) {
    bool thereIsDead = false;
    for (auto i = team->begin(); i < team->end(); i++) {
        if (i->getIsDead()) {
            team->erase(i);
            thereIsDead = true;
        }
    }

    for (auto i = order->begin(); i < order->end(); i++) {
        if (i->getIsDead()) {
            order->erase(i);
        }
    }

    if (thereIsDead) {
        for (int i = 0; i < team->size(); i++) {
            team->at(i).setPosition(i+1);
        }
    }

    for (auto k = order->begin(); k < order->end(); k++) {
        for (auto l = team->begin(); l < team->end(); l++) {
            if (l->getName() == k->getName()) {
                k->setPosition(l->getPosition());
            }
        }
    }
}

void updateGame(std::shared_ptr<std::vector<Hero>> heroes, std::shared_ptr<std::vector<Monster>> monsters, std::shared_ptr<std::vector<Unit>> newOrder) {
    for (auto i = newOrder->begin(); i < newOrder->end(); i++) {
        for (auto h = heroes->begin(); h < heroes->end(); h++) {
            if (i->getName() == h->getName()) {
                int pos = i->getPosition() - 1;
                newOrder->erase(i);
                newOrder->insert(i, heroes->at(h->getPosition() - 1));
            }
        }
        for (auto m = monsters->begin(); m < monsters->end(); m++) {
            if (i->getName() == m->getName()) {
                int pos = i->getPosition() - 1;
                newOrder->erase(i);
                newOrder->insert(i, monsters->at(m->getPosition() - 1));
            }
        }
    }
}


void setRoundOrder(std::shared_ptr<std::vector<Hero>> heroes, std::shared_ptr<std::vector<Monster>> monsters, std::shared_ptr<std::vector<Unit>> newOrder) {
    newOrder->clear();

    for (auto i = heroes->begin(); i < heroes->end(); i++) {
        i->setSpeed(i->getSpeed() + dCustom(1, 8));
        newOrder->push_back(*i);
    }

    for (auto i = monsters->begin(); i < monsters->end(); i++) {
        i->setSpeed(i->getSpeed() + dCustom(1, 8));
        newOrder->push_back(*i);
    }

    bool sort = true;
    std::vector<Unit> maxUnit(1);

    std::sort(newOrder->begin(), newOrder->end());

    std::cout << std::endl;

    for (auto i = newOrder->begin(); i < newOrder->end(); i++) {
        i->print();
        std::cout << std::endl;
    }
}

bool isGameOver(std::shared_ptr<std::vector<Hero>> heroes, std::shared_ptr<std::vector<Monster>> monsters) {
    if (heroes->empty()) {
        std::cout << "Game Over, all heroes are dead! What a shame!";
        return true;
    }
    else if (monsters->empty()) {
        std::cout << "Game Over, all monsters are dead! Glory to our Heroes!!";
        return true;
    }
    return false;
}

void printAll(std::shared_ptr<std::vector<Hero>> heroes, std::shared_ptr<std::vector<Monster>> monsters) {
    std::cout << "Hero Team" << std::endl;
    int even = 0;
    for (auto i = heroes->begin(); i < heroes->end(); i++) {
        i->print();
        even++;
        if (even == 2) {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << std::endl;

    even = 0;
    std::cout << "Monster Team" << std::endl;
    for (auto i = monsters->begin(); i < monsters->end(); i++) {
        i->print();
        even++;
        if (even == 2) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void printSkills(std::shared_ptr<std::vector<std::shared_ptr<Skill>>> skills) {
    int index = 1;
    std::cout << "| ";
    for (auto i = skills->begin(); i < skills->end(); i++) {
        std::cout << "(" << index << ") " << (*i)->getType() << " ";
        index++;
    }
    std::cout << "|" << std::endl;
}

bool isTeamFullHP(std::shared_ptr<std::vector<Monster>> team) {
    for (auto i = team->begin(); i < team->end(); i++) {
        if (!(i->isFullHP())) {
            return false;
        }
    }
    return true;
}

//function for printing status

int main() {
    srand(time(nullptr));

    auto Heroes = std::make_shared<std::vector<Hero>>();
    auto Monsters = std::make_shared<std::vector<Monster>>();

    Heroes->reserve(4);
    Heroes->emplace_back(Crusader("Lovell - C", 1));    //Crusader
    Heroes->emplace_back(Vestal("Pericard - V", 2));    //Vestal
    Heroes->emplace_back(Crusader("Valance - C", 3));   //Crusader
    Heroes->emplace_back(Vestal("Pipin - V", 4));       //Vestal

    Monsters->reserve(4);
    Monsters->emplace_back(BoneSoldier("Buttons - S", 1));      //BoneSoldier
    Monsters->emplace_back(BoneDefender("Whitewash - D", 2));   //BoneDefender
    Monsters->emplace_back(BoneDefender("Fetidling - D", 3));   //BoneDefender
    Monsters->emplace_back(BoneSoldier("Brinecat - S", 4));     //BoneSoldier

    auto order = std::make_shared<std::vector<Unit>>();

    order->reserve(8);

    auto CrusaderSkillSet = std::make_shared<std::vector<std::shared_ptr<Skill>>>();
    CrusaderSkillSet->reserve(5);

    CrusaderSkillSet->push_back(std::make_shared<Smite>());
    CrusaderSkillSet->push_back(std::make_shared<StunningBlow>());
    CrusaderSkillSet->push_back(std::make_shared<HolyLance>());
    CrusaderSkillSet->push_back(std::make_shared<BulwarkOfFaith>());
    CrusaderSkillSet->push_back(std::make_shared<MoveSkill>());

    auto VestalSkillSet = std::make_shared<std::vector<std::shared_ptr<Skill>>>();
    VestalSkillSet->reserve(5);

    VestalSkillSet->push_back(std::make_shared<MaceBash>());
    VestalSkillSet->push_back(std::make_shared<DazzlingLight>());
    VestalSkillSet->push_back(std::make_shared<DivineGrace>());
    VestalSkillSet->push_back(std::make_shared<DivineComfort>());
    VestalSkillSet->push_back(std::make_shared<MoveSkill>());

    auto BoneSoldierSkillSet = std::make_shared<std::vector<std::shared_ptr<Skill>>>();
    BoneSoldierSkillSet->reserve(3);

    BoneSoldierSkillSet->push_back(std::make_shared<GraveyardSlash>());
    BoneSoldierSkillSet->push_back(std::make_shared<GraveyardStumble>());
    BoneSoldierSkillSet->push_back(std::make_shared<MoveSkill>());

    auto BoneDefenderSkillSet = std::make_shared<std::vector<std::shared_ptr<Skill>>>();
    BoneDefenderSkillSet->reserve(4);

    BoneDefenderSkillSet->push_back(std::make_shared<AxeBlade>());
    BoneDefenderSkillSet->push_back(std::make_shared<DeadWeight>());
    BoneDefenderSkillSet->push_back(std::make_shared<KnittingBones>());
    BoneDefenderSkillSet->push_back(std::make_shared<MoveSkill>());

    MoveSkill m;

    //All fields are declared

    int roundCount = 1;
    std::cout << std::endl;
    std::cout << "                   ______ ___  ___ _____    ______ ______  _____  _____  _____  _   _  _____  _____ " << std::endl;
    std::cout << "                   |  _  \\|  \\/  ||  __ \\   | ___ \\| ___ \\|  ___|/  ___||  ___|| \\ | ||_   _|/  ___|" << std::endl;
    std::cout << "                   | | | || .  . || |  \\/   | |_/ /| |_/ /| |__  \\ `--. | |__  |  \\| |  | |  \\ `--. " << std::endl;
    std::cout << "                   | | | || |\\/| || | __    |  __/ |    / |  __|  `--. \\|  __| | . ` |  | |   `--. \\" << std::endl;
    std::cout << "                   | |/ / | |  | || |_\\ \\   | |    | |\\ \\ | |___ /\\__/ /| |___ | |\\  |  | |  /\\__/ /" << std::endl;
    std::cout << "                   |___/  \\_|  |_/ \\____/   \\_|    \\_| \\_|\\____/ \\____/ \\____/ \\_| \\_/  \\_/  \\____/ " << std::endl;

    std::cout << " ______   _______  _______  _        _______  _______ _________   _______  _______  _______  ______   _______ _________" << std::endl;
    std::cout << "(  __  \\ (  ___  )(  ____ )| \\    /\\(  ____ \\(  ____ \\\\__   __/  (  ____ \\(  ___  )(       )(  ___ \\ (  ___  )\\__   __/" << std::endl;
    std::cout << "| (  \\  )| (   ) || (    )||  \\  / /| (    \\/| (    \\/   ) (     | (    \\/| (   ) || () () || (   ) )| (   ) |   ) (   " << std::endl;
    std::cout << "| |   ) || (___) || (____)||  (_/ / | (__    | (_____    | |     | |      | |   | || || || || (__/ / | (___) |   | |   " << std::endl;
    std::cout << "| |   | ||  ___  ||     __)|   _ (  |  __)   (_____  )   | |     | |      | |   | || |(_)| ||  __ (  |  ___  |   | |   " << std::endl;
    std::cout << "| |   ) || (   ) || (\\ (   |  ( \\ \\ | (            ) |   | |     | |      | |   | || |   | || (  \\ \\ | (   ) |   | |   " << std::endl;
    std::cout << "| (__/  )| )   ( || ) \\ \\__|  /  \\ \\| (____/\\/\\____) |   | |     | (____/\\| (___) || )   ( || )___) )| )   ( |   | |   " << std::endl;
    std::cout << "(______/ |/     \\||/   \\__/|_/    \\/(_______/\\_______)   )_(     (_______/(_______)|/     \\||/ \\___/ |/     \\|   )_(   " << std::endl;
    std::cout << std::endl;
    std::cout << "                                             Designed to run on NVIDIA BREAD" << std::endl;
    std::cout << std::endl;

    std::cout << "                                                   Hit enter to start" << std::endl;
    std::string start;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    bool gameOver = false;

    while (true) {
        std::cout << R"(\(^_^)---ROUND )" << roundCount << "---(^_^)/" << std::endl;
        setRoundOrder(Heroes, Monsters, order);     //Round begin

        for (auto i = Heroes->begin(); i < Heroes->end(); i++) {
            (*i).decreaseProt(20);
        }
        for (auto i = Monsters->begin(); i < Monsters->end(); i++) {
            (*i).decreaseProt(20);
        }

        auto skillToBeUsed = BoneSoldierSkillSet->begin();

        for (auto willPlay = order->begin(); willPlay < order->end(); willPlay++) {

            std::cout << std::endl;
            std::cout << "Turn started" << std::endl;
            std::cout << std::endl;

            std::cout << std::endl;
            std::cout << std::endl;
            (*willPlay).print();
            std::cout << " will play" << std::endl;
            std::cout << std::endl;

            printAll(Heroes, Monsters);     //Turn Begin

            if (willPlay->getStun()) {
                (*willPlay).print();
                std::cout << " cannot play, is stunned" << std::endl;
                if (willPlay->getType() == "Hero") {
                    Heroes->at(willPlay->getPosition() - 1).stunOff();
                }
                else {
                    Monsters->at(willPlay->getPosition() - 1).stunOff();
                }

                updateGame(Heroes, Monsters, order);
                std::cout << "Turn finished. Press enter to continue..." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
            else {
                if (willPlay->getType() == "Hero") {
                    auto target = Monsters->begin();
                    int skillIndex;
                    int targetIndex;
                    auto chosenSkill = CrusaderSkillSet->begin();

                    if (willPlay->getUnitType() == "Crusader") {
                        while (true) {
                            printSkills(CrusaderSkillSet);
                            std::cout << "Choose skill index: ";
                            std::cin >> skillIndex;
                            std::cout << std::endl;
                            while (skillIndex < 1 || skillIndex > 5) {
                                std::cout << "Invalid skill index! Choose skill: ";
                                std::cin >> skillIndex;
                            }
                            while (skillIndex == 5 && (Heroes->end()-1)->getPosition() == 1) {
                                std::cout << "You can't move. You are the last man standing!" << std::endl;
                                std::cout << "Choose skill index: ";
                                std::cin >> skillIndex;
                            }
                            while (skillIndex == 3 && (Monsters->end()-1)->getPosition() == 1) {
                                std::cout << "There are no monsters that you can attack with this skill." << std:: endl;
                                std::cout << "Choose skill index: ";
                                std::cin >> skillIndex;
                                std::cout << std::endl;
                            }
                            chosenSkill = CrusaderSkillSet->begin() + skillIndex - 1;

                            if ((*chosenSkill)->checkPosition(willPlay)) {
                                break;
                            }
                            std::cout << (*chosenSkill)->getType() << std::endl;
                            std::cout << "You cannot use this skill at this specific position" << std::endl;
                            std::cout << "You can only use this skill at these specific stated positions: ";
                            (*chosenSkill)->printPosition();
                            std::cout << std::endl;
                        }

                        if ((*chosenSkill)->getType() == "BulwarkOfFaith") {
                            auto wounded = Heroes->begin() + willPlay->getPosition() - 1;
                            (*chosenSkill)->effect(willPlay, wounded, Heroes);
                        }
                        else if ((*chosenSkill)->getType() =="Move") {
                            while (true) {
                                std::cout << "Choose a place to move: ";
                                std::cin >> targetIndex;
                                if ((*willPlay).getPosition() == targetIndex) {
                                    std::cout << "Invalid, you cannot move to your own location." << std::endl;
                                }
                                else if (targetIndex > (Heroes->end()-1)->getPosition() || targetIndex <= 0) {
                                    std::cout << "Invalid, you can not move out of the team boundaries." << std::endl;
                                }
                                else {
                                    m.move(Heroes, willPlay, targetIndex, order);
                                    break;
                                }
                            }

                        }
                        else {
                            while (true) {
                                std::cout << "Choose an opponent to attack: ";
                                (*chosenSkill)->printTargets(Monsters);
                                std::cout << std::endl;
                                std::cin >> targetIndex;
                                target = Monsters->begin() + targetIndex - 1;

                                if ((*chosenSkill)->checkTargets<Monster>(target) && targetIndex <= (Monsters->end()-1)->getPosition() && targetIndex > 0) {
                                    (*chosenSkill)->attack(Heroes, willPlay, target, order);
                                    break;
                                }
                                std::cout << "Invalid Target" << std::endl;
                            }
                        }
                    }
                    else {
                        int skillIndex;
                        int targetIndex;
                        auto chosenSkill = VestalSkillSet->begin();

                        while (true) {
                            printSkills(VestalSkillSet);
                            std::cout << "Choose skill index: ";
                            std::cin >> skillIndex;
                            std::cout << std::endl;
                            while (skillIndex < 1 || skillIndex > 5) {
                                std::cout << "Invalid skill index! Choose skill: ";
                                std::cin >> skillIndex;
                            }
                            while (skillIndex == 5 && (Heroes->end()-1)->getPosition() == 1) {
                                std::cout << "You can't move. You are the last man standing!" << std::endl;
                                std::cout << "Choose skill index: ";
                                std::cin >> skillIndex;
                            }
                            chosenSkill = VestalSkillSet->begin() + skillIndex - 1;

                            if ((*chosenSkill)->checkPosition(willPlay)) {
                                break;
                            }
                            std::cout << (*chosenSkill)->getType() << std::endl;
                            std::cout << "You cannot use this skill at this specific position" << std::endl;
                            std::cout << "You can only use this skill at these specific stated positions: ";
                            (*chosenSkill)->printPosition();
                            std::cout << std::endl;
                        }

                        if ((*chosenSkill)->getType() == "Move") {
                            while (true) {
                                std::cout << "Choose a place to move: ";
                                std::cin >> targetIndex;
                                if ((*willPlay).getPosition() == targetIndex) {
                                    std::cout << "Invalid, you cannot move to your own location." << std::endl;
                                }
                                else if (targetIndex > (Heroes->end()-1)->getPosition() || targetIndex <= 0) {
                                    std::cout << "Invalid, you can not move out of the team boundaries." << std::endl;
                                }
                                else {
                                    m.move(Heroes, willPlay, targetIndex, order);
                                    break;
                                }
                            }
                        }
                        else if ((*chosenSkill)->getType() == "DivineComfort") {
                            auto wounded = Heroes->begin();
                            (*chosenSkill)->effect(willPlay, wounded, Heroes);
                        }
                        else if ((*chosenSkill)->getType() == "DivineGrace") { //?
                            while (true) {
                                std::cout << "Choose a teammate to heal: ";
                                std::cin >> targetIndex;
                                if (targetIndex <= (Heroes->end()-1)->getPosition() && targetIndex > 0) {
                                    auto wounded = Heroes->begin() + targetIndex - 1;
                                    (*chosenSkill)->effect(willPlay, wounded, Heroes);
                                    break;
                                }
                                std::cout << "Wrong input!" << std::endl;
                            }
                        }
                        else {
                            while (true) {
                                std::cout << "Choose an opponent to attack: ";
                                (*chosenSkill)->printTargets(Monsters);
                                std::cout << std::endl;
                                std::cin >> targetIndex;
                                target = Monsters->begin() + targetIndex - 1;

                                if ((*chosenSkill)->checkTargets<Monster>(target)  && targetIndex <= (Monsters->end()-1)->getPosition() && targetIndex > 0) {
                                    (*chosenSkill)->attack(Heroes, willPlay, target, order);
                                    break;
                                }
                                std::cout << "Invalid Target" << std::endl;
                            }
                        }

                    }
                    checkDying<Monster>(target);
                }



                else {
                    int skillChance = d100();

                    if (skillChance <= 10 && (Monsters->end() - 1)->getPosition() != 1) {        //%10 Move Skill
                        m.move(Monsters, willPlay, dExceptionCustom(1, (Monsters->end()-1)->getPosition(), willPlay->getPosition()), order);
                    }

                        //%90 Attack and Utility Skills
                    else {
                        if (willPlay->getUnitType() == "BoneSoldier") {
                            skillToBeUsed = BoneSoldierSkillSet->begin() + dCustom(0, 1);
                            auto target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));

                            while (!((*skillToBeUsed)->attack(Monsters, willPlay, target, order))) {
                                target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));
                                skillToBeUsed = BoneSoldierSkillSet->begin() + dCustom(0, 1);
                            }
                            checkDying<Hero>(target);
                        }
                        else {    //BoneDefender

                            skillToBeUsed = BoneDefenderSkillSet->begin() + dCustom(0, 2);

                            auto target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));
                            auto teammate = Monsters->begin() + dCustom(0, ((Monsters->end()-1)->getPosition() - 1));


                            if (isTeamFullHP(Monsters)) {
                                if (willPlay->getPosition() == 4) {
                                    m.move(Monsters, willPlay, dExceptionCustom(1, (Monsters->end()-1)->getPosition(), willPlay->getPosition()), order);
                                }
                                else if (willPlay->getPosition() == 3) {
                                    skillToBeUsed = BoneDefenderSkillSet->begin() + 1;
                                    while (!((*skillToBeUsed)->attack(Monsters, willPlay, target, order))) {
                                        target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));
                                    }
                                }
                                else {
                                    skillToBeUsed = BoneDefenderSkillSet->begin() + dCustom(0, 1);
                                    while (!((*skillToBeUsed)->attack(Monsters, willPlay, target, order))) {
                                        target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));
                                    }
                                }
                            }
                            else {
                                if (willPlay->getPosition() == 4) {
                                    skillToBeUsed = BoneDefenderSkillSet->begin() + 2;
                                    while(teammate->isFullHP()) {
                                        teammate = Monsters->begin() + dCustom(0, ((Monsters->end()-1)->getPosition() - 1));
                                    }
                                    (*skillToBeUsed)->effect(willPlay, teammate, Monsters);
                                }
                                else if (willPlay->getPosition() == 3) {
                                    skillToBeUsed = BoneDefenderSkillSet->begin() + dCustom(1, 2);
                                    if ((*skillToBeUsed)->getType() == "KnittingBones") {
                                        while(teammate->isFullHP()) {
                                            teammate = Monsters->begin() + dCustom(0, ((Monsters->end()-1)->getPosition() - 1));
                                        }
                                        (*skillToBeUsed)->effect(willPlay, teammate, Monsters);
                                    }
                                    else {
                                        while (!((*skillToBeUsed)->attack(Monsters, willPlay, target, order))) {
                                            target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));
                                        }
                                    }
                                }
                                else {
                                    skillToBeUsed = BoneDefenderSkillSet->begin() + dCustom(0, 1);
                                    while (!((*skillToBeUsed)->attack(Monsters, willPlay, target, order))) {
                                        target = Heroes->begin() + dCustom(0, ((Heroes->end()-1)->getPosition() - 1));
                                    }
                                }
                            }
                            checkDying<Hero>(target);
                        }
                    }
                }
                updateGame(Heroes, Monsters, order);
                grave<Monster>(Monsters, order);
                grave<Hero>(Heroes, order);
                std::cout << "Turn finished. Press enter to continue..." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                if ((*willPlay).getType() == "Hero") {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                }
                if (isGameOver(Heroes, Monsters)) {
                    gameOver = true;
                    break;
                }
            }
        }
        roundCount++;
        if (gameOver) {
            break;
        }
    }
}