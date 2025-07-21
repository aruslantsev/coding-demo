#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int num_disks[3];
    int *disks_sizes[3];
} disks_positions;


void move_disk(int from_id, int to_id, disks_positions *disks)
{
    disks->disks_sizes[to_id][disks->num_disks[to_id]++] = disks->disks_sizes[from_id][--disks->num_disks[from_id]];
}


void print_disks(disks_positions *disks) {
    static int move_number = 0;
    printf("=============== Move %d =================\n", move_number);
    for (int i = 0; i < 3; i++) {
        printf("%d : ", i + 1);
        for (int j = 0; j < disks -> num_disks[i]; j++) {
            printf("%d ", disks -> disks_sizes[i][j]);
        }
        printf("\n");
    }
    move_number++;
}


void hanoi(int from_id, int to_id, int empty_id, int num_disks, disks_positions *disks, void (*show_state)(disks_positions *))
{
    if (num_disks > 1) {
        hanoi(from_id, empty_id, to_id, num_disks - 1, disks, show_state);
        move_disk(from_id, to_id, disks);
        if (show_state != NULL) {
            show_state(disks);
        }
        hanoi(empty_id, to_id, from_id, num_disks - 1, disks, show_state);
    } else {
        move_disk(from_id, to_id, disks);
        if (show_state != NULL)
        {
            show_state(disks);
        }
    }
    return;
}


int main() {
    int num_disks;
    void (*show_state)(disks_positions *) = print_disks;
    printf("Num disks: ");
    scanf("%d", &num_disks);
    disks_positions disks = {
        .num_disks = {num_disks, 0, 0},
        .disks_sizes = {
            (int *) malloc(sizeof(int) * num_disks),
            (int *) malloc(sizeof(int) * num_disks),
            (int *) malloc(sizeof(int) * num_disks)}
    };
    for (int disk_num = 0; disk_num < num_disks; disk_num++) {
        disks.disks_sizes[0][disk_num] = num_disks - disk_num;
    }

    if (show_state != NULL) {
        show_state(&disks);
    }
    hanoi(0, 1, 2, num_disks, &disks, show_state);

    for (int i = 0; i < 3; i++) {
        free(disks.disks_sizes[i]);
    }

    return 0;
}
