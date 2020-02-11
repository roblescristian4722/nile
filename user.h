#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    void setPassword(const QString &password);
    void setUsername(const QString &username);
    void setEmail(const QString &email);

    QString getEmail() const;
    QString getUsername() const;
    QString getPassword() const;

private:
    QString m_username;
    QString m_password;
    QString m_email;
};

#endif // USER_H
