//Saifulalh Shaik
// CS 333 Lab1
// 04/23/24
// 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>


#define LOGIN_LINE_LEN 1024
#define LOGON_INFO "last | grep %s"


void show_logon_info(const char *logname, int show_logon_lines);
void show_user_info(const char* logname, int show_logon, int logon_lines, int show_plan, int show_project, int verbose_level);

// This show_logon_info function is designed to display the logon information when using 'l' or 'L'.
void show_logon_info(const char *logname, int show_logon_lines) {
	char cmd[LOGIN_LINE_LEN] = {'\0'};
	char logon[LOGIN_LINE_LEN] = {'\0'};
	FILE *logon_info = NULL;
	int logon_count = 0;

	sprintf(cmd, LOGON_INFO, logname);
	logon_info = popen(cmd, "r");
	if (logon_info != NULL) {
		while (fgets(logon, LOGIN_LINE_LEN, logon_info) != NULL && logon_count < show_logon_lines) {
			fputs(logon, stdout);
			++logon_count;
		}
		pclose(logon_info);
		if (logon_count == 0) {
			printf("** No recent logon information. **\n");
		}
	} else {
		perror("could not popen() connection");
	}
}

// This show_user_info function is designed to display various information about a user based on the provided parameters.
void show_user_info(const char* logname, int show_logon, int logon_lines, int show_plan, int show_project, int verbose_level) {
	struct passwd *pw;

	char project_path[256];
	char line[1024];

	FILE *project_file = NULL;

	pw = getpwnam(logname);
	if (pw == NULL) {
		fprintf(stderr, "User entry not found for logname = %s\n", logname);
		return;
	}


    printf("Logname: %s\n", pw->pw_name); //Username
	printf("\tName: %s\n", pw->pw_gecos); //Full name or other identifying information
	printf("\tUID: %d\n", pw->pw_uid);  //User ID
        printf("\tGID: %d\n", (int)pw->pw_gid);  //Group ID
        printf("\tHome: %s\n", pw->pw_dir);  //Home directory
        printf("\tShell: %s\n", pw->pw_shell);  //Login shell

       //Displaying Login Information
	if (show_logon) {
		printf("Recent logon information:\n");
		show_logon_info(logname, logon_lines);
	}

       //Displaying Project Information
	if (show_project) {
		snprintf(project_path, sizeof(project_path), "%s/.project", pw->pw_dir);
		project_file = fopen(project_path, "r");
		if (project_file != NULL) {
			printf("Project:\n");
			while (fgets(line, sizeof(line), project_file) != NULL) {
				printf("    %s", line); 
			}
			fclose(project_file);
			if (line[strlen(line) - 1] != '\n') {
				printf("\n");
			}
		} else {

			if (!(show_plan && access(project_path, F_OK) != -1)) {
				printf("Project: N/A\n");
			}
		} 
		}
        

       //Displaying Plan Information

        if(show_plan){
        char path[1024];
        //char line[256]; // Move the declaration to the top of the block
        FILE *file = NULL; // Initialize the pointer to NULL here

        sprintf(path, "%s/%s",pw->pw_dir, ".plan");
        file = fopen(path, "r"); // Assign the file pointer here

        if (file == NULL) {
        printf("%s: N/A\n\n", ".Plan" + 1); // +1 to skip the dot in the file name, added newline for formatting
    } else {
        printf("%s:\n", ".Plan" + 1); // +1 to skip the dot in the file name
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
        fclose(file);
        printf("\n"); 
    }
     }

	}
	int main(int argc, char *argv[]) {
		int opt;
    int show_logon = 1; //Flags to show logon Info
    
    int logon_lines = 2; //Flag for Logon lines

    int show_plan = 1; //Flag indicating whether to show the .plan file.

    int show_project = 1; //Flag indicating whether to show the .project file.
    
    int verbose_level = 0; //Counter for verbose mode.
//Parsing Command-Line Arguments. It uses getopt to parse command-line options.
		while ((opt = getopt(argc, argv, "lL:pPvhY")) != -1) {
			switch (opt) {
				case 'l': //Don't show recent login information.
					show_logon = 0;
					break;
				case 'L': //Set the number of recent login records to show.
					logon_lines = strtol(optarg, NULL, 10);
					if (logon_lines < 0 || logon_lines > 1000) {
						logon_lines = 2;
					}
					else if(logon_lines == 0){
						show_logon = 0;}

					break;
				case 'p': //Don't show the .plan file.
					show_plan = 0;
					break;
				case 'P': //Don't show the .project file.
					show_project = 0;
					break;
				case 'v': // Enable verbose mode.
					verbose_level++;
					fprintf(stderr, "Verbose level: %d\n", verbose_level);
					break;
				
                case 'h': //Show help text and exit
                printf("Usage: %s [-hpPlLvY] [logname1] \n", argv[0]);
                printf("    -l     Do not show recent logon information\n");
                printf("    -L #   Number of recent logon records to show \n");
                printf("    -p     Do not show .plan \n");
                printf("    -P     Do not show .project \n");
                printf("    -v     Verbose mode\n");
                printf("    -h     Show help text and exit\n");
                printf("    -Y     You got this!!!\n");
                exit(EXIT_SUCCESS); 
				case 'Y': // show motivational message and exit
					printf("You got this!!!\n");
					return EXIT_SUCCESS;
				default: //Option Handling:
					fprintf(stderr, "Invalid option: -%c\n", optopt);
					return EXIT_FAILURE;
			}
		}

		for (int i = optind; i < argc; i++) {
			show_user_info(argv[i], show_logon, logon_lines, show_plan, show_project, verbose_level);
			printf("\n"); 
		}

		return EXIT_SUCCESS;
	}


