#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


#define NR_OF_TESTS 10
#define true 1
#define false 0

// A utility function to reverse a string
void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}
// Implementation of citoa()
char* citoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}


int main() {
	const char *path_to_tests_dir = "../tests/test";
	srand(time(NULL));
	int packs_weight[100];
	int cap_and_nr_packs[2];

    for (int i = 0; i < NR_OF_TESTS; i++) {
        char filename[32];
        char filepath[64];

        citoa(i, filename, 10);
        strcat(filename, ".txt");
        strcpy(filepath, path_to_tests_dir);
        strcat(filepath, filename);

		int fd = open(filepath, O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (fd < 0) {
			perror("open");
			return 1;
		}
		int upper_bound_cap = 1e6;
		int lower_bound_cap = 1;

		int upper_bound_nrp = 25;
		int lower_bound_nrp = 5;

		int nr_of_packs = rand() % upper_bound_nrp + lower_bound_nrp;
		int capacity = rand() % upper_bound_cap + lower_bound_cap;

		cap_and_nr_packs[0] = nr_of_packs;
		cap_and_nr_packs[1] = capacity;

		dprintf(fd, "%d %d\n", nr_of_packs, capacity);
		for(int j = 0; j < nr_of_packs; j ++) {
			packs_weight[j] = rand() % capacity + lower_bound_nrp;
			dprintf(fd,"%d ", packs_weight[j]);
		}
		//close(fd);
		//write(fd, (void *)p)
    }
	return 0;
}