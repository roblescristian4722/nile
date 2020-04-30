#include "user.h"

User::User()
{
}

QString User::getUsername() const
{
    return m_username;
}

void User::setUsername(const QString &username)
{
    m_username = username;
}

QString User::getPassword() const
{
    return m_password;
}

void User::setPassword(const QString &password)
{
    m_password = password;
}

QString User::getEmail() const
{
    return m_email;
}

void User::setEmail(const QString &email)
{
    m_email = email;
}
