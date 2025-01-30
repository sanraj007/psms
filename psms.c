#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX 100
#define OFFICER_PASSWORD "admin123"
#define RESET_PASSWORD "reset456"
int t =0;
int* tt = &t;

struct Complaint {
    char department[50];
    char name[50];
    char phone[20];
    char panchayat[50];
    char address[100];
    char email[50];
    char complaint[200];
    char reply[200];
};

void submitComplaint();
void readReply();
void officerSection();
void viewComplaints();
void replyToComplaint();
void deleteComplaint();
void resetComplaints();

int main() {
    int choice;

    while (1) {

        printf("\nHiiii, Welcome to Public Service Management System\n");
        printf("1. Officer Section\n");
        printf("2. Public Section\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                officerSection();
                break;
            case 2:
                printf("\nPublic Section\n");
                printf("1. Submit a Complaint\n");
                printf("2. Read Reply\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                getchar();
                if (choice == 1)
                    submitComplaint();
                else if (choice == 2)
                    readReply();
                else
                    printf("Invalid choice.\n");
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

// Function for Public to Submit a Complaint
void submitComplaint() {
    struct Complaint c;
    FILE *file = fopen("complaints.dat", "ab");

    printf("\nSelect Department (Water/Electrical/Drainage): ");
    fgets(c.department, sizeof(c.department), stdin);
    strtok(c.department, "\n");

    printf("Enter Name: ");
    fgets(c.name, sizeof(c.name), stdin);
    strtok(c.name, "\n");

    printf("Enter Phone Number: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    strtok(c.phone, "\n");

    printf("Enter Panchayat: ");
    fgets(c.panchayat, sizeof(c.panchayat), stdin);
    strtok(c.panchayat, "\n");

    printf("Enter Address: ");
    fgets(c.address, sizeof(c.address), stdin);
    strtok(c.address, "\n");

    printf("Enter Email: ");
    fgets(c.email, sizeof(c.email), stdin);
    strtok(c.email, "\n");

    printf("Enter Complaint: ");
    fgets(c.complaint, sizeof(c.complaint), stdin);
    strtok(c.complaint, "\n");

    strcpy(c.reply, "No reply yet.");

    fwrite(&c, sizeof(struct Complaint), 1, file);
    fclose(file);
    printf("Complaint submitted successfully!\n");
}

// Function for Public to Read Officer's Reply
void readReply() {
    char email[50];
    struct Complaint c;
    int found = 0;
    FILE *file = fopen("complaints.dat", "rb");

    if (file == NULL) {
        printf("No complaints found.\n");
        return;
    }

    printf("Enter your Email to check reply: ");
    fgets(email, sizeof(email), stdin);
    strtok(email, "\n");

    while (fread(&c, sizeof(struct Complaint), 1, file))
        {
        if (strcmp(email, c.email) == 0) {
            printf("\nComplaint: %s\nReply: %s\n", c.complaint, c.reply);
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found && *tt==0 ){

        printf("No reply found for this email.\n");
    }

    else if(!found && *tt==1 ){

        printf("Complaint is verified and deleted\n");
    }

}

// Function for Officer Section
void officerSection() {
    char password[20];
    printf("\nEnter Officer Password: ");
    fgets(password, sizeof(password), stdin);
    strtok(password, "\n");

    if (strcmp(password, OFFICER_PASSWORD) != 0) {
        printf("Incorrect password!\n");
        return;
    }

    int choice;
    while (1) {
        printf("\nOfficer Section\n");
        printf("1. View Complaints\n");
        printf("2. Reply to Complaint\n");
        printf("3. Delete Complaint\n");
        printf("4. Reset All Complaints\n");
        printf("5. Exit Officer Section\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                viewComplaints();
                break;
            case 2:
                replyToComplaint();
                break;
            case 3:
                deleteComplaint();
                break;
            case 4:
                resetComplaints();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// Function for Officer to View Complaints
void viewComplaints() {
    struct Complaint c;
    FILE *file = fopen("complaints.dat", "rb");

    if (file == NULL) {
        printf("No complaints available.\n");
        return;
    }

    printf("\nAll Complaints:\n");
    while (fread(&c, sizeof(struct Complaint), 1, file)) {
        printf("\nDepartment: %s\nName: %s\nPhone: %s\nPanchayat: %s\nAddress: %s\nEmail: %s\nComplaint: %s\nReply: %s\n",
               c.department, c.name, c.phone, c.panchayat, c.address, c.email, c.complaint, c.reply);
    }

    fclose(file);
}

// Function for Officer to Reply to a Complaint
void replyToComplaint() {
    struct Complaint c;
    FILE *file = fopen("complaints.dat", "rb+");
    char email[50];
    int found = 0;

    if (file == NULL) {
        printf("No complaints found.\n");
        return;
    }

    printf("Enter Email to reply: ");
    fgets(email, sizeof(email), stdin);
    strtok(email, "\n");

    while (fread(&c, sizeof(struct Complaint), 1, file)) {
        if (strcmp(email, c.email) == 0) {
            printf("Enter reply: ");
            fgets(c.reply, sizeof(c.reply), stdin);
            strtok(c.reply, "\n");

            fseek(file, -sizeof(struct Complaint), SEEK_CUR);
            fwrite(&c, sizeof(struct Complaint), 1, file);
            printf("Reply sent successfully!\n");
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found)
        printf("Complaint not found.\n");
}

// Function for Officer to Delete Complaint
void deleteComplaint() {

    struct Complaint c;
    FILE *file = fopen("complaints.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    char email[50];
    int found = 0;

    if (file == NULL) {
        printf("No complaints found.\n");
        return;
    }

    printf("Enter Email to delete complaint: ");
    fgets(email, sizeof(email), stdin);
    strtok(email, "\n");

    while (fread(&c, sizeof(struct Complaint), 1, file)) {
        if (strcmp(email, c.email) != 0)
            fwrite(&c, sizeof(struct Complaint), 1, temp);
        else
            found = 1;
    }

    fclose(file);
    fclose(temp);
    remove("complaints.dat");
    rename("temp.dat", "complaints.dat");

    if (found){
        *tt =1;

        printf("Complaint deleted and email notified.\n");
    }
    else
        printf("Complaint not found.\n");
}

// Function for Officer to Reset All Complaints
void resetComplaints() {
    char password[20];
    printf("Enter Reset Password: ");
    fgets(password, sizeof(password), stdin);
    strtok(password, "\n");

    if (strcmp(password, RESET_PASSWORD) != 0) {
        printf("Incorrect reset password!\n");
        return;
    }

    remove("complaints.dat");
    printf("All complaints have been reset!\n");
}
