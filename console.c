#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_MODELS 8

typedef struct BaseModel {
    char id[37];
    // Additional fields for BaseModel
} BaseModel;

typedef struct User {
    char id[37];
    // Additional fields for User
} User;

typedef struct Storage {
    BaseModel **objects;
    int count;
    int capacity;
} Storage;

Storage storage = {NULL, 0, 0};

void init_storage() {
    storage.capacity = 10;
    storage.objects = malloc(storage.capacity * sizeof(BaseModel *));
}

void save() {
    // In a real implementation, save to a file or database
}

char *generate_id() {
    // Generate a dummy ID, replace with a real UUID generator
    char *id = malloc(37);
    snprintf(id, 37, "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx");
    return id;
}

void create(const char *class_name) {
    if (strcmp(class_name, "BaseModel") == 0) {
        BaseModel *obj = malloc(sizeof(BaseModel));
        strcpy(obj->id, generate_id());
        if (storage.count == storage.capacity) {
            storage.capacity *= 2;
            storage.objects = realloc(storage.objects, storage.capacity * sizeof(BaseModel *));
        }
        storage.objects[storage.count++] = (BaseModel *)obj;
        save();
        printf("%s\n", obj->id);
    }
    // Repeat similar blocks for User, Amenity, City, Place, Review, State
}

void show(const char *class_name, const char *id) {
    for (int i = 0; i < storage.count; ++i) {
        if (strcmp(storage.objects[i]->id, id) == 0) {
            printf("Instance: %s\n", storage.objects[i]->id);
            return;
        }
    }
    printf("** no instance found **\n");
}

void destroy(const char *class_name, const char *id) {
    for (int i = 0; i < storage.count; ++i) {
        if (strcmp(storage.objects[i]->id, id) == 0) {
            free(storage.objects[i]);
            for (int j = i; j < storage.count - 1; ++j) {
                storage.objects[j] = storage.objects[j + 1];
            }
            storage.count--;
            save();
            return;
        }
    }
    printf("** no instance found **\n");
}

void list_all(const char *class_name) {
    for (int i = 0; i < storage.count; ++i) {
        printf("%s\n", storage.objects[i]->id);
    }
}

void update(const char *class_name, const char *id, const char *attr_name, const char *value) {
    for (int i = 0; i < storage.count; ++i) {
        if (strcmp(storage.objects[i]->id, id) == 0) {
            // Update logic here (in real scenario, based on attribute types)
            save();
            return;
        }
    }
    printf("** no instance found **\n");
}

void process_command(const char *input) {
    char *command = strdup(input);
    char *args[MAX_INPUT_SIZE];
    int arg_count = 0;
    char *token = strtok(command, " ");
    while (token) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    if (arg_count == 0) {
        return;
    }

    if (strcmp(args[0], "create") == 0) {
        if (arg_count < 2) {
            printf("** class name missing **\n");
        } else {
            create(args[1]);
        }
    } else if (strcmp(args[0], "show") == 0) {
        if (arg_count < 3) {
            printf("** class name or id missing **\n");
        } else {
            show(args[1], args[2]);
        }
    } else if (strcmp(args[0], "destroy") == 0) {
        if (arg_count < 3) {
            printf("** class name or id missing **\n");
        } else {
            destroy(args[1], args[2]);
        }
    } else if (strcmp(args[0], "all") == 0) {
        list_all(arg_count > 1 ? args[1] : "");
    } else if (strcmp(args[0], "update") == 0) {
        if (arg_count < 5) {
            printf("** missing arguments **\n");
        } else {
            update(args[1], args[2], args[3], args[4]);
        }
    } else if (strcmp(args[0], "quit") == 0) {
        free(command);
        exit(0);
    } else if (strcmp(args[0], "EOF") == 0) {
        free(command);
        exit(0);
    } else {
        printf("** unknown command **\n");
    }

    free(command);
}

int main() {
    char input[MAX_INPUT_SIZE];
    init_storage();

    printf("(hbnb) ");
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;  // Remove trailing newline
        process_command(input);
        printf("(hbnb) ");
    }

    return 0;
}
