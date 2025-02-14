#ifndef SNMP_ENUM_H
#define SNMP_ENUM_H
#include "snmp/snmp_module.h"
#ifdef __cplusplus
extern          "C" {
#endif

    struct snmp_enum_list {
        struct snmp_enum_list *next;
        int             value;
        char           *label;
    };
#ifdef MOD_SNMP_VER_0_1
struct snmp_enum_list_str {
    char           *name;
    struct snmp_enum_list *list;
    struct snmp_enum_list_str *next;
};
#endif

#define SE_MAX_IDS 5
#define SE_MAX_SUBIDS 32        /* needs to be a multiple of 8 */

    /*
     * begin storage definitions 
     */
    /*
     * These definitions correspond with the "storid" argument to the API 
     */
#define SE_LIBRARY_ID     0
#define SE_MIB_ID         1
#define SE_APPLICATION_ID 2
#define SE_ASSIGNED_ID    3

    /*
     * library specific enum locations 
     */

    /*
     * error codes 
     */
#define SE_OK            0
#define SE_NOMEM         1
#define SE_ALREADY_THERE 2
#define SE_DNE           -2

    int             init_snmp_enum(const char *type);
    struct snmp_enum_list *se_find_list(unsigned int major,
                                        unsigned int minor);
    struct snmp_enum_list *se_find_slist(const char *listname);
    int             se_store_in_list(struct snmp_enum_list *,
                                     unsigned int major, unsigned int minor);
    int             se_find_value(unsigned int major, unsigned int minor,
                                  char *label);
    int             se_find_free_value(unsigned int major, unsigned int minor);
    char           *se_find_label(unsigned int major, unsigned int minor,
                                  int value);
    int             se_add_pair(unsigned int major, unsigned int minor,
                                char *label, int value);

    /*
     * finds a list of enums in a list of enum structs associated by a name. 
     */
    /*
     * find a list, and then operate on that list
     *   ( direct methods further below if you already have the list pointer)
     */
    char           *se_find_label_in_slist(const char *listname,
                                           int value);
    int             se_find_value_in_slist(const char *listname,
                                           char *label);
    int             se_find_free_value_in_slist(const char *listname);
    int             se_add_pair_to_slist(const char *listname, char *label,
                                         int value);

    /*
     * operates directly on a possibly external list 
     */
    char           *se_find_label_in_list(struct snmp_enum_list *list,
                                          int value);
    int             se_find_value_in_list(struct snmp_enum_list *list,
                                          char *label);
    int             se_find_free_value_in_list(struct snmp_enum_list *list);
    int             se_add_pair_to_list(struct snmp_enum_list **list,
                                        char *label, int value);

    /*
     * Persistent enumeration lists
     */
    void            se_store_enum_list(struct snmp_enum_list *new_list,
                                       const char *token, char *type);
    void            se_store_list(unsigned int major, unsigned int minor, char *type);
    void            se_store_slist(const char *listname, char *type);
    int             se_store_slist_callback(int majorID, int minorID,
                                           void *serverargs, void *clientargs);
    void            se_read_conf(const char *word, char *cptr);
    void            clear_snmp_enum(void);

#ifdef __cplusplus
}
#endif
#endif                          /* SNMP_ENUM_H */
