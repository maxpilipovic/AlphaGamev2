#include "Player.h"

int Player::getCash() {
    return m_cash;
}

int Player::getLevel() {
    return m_level;
}

void Player::setCash(int cash) {
    m_cash = cash;
}

void Player::setLevel(int level) {
    m_level = level;
}