#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

struct Student {
    int id;
    char name[50];
    int year;
    int grades;
    char course[50];
};

void to_lower_str(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void search_students(struct Student *students, int student_count){
    char guess[50];
    
    if(student_count == 0){
        printf("No students yet!\n");
        return;
    }

    printf("Enter students name: \n");
    while(getchar()!= '\n');
    fgets(guess, sizeof(guess), stdin);
    
    size_t len = strlen(guess);
    if (len > 0 && guess[len - 1] == '\n') {
        guess[len - 1] = '\0';
    }

    to_lower_str(guess);
    
    int found = 0;
    char year_str[10], id_str[10], grades_str[10];
    

    printf("Results of search: \n");

    for(int i = 0; i < student_count; i++){
        sprintf(year_str, "%d", students[i].year);
        sprintf(id_str, "%d", students[i].id);
        sprintf(grades_str, "%d", students[i].grades);

        char name_copy[50];
        strcpy(name_copy, students[i].name);
        to_lower_str(name_copy);

        char course_copy[50];
        strcpy(course_copy, students[i].course);
        to_lower_str(course_copy);

        if(strstr(name_copy, guess) || strstr(course_copy, guess) || strstr(grades_str, guess) || strstr(id_str, guess) || strstr(year_str, guess)){
             printf("ID: %d | Name: %s | Course: %s | Year: %d | Grade: %d\n",
                   students[i].id, students[i].name,students[i].course, students[i].year, students[i].grades);
            found = 1;
        }
    }
    if(!found){
        printf("Don`t find any students\n");
    }
};

void save_info_csv(struct Student *students, int student_count){
    FILE *file_student = fopen("file_student.csv", "w");

    if(!file_student){
        printf("Error, cant opent file for saving\n");
        return;
    }

    fprintf(file_student, "ID,Name,Course,Year,Grades\n");

    for(int i = 0; i < student_count; i++){
        fprintf(file_student, "%d,\"%s\",\"%s\",%d,%d\n", students[i].id, students[i].name,students[i].course, students[i].year, students[i].grades);
    }
    fclose(file_student);
};

void load_info(struct Student **students, int *student_count){
    FILE *file_students = fopen("file_students.txt", "r");

    if (!file_students) {
    printf("No saved students found.\n");
    return;
    }

    struct Student temp;

    while(fscanf(file_students, "%d | \"%49[^\"]\" | \"%49[^\"]\" | %d | %d\n" ,&temp.id, temp.name, temp.course, &temp.year, &temp.grades) == 5){
        *students = realloc(*students, ((*student_count) + 1) * sizeof(struct Student));

        (*students)[*student_count] = temp;

        (*student_count)++;
    }

    fclose(file_students);
    printf("Loaded %d students from file.\n", *student_count);
};

void save_info(struct Student *students, int student_count) {
    FILE *file_students = fopen("file_students.txt", "w");
    if(!file_students){
        printf("Error, cant opent file for saving\n");
        return;
    }
    for(int i = 0; i < student_count; i++){
        fprintf(file_students, "%d | \"%s\" | \"%s\" | %d | %d\n", students[i].id, students[i].name,students[i].course, students[i].year, students[i].grades);
    }

    fclose(file_students);
};

int get_id(int **arr_id, int *size_id){
    int id; 
    int unic;

    do{
        id = 1000 + rand() % 9000;
        unic = 1;
        for(int i = 0; i < *size_id; i++){
            if (id == (*arr_id)[i]){
                unic = 0;
                break;
            }
        } 
    } while (!unic);
    
    int *tmp = realloc(*arr_id, ((*size_id) + 1) * sizeof(int));
    if (!tmp){
        printf("Memory failed\n");
    }
    *arr_id = tmp;

    (*arr_id)[*size_id] = id;

    (*size_id)++;

    return id;
};

void get_student(int **arr_id, int *size_id, struct Student **students, int *student_count) {
    struct Student s;
    s.id = get_id(arr_id, size_id);

    printf("Enter students name: ");
    while (getchar() != '\n');  
    fgets(s.name, sizeof(s.name), stdin);
    size_t len = strlen(s.name);
    if (len > 0 && s.name[len - 1] == '\n') {
    s.name[len - 1] = '\0';
    }

    printf("Enter students course: "); 
    fgets(s.course, sizeof(s.course), stdin);
    size_t l = strlen(s.course);
    if (l > 0 && s.course[l - 1] == '\n') {
    s.course[l - 1] = '\0';
    }

    printf("Enter students year: ");
    scanf("%d", &s.year);
    
    printf("Enter students grades: ");
    scanf("%d", &s.grades);

    *students = realloc(*students, ((*student_count) + 1) * sizeof(struct Student));
    if (*students == NULL) {
    printf("Memory allocation failed!\n");
    return;
    }

    (*students)[*student_count] = s;
    (*student_count)++;

    save_info(*students, *student_count);
    save_info_csv(*students, *student_count);
};

void show_students(struct Student *students, int student_count){
    if(student_count == 0){
        printf("No students yet!\n");
    }
    for(int i = 0; i < student_count; i++){
        printf("Student %d\n", (i+1));
        printf("ID: %d\nName: %s\nCourse: %s\nYear: %d\nGrade: %d\n\n",students[i].id, students[i].name,students[i].course, students[i].year, students[i].grades);
    } 
};

void get_info(){
    printf("1. Add student\n");
    printf("2. Show all students\n");
    printf("3. Help\n");
    printf("4. Delete student\n");
    printf("5. Search student\n");
    printf("6. Exit\n");
};

void del_student(struct Student *students, int *student_count){
    int num_del;
    printf("Enter number of student which you want to dellite: \n");
    scanf("%d", &num_del);

    if(num_del < 1 || num_del > *student_count){
        printf("invalid number");
        return;
    }

    int index = num_del - 1;

    for(int i = index; i < *student_count - 1; i++){
        students[i] = students[i + 1];
    }
    (*student_count)--;
    save_info(students, *student_count);
    save_info_csv(students, *student_count);
    printf("Student dellited successful\n");
};

void dell_for_id(struct Student *students, int *student_count){
    int id_to_dell;
    int found = 0;

    printf("Enter ID to dellite: \n");
    scanf("%d", &id_to_dell);

    for(int i = 0; i < *student_count; i++){
        if(students[i].id == id_to_dell){
            for(int j = i; j < *student_count - 1; j++){
                students[j] = students[j + 1];
            }
            found = 1;
            (*student_count)--;
            break;
        }
    }
    if(!found){
        printf("No student found with ID %d.\n", id_to_dell);
        return;
    }
    save_info(students, *student_count);
    save_info_csv(students, *student_count);
    printf("Student dellited successful\n");
};

int main(){
    srand(time(NULL));
    int size_id = 0;
    int *arr_id = NULL;

    struct Student *students = NULL;
    int student_count = 0;

    load_info(&students, &student_count);

    int choice;
    printf("------------Students_Table------------\n");
    printf("1. Add student\n");
    printf("2. Show all students\n");
    printf("3. Help\n");
    printf("4. Delete student\n");
    printf("5. Search student\n");
    printf("6. Exit\n");
    do
    {
       
        printf("Choose an option: \n");
        if(scanf("%d", &choice) != 1){
            printf("Invalid number\n");

            while(getchar() != '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
            get_student(&arr_id, &size_id, &students, &student_count);
            break;
        case 2:
            show_students(students, student_count);
            break;
        case 3:
            get_info();  
            break;
        case 4:
            dell_for_id(students, &student_count);
            break;
        case 5:
            search_students(students, student_count);
            break;
        case 6:
            printf("-----------Closing-program-----------\n");
            break;
        default:
            printf("Invalid choice. Try again\n");
            break;
        }

    } while (choice != 6);
    free(arr_id);
    free(students);

    return 0;
}