/*


                Birzeit University
            Department of Computer Science
                    COMP2421
            Data Structures And Algoritims 

            Project #1

            Student ID: 1201180

            Contact: contact@ibraheemalyan.dev
            Portfolio: https://www.ibraheemalyan.dev/

By
  ___ _               _                              _    _
 |_ _| |__  _ __ __ _| |__   ___  ___ _ __ ___      / \  | |_   _  __ _ _ __
  | || '_ \| '__/ _` | '_ \ / _ \/ _ \ '_ ` _ \    / _ \ | | | | |/ _` | '_ \
  | || |_) | | | (_| | | | |  __/  __/ | | | | |  / ___ \| | |_| | (_| | | | |
 |___|_.__/|_|  \__,_|_| |_|\___|\___|_| |_| |_| /_/   \_\_|\__, |\__,_|_| |_|
                                                            |___/


*/

/*

        Run the code in ANSI-compatible Terminal 
        e.g : cmd, powershell,  bash, zsh, macos terminal, iterm, iterm2

        Color Codes

Black: \033[0;30m
Red: \033[0;31m
Green: \033[0;32m
Yellow: \033[0;33m
Blue: \033[0;34m
Magenta: \033[0;35m
Cyan: \033[0;36m
White: \033[0;37m

        Style

Normal: \033[0;3?m
Bold:   \033[1;3?m


Reset: \033[0m

*/

// Author: Ibraheem Alyan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// /////////////////////////////////////////////////
// ////////////////////// Define Structs
// /////////////////////////////////////////////////

struct CourseObject
{
    char name[50];
    char id[10];
    int year;
    int current_seats;
    int max_seats;
    int waiting;
    int registration_index; // index in the registration array (of students' linked lists)
};

struct CourseNode
{
    struct CourseObject *obj_ptr;
    struct CourseNode *next;
};


typedef struct CourseNode *CourseLL;
typedef struct CourseNode CourseNode;
typedef struct CourseObject CourseObject;

struct StudentObject
{
    char name[50];
    int id;
    int registered_courses;
};

struct StudentNode
{
    struct StudentObject *obj_ptr;
    struct StudentNode *next;
};

typedef struct StudentNode *StudentLL;
typedef struct StudentNode StudentNode;
typedef struct StudentObject StudentObject;

// /////////////////////////////////////////////////
// ////////////////////// Prototypes
// /////////////////////////////////////////////////

void read_courses();
void read_students(StudentLL registration[], StudentLL waiting_list[]);
CourseObject * construct_course(char name[50], char id[10], int year, int max_seats);
CourseNode * construct_course_node(CourseObject * course);
void print_course(CourseNode *c);
StudentObject *construct_student(char name[50], int id);
void print_student(StudentObject *c);
CourseLL init_courses_linked_list();
void insert_course(CourseNode *to_append);
void insert_student(StudentObject *obj_ptr);
void insert_student_in_LL(StudentObject *new_obj, StudentLL header);
void print_courses_LL();
CourseNode *get_course_by_id(char id[10]);
int RegisterStudent(StudentObject *s, char course_id[10], StudentLL registration[], StudentLL waiting_list[]);
void print_list_of_LLs(StudentLL array[]);
void print_students_LL(StudentLL header);
void print_simple_students_LL(StudentLL header);

// /////////////////////////////////////////////////
// ////////////////////// Global Variables
// /////////////////////////////////////////////////

CourseLL courses;
StudentLL students;

int courses_count;
int students_count;

// /////////////////////////////////////////////////
// ////////////////////// Files Functions
// /////////////////////////////////////////////////

void read_courses()
{

    FILE *courses_file = fopen("Courses.txt", "r");

    char name[50];
    char id[10];
    int year;
    int max_seats;

    while (fscanf(courses_file, "\n%[^,],%[^,],%d,%d", name, id, &year, &max_seats) == 4)
    {

        CourseObject * obj = construct_course(name, id, year, max_seats);
        CourseNode * c = construct_course_node(obj);

        insert_course(c);
    }

    fclose(courses_file);
}

void read_students(StudentLL registration[], StudentLL waiting_list[])
{

    FILE *students_file = fopen("Students.txt", "r");

    char name[50];
    int id;

    while (fscanf(students_file, "\n%[^#]#%d", name, &id) == 2)
    {
        StudentObject *s = construct_student(name, id);

        students_count++;

        char course_id[10];

        printf("\nRegistering Student with id:'\033[0;32m%d\033[0m'", s->id);

        while (fscanf(students_file, "#%[^#\n]", course_id) == 1)
        {
            RegisterStudent(s, course_id, registration, waiting_list);
        }

        printf("\n");

        insert_student(s);
    }

    fclose(students_file);
}

// /////////////////////////////////////////////////
// ////////////////////// Courses Functions
// /////////////////////////////////////////////////

CourseObject * construct_course(char name[50], char id[10], int year, int max_seats)
{

    CourseObject *res = (CourseObject *)malloc(sizeof(CourseObject));

    strcpy(res->name, name);
    strcpy(res->id, id);

    res->year = year;
    res->max_seats = max_seats;

    res->current_seats = 0;
    res->waiting = 0;

    return res;
}

CourseNode * construct_course_node(CourseObject * course)
{

    CourseNode *res = (CourseNode *)malloc(sizeof(CourseNode));

    res->obj_ptr = course;

    return res;
}

// TODO remove
void print_course_debug(CourseNode *c)
{
    printf("< Course index:'\033[0;31m%d\033[0m' id:'\033[0;31m%s\033[0m'  max seats:\033[1;34m%d\033[0m  year:\033[0;34m%d\033[0m\tcurrent seats:\033[1;34m%d\033[0m\twaiting:\033[1;34m%d\033[0m\tname:'\033[0;32m%s\033[0m'>", c->obj_ptr->registration_index, c->obj_ptr->id, c->obj_ptr->max_seats, c->obj_ptr->year, c->obj_ptr->current_seats, c->obj_ptr->waiting, c->obj_ptr->name);
}

void print_course(CourseNode *c)
{
    printf("| \033[0;33m%-6d\033[0m | \033[0;31m%-8s\033[0m | \033[1;34m%-3d\033[0m | \033[0;34m%-3d\033[0m | \033[1;36m%-8d\033[0m | \033[1;34m%-8d\033[0m | \033[0;32m%-50s\033[0m |\n", c->obj_ptr->registration_index, c->obj_ptr->id, c->obj_ptr->max_seats, c->obj_ptr->year, c->obj_ptr->current_seats, c->obj_ptr->waiting, c->obj_ptr->name);
}

void print_heading(char heading[])
{
    printf("\n\n-----------------------------------\n> \033[0;31m%s\033[0m \n-----------------------------------\n", heading);
}

// /////////////////////////////////////////////////
// ////////////////////// Student Functions
// /////////////////////////////////////////////////

StudentObject *construct_student(char name[50], int id)
{

    StudentObject *res = (StudentObject *)malloc(sizeof(StudentObject));

    strcpy(res->name, name);

    res->id = id;
    res->registered_courses = 0;

    return res;
}

void print_student_(StudentObject *s)
{
    printf("< Student  id:'\033[0;33m%d\033[0m' name:'\033[0;36m%s\033[0m' # registered_courses:'\033[0;32m%d\033[0m' >", s->id, s->name, s->registered_courses);
}

void print_student(StudentObject *s)
{
    printf("| \033[0;33m%-6d\033[0m | \033[0;34m%-3d\033[0m | \033[0;32m%-50s\033[0m |\n", s->id, s->registered_courses, s->name);
}

// /////////////////////////////////////////////////
// ////////////////////// Courses LinkedList Functions
// /////////////////////////////////////////////////

CourseLL init_courses_linked_list()
{

    CourseNode *header = (CourseLL)malloc(sizeof(CourseNode));

    header->next = NULL;

    courses_count = 0;

    return header;
}

// inserts course such that the linked list is sorted ascending by ID (reduces search complexity)
void insert_course(CourseNode *to_append)
{

    CourseNode *current = courses;

    int added = 0;
    int i = 0;

    while (current->next)
    {
        if (strcmp((current->next->obj_ptr->id), (to_append->obj_ptr->id)) > 0)
        {

            to_append->next = current->next;
            current->next = to_append;
            added = 1;
            break;
        }

        i++;

        current = current->next;
    }

    if (!added)
    {

        current->next = to_append;

        to_append->next = NULL;
    }

    to_append->obj_ptr->registration_index = courses_count;

    courses_count++;
}

// inserts course such that the linked list is sorted ascending by Name
void insert_course_by_name(CourseLL header, CourseObject *obj)
{

    CourseNode * new_node = construct_course_node(obj);
    
    CourseLL current = header;

    int added = 0;

    while (current->next)
    {
        if (strcmp((current->next->obj_ptr->name), (new_node->obj_ptr->name)) > 0)
        {

            new_node->next = current->next;
            current->next = new_node;
            added = 1;
            break;
        }

        current = current->next;
    }

    if (!added)
    {

        current->next = new_node;

        new_node->next = NULL;
    }

}

void print_courses_LL()
{

    CourseNode *current = courses;

    printf("\n\n\nLinked List > ");

    while (current->next != NULL)
    {
        printf("\n\t|\n\t\\/\n");
        print_course((current->next));

        current = current->next;
    }
}

// /////////////////////////////////////////////////
// ////////////////////// Students LinkedList Functions
// /////////////////////////////////////////////////

StudentLL init_students_linked_list()
{

    StudentNode *header = (StudentLL)malloc(sizeof(StudentNode));

    header->next = NULL;
    header->obj_ptr = NULL;

    students_count = 0;

    return header;
}

// inserts the student in a postion such that the linked list is sorted ascending by ID
void insert_student(StudentObject *new_obj)
{

    StudentNode *new_node_ptr = (StudentNode *)malloc(sizeof(StudentNode));
    ;

    new_node_ptr->obj_ptr = new_obj;

    StudentNode *current = students;

    int added = 0;

    while (current->next)
    {
        if ((current->next->obj_ptr->id) > (new_node_ptr->obj_ptr->id))
        {

            new_node_ptr->next = current->next;
            current->next = new_node_ptr;
            added = 1;
            break;
        }

        current = current->next;
    }

    if (!added)
    {

        current->next = new_node_ptr;

        new_node_ptr->next = NULL;
    }
}

// inserts the student in a postion such that the linked list is sorted ascending by ID
void insert_student_in_LL(StudentObject *new_obj, StudentLL header)
{

    StudentNode *new_node_ptr = (StudentNode *)malloc(sizeof(StudentNode));
    ;

    new_node_ptr->obj_ptr = new_obj;

    StudentNode *current = header;

    int added = 0;

    while (current->next)
    {
        if ((current->next->obj_ptr->id) > (new_node_ptr->obj_ptr->id))
        {

            new_node_ptr->next = current->next;
            current->next = new_node_ptr;
            added = 1;
            break;
        }

        current = current->next;
    }

    if (!added)
    {

        current->next = new_node_ptr;

        new_node_ptr->next = NULL;
    }
}

void print_students_LL(StudentLL header)
{

    StudentNode *current = header;

    printf("\n\nStudents Linked List > ");

    while (current->next != NULL)
    {
        printf("\n\t|\n\t\\/\n");
        print_student((current->next->obj_ptr));

        current = current->next;
    }
}

void print_simple_students_LL(StudentLL header)
{

    StudentNode *current = header;

    while (current->next != NULL)
    {
        printf(" -- > %d", current->next->obj_ptr->id);

        current = current->next;
    }
}

void print_registration(StudentLL registration[])
{
    print_list_of_LLs(registration);
}

void print_waiting_list(StudentLL waiting_list[])
{
    print_list_of_LLs(waiting_list);
}

void print_sorted_courses_table_by_id()
{

    CourseNode *current = courses;

    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("\033[0;33m  index\033[0m   \033[0;31m    ID    \033[0m \033[1;34m Max \033[0m \033[0;34m Year \033[0m \033[1;36mCurrent\033[0m \033[1;34m   Waiting\033[0m             \033[0;32mName\033[0m                                      |\n");
    printf("------------------------------------------------------------------------------------------------------------\n");

    while (current->next != NULL)
    {
        print_course((current->next));
        current = current->next;
    }

    printf("------------------------------------------------------------------------------------------------------------\n");
}

void print_sorted_courses_table_by_name()
{

    CourseLL new_ll = init_courses_linked_list();
;
    CourseNode *current = courses;

    // Sort
    while (current->next != NULL)
    {
        insert_course_by_name(new_ll, current->next->obj_ptr);
        current = current->next;
    }

    current = new_ll;

    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("\033[0;33m  index\033[0m   \033[0;31m    ID    \033[0m \033[1;34m Max \033[0m \033[0;34m Year \033[0m \033[1;36mCurrent\033[0m \033[1;34m   Waiting\033[0m             \033[0;32mName\033[0m                                      |\n");
    printf("------------------------------------------------------------------------------------------------------------\n");

    while (current->next != NULL)
    {
        print_course((current->next));
        current = current->next;
    }

    printf("------------------------------------------------------------------------------------------------------------\n");
}

void print_sorted_courses_table_less_than_5()
{

    CourseNode *current = courses;

    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("\033[0;33m  index\033[0m   \033[0;31m    ID    \033[0m \033[1;34m Max \033[0m \033[0;34m Year \033[0m \033[1;36mCurrent\033[0m \033[1;34m   Waiting\033[0m             \033[0;32mName\033[0m                                      |\n");
    printf("------------------------------------------------------------------------------------------------------------\n");

    while (current->next != NULL)
    {
        if (current->next->obj_ptr->current_seats < 5)
        {
            print_course((current->next));
        }

        current = current->next;
    }

    printf("------------------------------------------------------------------------------------------------------------\n");
}

void print_course_students_table(CourseNode *course, StudentLL registration[], StudentLL waiting_list[])
{

    printf("\n\n\n\033[1;31m----------------------------------------------------------------------");
    printf("\n----------------------------------------------------------------------\033[0m \n\n\n");

    if (registration[course->obj_ptr->registration_index]->next == NULL)
    {
        printf("Course \033[1;32m%s\033[0m has no registered students", course->obj_ptr->id);
        return;
    }

    printf("Course \033[1;32m%s\033[0m registered students:\n", course->obj_ptr->id);

    printf("----------------------------------------------------------------------\n");
    printf("\033[0;33m ID      \033[0m \033[0;34m #reg \033[0m           \033[0;32mName\033[0m                                      |\n");
    printf("----------------------------------------------------------------------\n");

    StudentNode *current = registration[course->obj_ptr->registration_index];

    while (current->next != NULL)
    {
        print_student(current->next->obj_ptr);

        current = current->next;
    }
    printf("----------------------------------------------------------------------\n");

    if (waiting_list[course->obj_ptr->registration_index]->next == NULL)
    {
        return;
    }

    printf("\nWaiting list\n");

    printf("----------------------------------------------------------------------\n");
    printf("\033[0;33m ID      \033[0m \033[0;34m #reg \033[0m           \033[0;32mName\033[0m                                      |\n");
    printf("----------------------------------------------------------------------\n");

    current = waiting_list[course->obj_ptr->registration_index];

    while (current->next != NULL)
    {
        print_student(current->next->obj_ptr);

        current = current->next;
    }
    printf("----------------------------------------------------------------------\n");
}

void print_sorted_students_tables(StudentLL registration[], StudentLL waiting_list[])
{

    CourseNode *current = courses;

    while (current->next != NULL)
    {
        print_course_students_table(current->next, registration, waiting_list);
        current = current->next;
    }

    printf("\n\n\n");
}

void init_array_of_students_LLs(StudentLL registration[], int courses_count)
{

    for (size_t i = 0; i < courses_count; i++)
    {
        registration[i] = init_students_linked_list();
    }
}

// /////////////////////////////////////////////////
// ////////////////////// Search Functions
// /////////////////////////////////////////////////

CourseNode *get_course_by_id(char id[10])
{

    CourseNode *current = courses;

    while (current->next != NULL)
    {

        // measure complexity
        // printf("n");

        int cmp = strcmp(id, current->next->obj_ptr->id);

        if (cmp < 0)
        {

            return NULL;
        }
        else if (cmp == 0)
        {
            return current->next;
        }

        current = current->next;
    }

    return NULL;
}

StudentNode *get_student_by_id_from_course(StudentLL header, int id)
{

    StudentNode *current = header;

    while (current->next != NULL)
    {

        if (id < current->next->obj_ptr->id)
        {

            return NULL;
        }
        else if (id == current->next->obj_ptr->id)
        {
            return current->next;
        }

        current = current->next;
    }

    return NULL;
}

void search_for_course_and_print_students(StudentLL registration[], StudentLL waiting_list[])
{

    printf("Enter Course ID : ");

    char id[10];
    scanf("%s", id);

    CourseNode *course = get_course_by_id(id);

    if (course == NULL)
    {
        printf("\n\033[0;31mCourse '\033[0;35m%s\033[0;31m' Was not found\033[0m\n", id);
        return;
    }

    print_course_students_table(course, registration, waiting_list);
}

void search_for_student_in_course(StudentLL registration[])
{

    printf("Enter Course ID : ");

    char course_id[10];
    scanf("%s", course_id);

    CourseNode *course = get_course_by_id(course_id);

    if (course == NULL)
    {
        printf("\n\033[0;31mCourse '\033[0;35m%s\033[0;31m' Was not found\033[0m\n", course_id);
        return;
    }

    int student_id;

    printf("Enter Student ID (int): ");

    scanf("%d", &student_id);

    StudentNode *s = get_student_by_id_from_course(registration[course->obj_ptr->registration_index], student_id);

    if (s == NULL)
    {
        printf("\n\033[0;31mStudent with id \033[0;35m%d\033[0;31m was not found\033[0m\n", student_id);
        return;
    }

    printf("\n\n >>");

    print_student(s->obj_ptr);
}

// /////////////////////////////////////////////////
// ////////////////////// Registration Functions
// /////////////////////////////////////////////////

int can_register(int student_id, int course_year)
{

    // e.g , student_id = 1154567, course year = 115
    if ((student_id / 10000) <= course_year)
    {
        // can register
        return 1;
    }

    // cannot register
    return 0;
}

int RegisterStudent(StudentObject *s, char course_id[10], StudentLL registration[], StudentLL waiting_list[])
{

    printf("\n-- %s: ", course_id);

    if (s->registered_courses >= 5)
    {
        printf("\033[0;31mcannot register, reached courses count limit (5)\033[0m");
        return 1;
    }

    CourseNode *course = get_course_by_id(course_id);

    if (course == NULL)
    {
        printf("\033[0;31mcourse is not offered by department\033[0m");
        return 1;
    }

    if (!can_register(s->id, course->obj_ptr->year))
    {
        printf("\033[0;31mcannot register, student must be from year %d or older\033[0m", course->obj_ptr->year);
        return 1;
    }

    if (course->obj_ptr->current_seats >= course->obj_ptr->max_seats)
    {
        insert_student_in_LL(s, waiting_list[course->obj_ptr->registration_index]);
        course->obj_ptr->waiting++;
        printf("\033[0;33mcourse is full, added student to waiting list\033[0m");
        return 1;
    }

    course->obj_ptr->current_seats++;

    s->registered_courses++;

    insert_student_in_LL(s, registration[course->obj_ptr->registration_index]);

    printf("\033[0;32mregistered\033[0m");

    return 0;
}

// /////////////////////////////////////////////////
// ////////////////////// Waiting List
// /////////////////////////////////////////////////

void print_list_of_LLs(StudentLL array[])
{

    for (int i = 0; i < courses_count; i++)
    {
        printf("\n course %d : ", i);
        print_simple_students_LL(array[i]);
    }
}

// /////////////////////////////////////////////////
// ////////////////////// Menu
// /////////////////////////////////////////////////

void show_menu(StudentLL registration[], StudentLL waiting_list[])
{

    while (1)
    {

        printf("\n\n\n\033[1;36m----------------------------------------------------------------------");
        printf("\n----------------------------------------------------------------------\033[0m \n\n\n");

        printf("\nAvailable Commands:\n\n");

        printf("\n\033[0;34m --- Debug --- \033[0m\n");
        printf("\033[0;31m1 \033[0m: Print Full Students LinkedList\n");
        printf("\033[0;31m2 \033[0m: Print Full Courses LinkedList\n");
        printf("\033[0;31m3 \033[0m: Print Registration Array of Students' LinkedLists (IDs only) \n");
        printf("\033[0;31m4 \033[0m: Print Waiting List Array of Students' LinkedLists (IDs only) \n");

        printf("\n\033[0;34m --- Reports --- \033[0m\n");
        printf("\033[0;31m5 \033[0m: Print Courses Table Sorted By Name\n");
        printf("\033[0;31m6 \033[0m: Print Courses Table Sorted By ID\n");
        printf("\033[0;31m7 \033[0m: Print Students Info In courses tables (ordered by student ID)\n");
        printf("\033[0;31m8 \033[0m: Print Courses With Less Than 5 Students (ordered by course ID)\n");

        printf("\n\033[0;34m --- Search --- \033[0m\n");
        printf("\033[0;31m9 \033[0m: Find Course & Print Its Students (based on course ID)\n");
        printf("\033[0;31m10 \033[0m: Search For Student In Course (based on student and course IDs) \n");

        printf("\n\033[0;31mq \033[0m: Quit\n");

        printf("\n > \033[0;36mEnter command number to excute : \033[0m");

        int command = 0;
        scanf("%d", &command);

        printf("\n\n");

        switch (command)
        {
        case 1:
            print_heading("Full Students LinkedList");
            print_students_LL(students);
            break;

        case 2:
            print_heading("Full Courses LinkedList");
            print_courses_LL();
            break;

        case 3:
            print_heading("Registration Array of Students' LinkedLists (IDs only)");
            print_registration(registration);
            break;

        case 4:
            print_heading("Waiting List Array of Students' LinkedLists (IDs only)");
            print_waiting_list(waiting_list);
            break;

        case 5:
            print_heading("Courses Table Sorted By Name");
            print_sorted_courses_table_by_name();
            break;

        case 6:
            print_heading("Courses Table Sorted By ID");
            print_sorted_courses_table_by_id();
            break;

        case 7:
            print_heading("Students Info In courses tables (ordered by student ID)");
            print_sorted_students_tables(registration, waiting_list);
            break;

        case 8:
            print_heading("Print Courses With Less Than 5 Students (ordered by course ID)");
            print_sorted_courses_table_less_than_5();
            break;

        case 9:
            print_heading("Find Course & Print Its Students");
            search_for_course_and_print_students(registration, waiting_list);
            break;

        case 10:
            print_heading("Search For Student In Course (based on student and course IDs)");
            search_for_student_in_course(registration);
            break;

        default:
            printf("\033[1;31minvalid command number, quitting ...\033[0m\n");
            printf("\n\n\n");
            exit(0);
        }
    }
}

// /////////////////////////////////////////////////
// ////////////////////// Main
// /////////////////////////////////////////////////

int main()
{

    printf("Running ...\n\n\n");
    printf("\033[1;032mBy\n  ___ _               _                              _    _                   \n |_ _| |__  _ __ __ _| |__   ___  ___ _ __ ___      / \\  | |_   _  __ _ _ __  \n  | || '_ \\| '__/ _` | '_ \\ / _ \\/ _ \\ '_ ` _ \\    / _ \\ | | | | |/ _` | '_ \\ \n  | || |_) | | | (_| | | | |  __/  __/ | | | | |  / ___ \\| | |_| | (_| | | | |\n |___|_.__/|_|  \\__,_|_| |_|\\___|\\___|_| |_| |_| /_/   \\_\\_|\\__, |\\__,_|_| |_|\n                                                            |___/              \033[0m");

    printf("\n\n\n\n\n\033[1;032mHIT ENTER\033[0m to read the data from the files and start the registration process  \n\n\n ");

    char trash;
    scanf("%c", &trash);

    courses = init_courses_linked_list();

    read_courses();

    StudentLL registration[courses_count];
    StudentLL waiting_list[courses_count];

    init_array_of_students_LLs(registration, courses_count);
    init_array_of_students_LLs(waiting_list, courses_count);

    students = init_students_linked_list();

    read_students(registration, waiting_list);

    printf("\n\n> Read \033[1;33m%d\033[0m students\n", students_count);
    printf("> Read \033[1;33m%d\033[0m courses\n\n\n", courses_count);

    show_menu(registration, waiting_list);

    return 0;
}
