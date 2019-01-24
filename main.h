//
// Created by Dr. Brandon Wiley on 2019-01-15.
//

#ifndef U2F_AUTH_SERVER_CLI_MAIN_H
#define U2F_AUTH_SERVER_CLI_MAIN_H

int addUser(char *username);
int removeUser(char *username);
int setPassword(char *username, const char *password);
int enableU2F(char *username);
int disableU2F(char *username);
int enableSelfRegistration(char *username);
int disableSelfRegistration(char *username);
int registerWithResponse(char *username, char *userkeyfile, char *keyhandlefile);
int unregister(char *username);
int makeRequired(char *username);
int makeOptional(char *username);
int64_t getCurrentTimestamp();

#endif //U2F_AUTH_SERVER_CLI_MAIN_H
