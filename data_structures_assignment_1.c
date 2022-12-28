/*

Data Structures & Programming - 1st Homework

Written by: Ömer Hamza Bilgin
Date: December 9th, 2022

Big-O Notations:

	(A) - Display the student names in ascending order by ID's
	(B) - Display the student names in ascending order by first letter
	(C) - Display the student names in ascending order by faculty codes
	(D) - Delete a record by school number
	
	A, B, C are O(n) (linear). Because 3 different stacks are used to keep the data
	ordered in 3 different ways. And the sorting is done everytime an input is added
	to a stack, by insertion. 

	D is also O(n) (linear). Because searching for the to-be-deleted item is linear,
	and deletion (shifting the necessary elements in the stack) is also linear.

IMPORTANT NOTE:
	A lot of assumptions (such as regarding the input) were made on my part,
	since the homework description was not clear at all.

	Also, not a lot of commenting was done because the code is written pretty clearly.
	You can see that almost all of my function names are self-explanatory.

- Written in VIM -

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define MAX_STUDENTS 1000
#define MAX_NAME 20

// We define to different types of nodes. Int Node will hold the student number.
// And Letter Node will hold the rest.
// 1. Letter Node
typedef struct letter_node {
	char value;
	struct letter_node *next;
} letter_node;

// 2. Int Node
typedef struct int_node {
	int value;
	struct letter_node *next;
} int_node;

// And int_node is also called student_t to help the abstraction
typedef int_node student_t;

// Define stack as an array of student_t pointers with length MAX_STUDENTS
typedef student_t *stack[MAX_STUDENTS];

// Initialize 3 arrays to keep the data sorted in 3 different ways
stack students_ordered_by_id;
stack students_ordered_by_first_letter;
stack students_ordered_by_faculty_code;

// All stacks are initially empty
int top = -1;


// Main function prototypes
void input_students (void);
void list_students_by_id (void);
void list_students_by_first_letter (void);
void list_students_by_faculty_code (void);
void delete_student_by_student_number (void);

// Helper function prototypes
void print_student (student_t *student);
student_t *get_student (void);
bool add_student_to_students_ordered_by_id (student_t *new_student);
bool add_student_to_students_ordered_by_first_letter (student_t *new_student);
bool add_student_to_students_ordered_by_faculty_code (student_t *new_student);
void swap (student_t **stack, int i, int j);
int get_id_from_student (student_t *new_student);
char get_first_letter_from_student (student_t *new_student);
int get_faculty_code_from_student (student_t *new_student);
bool delete_student_by_student_number_from_stack(student_t **stack, int student_number);
bool is_student_in_stack (student_t **stack, student_t *given_student);

int main (int argc, char **argv) {

	int choice;
	while (true) {
		printf("\n ---------------- MENU ---------------\n\n");
		printf(" -1: Exit\n");
		printf("  0: Input students\n");
		printf("  1: List students by ID\n");
		printf("  2: List students by first letter\n");
		printf("  3: List students by faculty code\n");
		printf("  4: Delete student by student number\n\n");
		printf(" -------------------------------------\n");
		printf(" Choice: ");

		scanf("%d", &choice);
		
		switch (choice) {
			case 0:
				input_students();
				break;
			case 1:
				list_students_by_id();
				break;
			case 2:
				list_students_by_first_letter();
				break;
			case 3:
				list_students_by_faculty_code();
				break;
			case 4:
				delete_student_by_student_number();
				break;
			default:
				if (choice == -1) {
					return 1;
				}
				printf("Invalid input!\n");
				break;
		}
	}

	return EXIT_SUCCESS;
}

void delete_student_by_student_number (void) {
	printf("Student number to delete: "); 
	int student_number;
	scanf("%d", &student_number);
	
	if (delete_student_by_student_number_from_stack (students_ordered_by_id, student_number) && delete_student_by_student_number_from_stack (students_ordered_by_first_letter, student_number) && delete_student_by_student_number_from_stack (students_ordered_by_faculty_code, student_number)) {
		top--;
	}
}

// Return true is student with given student number is successfully deleted
bool delete_student_by_student_number_from_stack(student_t **stack, int given_student_number) {
	int current_student_number;
	for (int i=0; i<=top; i++) {
		current_student_number = stack[i]->value;
		if (current_student_number == given_student_number) { // Delete this particular student
			for (int j=i; j<top; j++) {
				stack[i] = stack[i+1];
			}
			return true;
		}
	}
	return false;
}

void list_students_by_id (void) {
	printf("  Listing students by ID...\n\n");
	for (int i=0; i<=top; i++) {
		printf("  %d)\t", i+1);
		print_student(students_ordered_by_id[i]);
	}
}

void list_students_by_first_letter (void) {
	printf("  Listing students by first letter...\n\n");
	for (int i=0; i<=top; i++) {
		printf("  %d)\t", i+1);
		print_student(students_ordered_by_first_letter[i]);
	}
}

void list_students_by_faculty_code (void) {
	printf("  Listing students by faculty code...\n\n");
	for (int i=0; i<=top; i++) {
		printf("  %d)\t", i+1);
		print_student(students_ordered_by_faculty_code[i]);
	}
}

void print_student (student_t *student) {
	// Print student number
	printf("%09d\t", student->value);

	// Print student name
	letter_node *current = student->next;
	while (current != NULL) {
		printf("%c", current->value);
		current = current->next;
	}
	
	printf("\n");
}

// This is the interface function which calls get_student() the necessary amount of times
// to get desired number of student inputs.
void input_students (void) {
	int student_count = -1;

	bool first_iteration = true;
	while (student_count < 0 || student_count > MAX_STUDENTS || student_count > (MAX_STUDENTS - top - 1)) {
		if (!first_iteration) {
			printf("Invalid input!\n");
		}
		printf("How many students to add? (%d room left): ", MAX_STUDENTS - top - 1);
		scanf("%d", &student_count);
		first_iteration = false;
	}
	
	student_t *new_student;
	for (int i=0; i<student_count; i++) {
		new_student = get_student();
		if (add_student_to_students_ordered_by_id(new_student) && add_student_to_students_ordered_by_first_letter(new_student) && add_student_to_students_ordered_by_faculty_code(new_student)) {
			top++;
		}
	}
}

// Gets name, surname and student number from user. Creates a new student structure with them.
// Returns the new student
student_t *get_student (void) {
	char name[MAX_NAME];
	char surname[MAX_NAME];
	int student_number;
	printf("Name: ");
	scanf("%s", name);
	printf("Surname: ");
	scanf("%s", surname);
	printf("Student number (9 digits): ");
	scanf("%d", &student_number);

	student_t *new_student = malloc(sizeof(student_t));
	new_student->value = student_number;
	new_student->next  = malloc(sizeof(letter_node));

	letter_node *current = new_student->next;
	// Adding the name
	for (int i=0, n=strlen(name); i<n; i++) {
		current->value = name[i];
		current->next  = malloc(sizeof(letter_node));
		current = current->next;
	}

	// Adding ' ' between name and surname
	current->value = ' ';
	current->next = malloc(sizeof(letter_node));

	current = current->next;
	
	// Adding the surname
	for (int i=0, n=strlen(surname); i<n; i++) {
		current->value = surname[i];
		current->next  = malloc(sizeof(letter_node));
		current = current->next;
	}

	return new_student;
}

// Returns true if given_student is in given stack
bool is_student_in_stack (student_t **stack, student_t *given_student) {
	for (int i=0; i<=top; i++) {
		if (given_student->value == stack[i]->value) {
			return true;
		}
	}
	return false;
}

// Returns true if student is successfully added to students_ordered_by_id
bool add_student_to_students_ordered_by_id (student_t *new_student) {
	if (is_student_in_stack(students_ordered_by_id, new_student)) {
		return false;
	}

	students_ordered_by_id[top+1] = new_student;
	int id_1, id_2;
	for (int i=top+1; i>0; i--) {
		id_1 = get_id_from_student(students_ordered_by_id[i]);
		id_2 = get_id_from_student(students_ordered_by_id[i-1]);
		if (id_1 < id_2) {
			swap (students_ordered_by_id, i, i-1);
		}
	}

	return true;
}

// Returns true if student is successfully added to students_ordered_by_first_letter
bool add_student_to_students_ordered_by_first_letter (student_t *new_student) {
	if (is_student_in_stack(students_ordered_by_first_letter, new_student)) {
		return false;
	}

	students_ordered_by_first_letter[top+1] = new_student;
	int first_letter_1, first_letter_2;
	for (int i=top+1; i>0; i--) {
		first_letter_1 = get_first_letter_from_student(students_ordered_by_first_letter[i]);
		first_letter_2 = get_first_letter_from_student(students_ordered_by_first_letter[i-1]);
		if (first_letter_1 < first_letter_2) {
			swap (students_ordered_by_first_letter, i, i-1);
		}
	}

	return true;
}

// Returns true if student is successfully added to students_ordered_by_faculty_code
bool add_student_to_students_ordered_by_faculty_code (student_t *new_student) {
	if (is_student_in_stack(students_ordered_by_faculty_code, new_student)) {
		return false;
	}

	students_ordered_by_faculty_code[top+1] = new_student;
	int faculty_code_1, faculty_code_2;
	for (int i=top+1; i>0; i--) {
		faculty_code_1 = get_faculty_code_from_student(students_ordered_by_faculty_code[i]);
		faculty_code_2 = get_faculty_code_from_student(students_ordered_by_faculty_code[i-1]);
		if (faculty_code_1 < faculty_code_2) {
			swap (students_ordered_by_faculty_code, i, i-1);
		}
	}

	return true;
}

int get_id_from_student (student_t *new_student) {
	int student_number = new_student->value;
	int id = student_number % 10000;
	return id;
}

char get_first_letter_from_student (student_t *new_student) {
	return new_student->next->value;
}

int get_faculty_code_from_student (student_t *new_student) {
	int student_number = new_student->value;
	int faculty_code = student_number / 1000000;
	return faculty_code;
}

// Swaps two elements with indices i & j in given stack
void swap (student_t **stack, int i, int j) {
	student_t *temp = stack[i];
	stack[i] = stack[j];
	stack[j] = temp;
}
