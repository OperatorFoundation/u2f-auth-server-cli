#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "u2fdbt.h"
#include "main.h"

int main(int argc, char *argv[]) {
    int aflag = 0;
    int rflag = 0;
    int pflag = 0;
    int eflag = 0;
    int dflag = 0;
    int sflag = 0;
    int nflag = 0;
    int gflag = 0;
    int kflag = 0;
    int uflag = 0;
    int qflag = 0;
    int oflag = 0;
    char *username = NULL;
    char *password = NULL;
    char *userkeyfile = NULL;
    char *keyhandlefile = NULL;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "arp:edsncg:k:uqo")) != -1) {
        switch (c) {
            // Add user
            case 'a':
                aflag = 1;
                break;
            // Remove user
            case 'r':
                rflag = 1;
                break;
            // Set password
            case 'p':
                pflag = 1;
                password = optarg;
                break;
            // enable U2F
            case 'e':
                eflag = 1;
                break;
            // disable U2F
            case 'd':
                dflag = 1;
                break;
            // enable self-registration
            case 's':
                sflag = 1;
                break;
            // disable self-registration
            case 'n':
                nflag = 1;
                break;
            // register key with challenge response
            case 'g':
                gflag = 1;
                userkeyfile = optarg;
                break;
                // register key with challenge response
            case 'k':
                kflag = 1;
                keyhandlefile = optarg;
                break;
            // unregister key
            case 'u':
                uflag = 1;
                break;
            // make U2F required
            case 'q':
                qflag = 1;
                break;
            // make U2F optional
            case 'o':
                oflag = 1;
                break;
            case '?':
                if (optopt == 'p')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort();
        }
    }

    if(aflag && rflag) {
        fprintf(stderr, "-a cannot be used with -r");
        abort();
    }

    if(eflag && dflag) {
        fprintf(stderr, "-e cannot be used with -d");
        abort();
    }

    if(dflag && sflag) {
        fprintf(stderr, "-d cannot be used with -s");
        abort();
    }

    if(dflag && gflag) {
        fprintf(stderr, "-d cannot be used with -g");
        abort();
    }

    if(dflag && qflag) {
        fprintf(stderr, "-d cannot be used with -q");
        abort();
    }

    if(sflag && nflag) {
        fprintf(stderr, "-s cannot be used with -n");
        abort();
    }

    if(gflag && uflag) {
        fprintf(stderr, "-g cannot be used with -u");
        abort();
    }

    if(gflag && oflag) {
        fprintf(stderr, "-g cannot be used with -o");
        abort();
    }

    if(gflag && !kflag) {
        fprintf(stderr, "-g requires -k");
        abort();
    }

    if(kflag && !gflag) {
        fprintf(stderr, "-k requires -g");
        abort();
    }

    if(qflag && oflag) {
        fprintf(stderr, "-q cannot be used with -o");
        abort();
    }

    if(optind == argc) {
        fprintf(stderr, "Username required.");
        abort();
    }

    username = argv[optind];

    // When disabling U2F, also disable self-registration, unregister key, and make U2F optional
    if(dflag) {
        sflag=0;
        nflag=1;

        gflag=0;
        uflag=1;

        qflag=0;
        oflag=1;
    }

    // When registering a key, also make U2F required
    if(gflag) {
        qflag=1;
        oflag=0;
    }

    if(aflag) {
        printf("Adding users...");
        if(addUser(username)) {
            printf("Added user %s.\n", username);
        } else {
            printf("Failed to add user %s.\n", username);
            abort();
        }
    }

    if(rflag) {
        if(removeUser(username)) {
            printf("Removed user %s.\n", username);
            return 0;
        } else {
            printf("Failed to remove user %s.\n", username);
            abort();
        }
    }

    if(pflag) {
        if(setPassword(username, password)) {
            printf("Set password for user %s.\n", username);
        } else {
            printf("Failed to set password user %s.\n", username);
            abort();
        }
    }

    if(eflag) {
        if(enableU2F(username)) {
            printf("Enabled U2F for user %s.\n", username);
        } else {
            printf("Failed to enable U2F for user %s.\n", username);
            abort();
        }
    }

    if(dflag) {
        if(disableU2F(username)) {
            printf("Disabled U2F for user %s.\n", username);
        } else {
            printf("Failed to disable U2F for user %s.\n", username);
            abort();
        }
    }

    if(sflag) {
        if(enableSelfRegistration(username)) {
            printf("Enabled self-registration for user %s.\n", username);
        } else {
            printf("Failed to enable self-registration for user %s.\n", username);
            abort();
        }
    }

    if(nflag) {
        if(disableSelfRegistration(username)) {
            printf("Disabled self-registration for user %s.\n", username);
        } else {
            printf("Failed to disable self-registration for user %s.\n", username);
            abort();
        }
    }

    if(gflag) {
        if(registerWithResponse(username, userkeyfile, keyhandlefile)) {
            printf("Registered key for user %s.\n", username);
        } else {
            printf("Failed to register key for user %s.\n", username);
            abort();
        }
    }

    if(uflag) {
        if(unregister(username)) {
            printf("Unregistered key for user %s.\n", username);
        } else {
            printf("Failed to unregister key for user %s.\n", username);
            abort();
        }
    }

    if(qflag) {
        if(makeRequired(username)) {
            printf("Made U2F required for user %s.\n", username);
        } else {
            printf("Failed to make U2F required for user %s.\n", username);
            abort();
        }
    }

    if(oflag) {
        if(makeOptional(username)) {
            printf("Made U2F optional for user %s.\n", username);
        } else {
            printf("Failed to make U2F optional for user %s.\n", username);
            abort();
        }
    }

    return 0;
}

int addUser(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record;

    record=(struct u2fdbt_Record *)malloc(sizeof(struct u2fdbt_Record));
    record->name=username;
    record->pw_digest="*";
    record->pw_mtime=0;
    record->record_mtime=getCurrentTimestamp();
    record->flags=U2FDBT_FLAG_DISABLED;
    record->unknown_flags="";

    u2fdbt_begin_update(db);
    if(u2fdbt_append_new(db, record))
    {
        u2fdbt_finish_update(db);
        u2fdbt_close(db);
        return 1;
    }
    else
    {
        // Cannot add user, as user already exists.
        u2fdbt_finish_update(db);
        u2fdbt_close(db);
        return 0;
    }
}

int removeUser(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");

    u2fdbt_begin_update(db);
    u2fdbt_delete_user(db, username);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int setPassword(char *username, const char *password) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot set password for user, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->pw_digest=(const char *)hash_password(password);
    record->pw_mtime=0;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int enableU2F(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags & ~U2FDBT_FLAG_DISABLED;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int disableU2F(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags | U2FDBT_FLAG_DISABLED;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int enableSelfRegistration(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags | U2FDBT_FLAG_SELF_REGISTER;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int disableSelfRegistration(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags & ~U2FDBT_FLAG_SELF_REGISTER;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int registerWithResponse(char *username, char *userkeyfile, char *keyhandlefile) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    FILE *ukf=fopen(userkeyfile, "rb");
    fseek(ukf, 0L, SEEK_END);
    unsigned int uksize = ftell(ukf);
    fseek(ukf, 0L, SEEK_SET);

    char *userkey=malloc(uksize);
    fread(userkey, uksize, 1, ukf);
    fclose(ukf);

    FILE *khf=fopen(keyhandlefile, "rb");
    fseek(khf, 0L, SEEK_END);
    unsigned int khsize = ftell(khf);
    fseek(khf, 0L, SEEK_SET);

    char *keyhandle=malloc(khsize+1);
    fread(keyhandle, khsize, 1, khf);
    keyhandle[khsize]='\0';
    fclose(khf);

    record->flags=record->flags | U2FDBT_FLAG_HAVE_KEYS;
    record->record_mtime=getCurrentTimestamp();
    record->userkey=userkey;
    record->userkeysize=uksize;
    record->keyhandle=keyhandle;

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int unregister(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags & ~U2FDBT_FLAG_HAVE_KEYS;
    record->record_mtime=getCurrentTimestamp();
    record->userkey=NULL;
    record->userkeysize=0;
    record->keyhandle=NULL;

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int makeRequired(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags | U2FDBT_FLAG_REQUIRED;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int makeOptional(char *username) {
    struct u2fdbt_File *db=u2fdbt_open("users.dbt");
    struct u2fdbt_Record *record=u2fdbt_find(db, username);

    // Cannot modify user record, as user does not exists.
    if(record==NULL) {
        u2fdbt_close(db);
        return 0;
    }

    record->flags=record->flags & ~U2FDBT_FLAG_REQUIRED;
    record->record_mtime=getCurrentTimestamp();

    u2fdbt_begin_update(db);
    u2fdbt_replace(db, record);
    u2fdbt_finish_update(db);
    u2fdbt_close(db);
    return 1;
}

int64_t getCurrentTimestamp() {
    struct timespec tms;

    if(clock_gettime(CLOCK_REALTIME, &tms)) {
        return -1;
    }

    /* seconds, multiplied with 1 million */
    int64_t micros = tms.tv_sec * 1000000;

    /* Add full microseconds */
    micros += tms.tv_nsec/1000;

    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }

    return micros;
}
