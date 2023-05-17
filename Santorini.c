#pragma GCC optimize ("O3")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct worker {
    int now_pos[2]; //工人現在位置座標(i和j)
    int now_floor; //工人現在樓層
    int sim_pos[2]; //模擬工人當下位置座標
    int sim_floor; //模擬工人當下樓層
    int sim_build_pos[2]; //模擬工人建造位置座標
    int color; //工人顏色
    int canmove; //模擬工人當下位置可移動格子數
    int canbuild; //模擬工人當下位置可建造格子數
    char name[10]; //Debug用 顯示是誰的工人
    int god[2]; //[0]: god是誰 / [1]: 有無啟動能力
};

int now_worker_pos[5][5]; //工人現在位置分布
int now_building_pos[5][5]; //建築現在位置分布
int sim_worker_pos[5][5]; //模擬工人位置分布
int sim_building_pos[5][5]; //模擬建築位置分布
long long int mywin[5][5][5][5][2]; //記錄各種組合贏的樣本數: 工人i座標 /工人j座標 /建築i座標 /建築j座標 /哪個工人
long long int mytried[5][5][5][5][2]; //記錄各種嘗試過的組合總樣本數
int which_worker = 0; //哪個工人做完第一步
int roundnum = 0; //從現在開始過幾回合可結束遊戲

void find_worker_valid_pos(struct worker[2], int); //尋找工人在模擬位置下是否能移動
void find_building_valid_pos(struct worker[2], int); //尋找工人在模擬位置下是否能建造
int sim_one_action(struct worker[2], char*[]); //模擬一玩家進行一回合的步驟
void sim_one_game(struct worker[2], struct worker[2], char*[]); //模擬一局
int verify_oppcan_threefloor_after_mybuild(struct worker[2], int, int, int, int); //判斷對方是否會在我建造完後直接上三樓
int *verify_must_defend(struct worker[2], struct worker[2]); //判斷我現在是否一定要防守對手上三樓
int verify_nowcan_block(struct worker[2], int, int, int); //判斷我現在移動過後 是否守得到對手上三樓
int verify_nowcan_win(struct worker[2]); //判斷我現在是否能直接站上三樓獲勝

int main(int argc, char *argv[]) {
    clock_t begin = clock();

    //讀檔
	FILE *ficolor = fopen(argv[5], "r");
	FILE *fistructure = fopen(argv[6], "r");
	int i = 0, j = 0;
	for(i = 0; i < 5; i++){
		fscanf(ficolor,"%d,%d,%d,%d,%d", &now_worker_pos[i][0], &now_worker_pos[i][1], &now_worker_pos[i][2], &now_worker_pos[i][3], &now_worker_pos[i][4]);
		fscanf(ficolor,"\n");
	}
	fclose(ficolor);
	for(i = 0; i < 5;i++){
		fscanf(fistructure,"%d,%d,%d,%d,%d", &now_building_pos[i][0], &now_building_pos[i][1], &now_building_pos[i][2], &now_building_pos[i][3], &now_building_pos[i][4]);
		fscanf(fistructure,"\n");
	}
	fclose(fistructure);

    struct worker myworker[2]; //我方兩位工人
    struct worker oppworker[2]; //敵方兩位工人
    int mycount = 0, oppcount = 0; //場上棋子數
    strncpy(myworker[0].name, "My", 10);
    strncpy(myworker[1].name, "My", 10);
    strncpy(oppworker[0].name, "Opp", 10);
    strncpy(oppworker[1].name, "Opp", 10);

    // 初始化棋子顏色
    if (!strcmp(argv[1], "1")) {
        myworker[0].color = 1; //己方黑色先手
        myworker[1].color = 1;
        oppworker[0].color = 2; //我方白色後手
        oppworker[1].color = 2;
    }
    else {
        oppworker[0].color = 1; //對方黑色先手
        oppworker[1].color = 1;
        myworker[0].color = 2; //我方白色後手
        myworker[1].color = 2;
    }
    //初始化worker內容
    for (i = 0; i < 2; i++) {
        myworker[i].canbuild = 0;
        myworker[i].canmove = 0;
        oppworker[i].canmove = 0;
        oppworker[i].canbuild = 0;
        
    }
    //初始化天神
    for (i = 0; i < 2; i++) {
        if (!strncmp(argv[2], "Zeus", 10)) myworker[i].god[0] = 0;
        else if (!strncmp(argv[2], "Demeter", 10)) myworker[i].god[0] = 1;
        else if (!strncmp(argv[2], "Triton", 10)) myworker[i].god[0] = 2;
        myworker[i].god[1] = 0; //預設尚未發動能力

        if (!strncmp(argv[3], "Zeus", 10)) oppworker[i].god[0] = 0;
        else if (!strncmp(argv[3], "Demeter", 10)) oppworker[i].god[0] = 1;
        else if (!strncmp(argv[3], "Triton", 10)) oppworker[i].god[0] = 2;
        myworker[i].god[1] = 0;
    }
    int k, l, m;
    //初始化勝率地圖
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            for (k = 0; k < 5; k++) {
                for(l = 0; l < 5; l++) {
                    for (m = 0; m < 2; m++) {
                        mywin[i][j][k][l][m] = 0;
                        mytried[i][j][k][l][m] = 0;
                    }
                }
            }
        }
    }
    //尋找目前我方棋子位置
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (now_worker_pos[i][j] == myworker[0].color) {
                mycount++;
                myworker[mycount - 1].now_pos[0] = i;
                myworker[mycount - 1].now_pos[1] = j;
                myworker[mycount - 1].now_floor = now_building_pos[i][j];
            }
        }
    }
    //尋找目前對方棋子位置
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (now_worker_pos[i][j] == oppworker[0].color) {
                oppcount++;
                oppworker[oppcount - 1].now_pos[0] = i;
                oppworker[oppcount - 1].now_pos[1] = j;
                oppworker[oppcount - 1].now_floor = now_building_pos[i][j];
            }
        }
    }
    srand(time(NULL));
    //若開局場上目前沒有我方棋子，分開放棋子
    if (!strncmp(argv[4], "Y", 10)) { 
        //若我先手
        if (!strncmp(argv[1], "1", 10)) {
            if (now_worker_pos[1][1] == 0 && now_worker_pos[3][3] == 0) { //case 1
                now_worker_pos[1][1] = myworker[0].color;
                now_worker_pos[3][3] = myworker[0].color;
                myworker[0].now_pos[0] = 1;
                myworker[0].now_pos[1] = 1;
                myworker[1].now_pos[0] = 3;
                myworker[1].now_pos[1] = 3;
            }
            else if (now_worker_pos[2][1] == 0 && now_worker_pos[2][3] == 0) { //case 2
                now_worker_pos[2][1] = myworker[0].color;
                now_worker_pos[2][3] = myworker[0].color;
                myworker[0].now_pos[0] = 2;
                myworker[0].now_pos[1] = 1;
                myworker[1].now_pos[0] = 2;
                myworker[1].now_pos[1] = 3;
            }
            else { //case 3
                now_worker_pos[1][3] = myworker[0].color;
                now_worker_pos[3][1] = myworker[0].color;
                myworker[0].now_pos[0] = 1;
                myworker[0].now_pos[1] = 3;
                myworker[1].now_pos[0] = 3;
                myworker[1].now_pos[1] = 1;
            }
        }
        //若我後手，兩隻棋子個別放在對手附近
        else {
            int p0_i = 0;
            int p0_j = 0;
            int p1_i = 0;
            int p1_j = 0;
            while(1) {
                p0_i = rand() % 3 + oppworker[0].now_pos[0] - 1;
                p0_j = rand() % 3 + oppworker[0].now_pos[1] - 1;
                p1_i = rand() % 3 + oppworker[1].now_pos[0] - 1;
                p1_j = rand() % 3 + oppworker[1].now_pos[1] - 1;
                if (now_worker_pos[p0_i][p0_j] == 0 && now_worker_pos[p1_i][p1_j] == 0 && p0_i >= 0 && p0_j >= 0 && p1_i >= 0 && p1_j >= 0 && p0_i < 5 && p0_j < 5 && p1_i < 5 && p1_j < 5 && (p0_i != p1_i || p0_j != p1_j)) {
                    now_worker_pos[p0_i][p0_j] = myworker[0].color;
                    now_worker_pos[p1_i][p1_j] = myworker[0].color;
                    myworker[0].now_pos[0] = p0_i;
                    myworker[0].now_pos[1] = p0_j;
                    myworker[1].now_pos[0] = p1_i;
                    myworker[1].now_pos[1] = p1_j;  
                    break;    
                }
            }
        }
    }
    else { //開始模擬
        for (i = 0; i < 150000; i++) { //隨機模擬150000局
            roundnum = 0;
            //開始模擬的地圖分布 將現有地圖作為模擬起點
            for (j = 0; j < 5; j++) {
                for (k = 0; k < 5; k++){
                    sim_worker_pos[j][k] = now_worker_pos[j][k];
                    sim_building_pos[j][k] = now_building_pos[j][k];
                }
            }
            for (l = 0; l < 2; l++) {
                myworker[l].sim_pos[0] = myworker[l].now_pos[0];
                myworker[l].sim_pos[1] = myworker[l].now_pos[1];
                myworker[l].sim_floor = myworker[l].now_floor;
                oppworker[l].sim_pos[0] = oppworker[l].now_pos[0];
                oppworker[l].sim_pos[1] = oppworker[l].now_pos[1];
                oppworker[l].sim_floor = oppworker[l].now_floor;
            }
            //我方模擬隨機取得一格可以走的位置
                sim_one_game(myworker, oppworker, argv);
        }

        //尋找勝率地圖中勝率最高的組合
        int my_win_max[5];
        //最需要防守的位置座標
        int* must_defend_pos = verify_must_defend(oppworker, myworker);
        int must_defend_i = must_defend_pos[0];
        int must_defend_j = must_defend_pos[1];
        free(must_defend_pos);
        must_defend_pos = NULL;
        int not0 = 0;
        //初始化勝率最高的組合為找第一個試過的組合
        for (i = 0; i < 5 && !not0; i++) {
            for (j = 0; j < 5 && !not0; j++) {
                for (k = 0; k < 5 && !not0; k++) {
                    for(l = 0; l < 5 && !not0; l++) {
                        for (m = 0; m < 2 && !not0; m++) {
                            if (mytried[i][j][k][l][m] != 0) {
                                my_win_max[0] = i;
                                my_win_max[1] = j;
                                my_win_max[2] = k;
                                my_win_max[3] = l;
                                my_win_max[4] = m;
                                not0 = 1;
                            }
                        }
                    }
                }
            }
        }

        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                for (k = 0; k < 5; k++) {
                    for(l = 0; l < 5; l++) {
                        for (m = 0; m < 2; m++) {
                            if ((!verify_nowcan_win(myworker)) && must_defend_i != -1 && (k != must_defend_i || l != must_defend_j)) mytried[i][j][k][l][m] *= 50; //若此時對方有三樓可以走，且我不會這時候贏的話，必須假設我不可能贏，大幅下降此組合勝率
                            if ((!verify_nowcan_win(myworker)) && verify_oppcan_threefloor_after_mybuild(oppworker, k, l, i, j)) mytried[i][j][k][l][m] *= 10; //若我方蓋這格會送頭，降低勝率
                            if ((!verify_nowcan_win(myworker)) && (abs(i - myworker[1 - m].now_pos[0]) * abs(i - myworker[1 - m].now_pos[0]) + abs(j - myworker[1 - m].now_pos[1]) * abs(j - myworker[1 - m].now_pos[1]) <= 4)) mytried[i][j][k][l][m] *= 1.15; //若我方棋子靠太近 降低勝率
                            if ((!verify_nowcan_win(myworker)) && (abs(i - oppworker[m].now_pos[0]) * abs(i - oppworker[m].now_pos[0]) + abs(j - oppworker[m].now_pos[1]) * abs(j - oppworker[m].now_pos[1]) <= 4)) mytried[i][j][k][l][m] *= 0.85; //若我方棋子離對方較近 提高勝率
                            if (((float)mywin[i][j][k][l][m] / mytried[i][j][k][l][m] > (float)mywin[my_win_max[0]][my_win_max[1]][my_win_max[2]][my_win_max[3]][my_win_max[4]] / mytried[my_win_max[0]][my_win_max[1]][my_win_max[2]][my_win_max[3]][my_win_max[4]])&& mytried[i][j][k][l][m] != 0) { //找到勝率更高的組合
                                my_win_max[0] = i;
                                my_win_max[1] = j;
                                my_win_max[2] = k;
                                my_win_max[3] = l;
                                my_win_max[4] = m;
                            }
                        }
                    }
                }
            }
        }

        //實際移動工人到勝率最高的位置，蓋到勝率最高的位置
        now_worker_pos[my_win_max[0]][my_win_max[1]] = myworker[0].color;
        now_building_pos[my_win_max[2]][my_win_max[3]]++;
        now_worker_pos[myworker[my_win_max[4]].now_pos[0]][myworker[my_win_max[4]].now_pos[1]] = 0;
    }


    //寫檔
    FILE *focolor = fopen(argv[5], "w");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            fprintf(focolor, "%d", now_worker_pos[i][j]);
            if (j < 4) {
                fprintf(focolor, ",");
            }
        }
        fprintf(focolor, "\n");
    }
    fclose(focolor);
    FILE *fostructure = fopen(argv[6], "w");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            fprintf(fostructure, "%d", now_building_pos[i][j]);
            if (j < 4) {
                fprintf(fostructure, ",");
            }
        }
        fprintf(fostructure, "\n");
    }
    fclose(fostructure);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);
    return 0;
}

//模擬一局
void sim_one_game(struct worker my[2], struct worker opp[2], char* argv[]) {
    int myresult = 0, oppresult = 0; //回傳對於我方及對方的輸贏結果
    myresult = sim_one_action(my, argv); //我方動第一回合
    //紀錄此局我方第一步為何
    int myfirstmove_i = my[which_worker].sim_pos[0];
    int myfirstmove_j = my[which_worker].sim_pos[1];
    int myfirstbuild_i = my[which_worker].sim_build_pos[0];
    int myfirstbuild_j = my[which_worker].sim_build_pos[1];
    int myfirstworker = which_worker;
    int ratio = 1; //依回合加權的倍率

    while(myresult == 0 && oppresult == 0) { //還沒有結果
        oppresult = sim_one_action(opp, argv); //對方動第一回合
        roundnum++;
        if (oppresult != 0) break;
        myresult = sim_one_action(my, argv);
    }
    if (roundnum < 2) ratio = 10;
    else if (roundnum < 5) ratio = 6;
    else if (roundnum < 10) ratio = 3;
    else if (roundnum < 25) ratio = 1;
    else ratio = 0;
    if (myresult == 1 || oppresult == -1) { //若我方獲勝 加入贏的樣本數
        mywin[myfirstmove_i][myfirstmove_j][myfirstbuild_i][myfirstbuild_j][myfirstworker] += myresult * ratio;
        mywin[myfirstmove_i][myfirstmove_j][myfirstbuild_i][myfirstbuild_j][myfirstworker] -= oppresult * ratio;
    }
    mytried[myfirstmove_i][myfirstmove_j][myfirstbuild_i][myfirstbuild_j][myfirstworker] += ratio; //無論輸贏 皆加入總樣本數
    if (myfirstmove_i == 2 && myfirstmove_j == 1 && oppresult == 1) {
        printf("myresult = %d, oppresult = %d\n", myresult, oppresult);
    }
}

//模擬行動一回合
int sim_one_action (struct worker w[2], char* argv[]) {
    //模擬隨機取得一格可以移動的位置
    int mi = 0, mj = 0, bi = 0, bj = 0; //暫存原先位置樓層
    find_worker_valid_pos(w, 0); //找第一位工人能不能移動
    find_worker_valid_pos(w, 1); //找第二位工人能不能移動
    if (w[0].canmove != 0 || w[1].canmove != 0) { //若存在可移動的工人
        if (w[0].canmove != 0 && w[1].canmove != 0) which_worker = rand() % 2; //若兩個工人都可移動 隨機取一個
        else if (w[0].canmove != 0) which_worker = 0;
        else which_worker = 1;

        w[which_worker].god[1] = rand() % 2; //工人隨機發動能力
        while (1) {
            mi = w[which_worker].sim_pos[0] + rand() % 3 - 1;
            mj = w[which_worker].sim_pos[1] + rand() % 3 - 1;
            if ((mi != w[which_worker].sim_pos[0] || mj != w[which_worker].sim_pos[1]) && (sim_building_pos[mi][mj] < 4) && (sim_building_pos[mi][mj] - w[which_worker].sim_floor <= 1) && (sim_worker_pos[mi][mj] == 0) && mi < 5 && mj < 5 && mi >= 0 && mj >= 0) {
                sim_worker_pos[w[which_worker].sim_pos[0]][w[which_worker].sim_pos[1]] = 0; //原先工人所在的位置清空
                w[which_worker].sim_pos[0] = mi; //標記移動後的工人位置
                w[which_worker].sim_pos[1] = mj;
                w[which_worker].sim_floor = sim_building_pos[mi][mj];

                sim_worker_pos[mi][mj] = w[which_worker].color; //後來工人所在的位置標記成棋子顏色
                break;
            }
        }
    }
    else return -1; //回傳移動方輸

    //模擬隨機取得一格可以蓋建築的位置
    find_building_valid_pos(w, which_worker); //找移動的工人可不可蓋在周圍(不包含腳下)
    if (w[which_worker].canbuild != 0 || (w[which_worker].god[0] == 0 && w[which_worker].god[1] == 1 && sim_building_pos[mi][mj] < 3)) { //若周圍可以蓋，或可以發動Zeus且有發動能力
        if (w[which_worker].god[0] == 0 && w[which_worker].god[1] == 1 && sim_building_pos[mi][mj] < 3) { //若天神是Zeus，且有發動能力
            sim_building_pos[mi][mj]++;
            w[which_worker].sim_floor = sim_building_pos[mi][mj];
            w[which_worker].sim_build_pos[0] = mi;
            w[which_worker].sim_build_pos[1] = mj;
        }
        else {
            while (1) {
                bi = w[which_worker].sim_pos[0] + rand() % 3 - 1;
                bj = w[which_worker].sim_pos[1] + rand() % 3 - 1;
                if (sim_building_pos[bi][bj] < 4 && sim_worker_pos[bi][bj] == 0 && bi < 5 && bj < 5 && bi >= 0 && bj >= 0) {
                    sim_building_pos[bi][bj]++; //建造此位置
                    w[which_worker].sim_build_pos[0] = bi;
                    w[which_worker].sim_build_pos[1] = bj;
                    break;
                }
            }
        }
        
        //判斷移動方是否贏了，若當下使用Zeus則不算贏
        if (w[which_worker].sim_floor == 3 && !(w[which_worker].god[0] == 0 && w[which_worker].god[1] == 1)) return 1; //回傳移動方贏
        else return 0; //回傳還沒結束
    }
    else return -1; //回傳移動方輸
}

void find_worker_valid_pos (struct worker w[2], int which_w) {
    int i = w[which_w].sim_pos[0]; //模擬:工人現在的位置
    int j = w[which_w].sim_pos[1];

    int m = 0, n = 0; //九宮格範圍
    int a = -1, b = 1, c = -1, d = 1;
    w[which_w].canmove = 0; //初始化可移動格數

    if (i == 0) a++;
    if (i == 4) b--;
    if (j == 0) c++;
    if (j == 4) d--;
    
    for (m = a; m <= b; m++) {
        for (n = c; n <= d; n++) {
            if ((m == 0) && (n == 0)) continue;
            if ((sim_building_pos[i + m][j + n] - sim_building_pos[i][j] <= 1) && (sim_worker_pos[i + m][j + n] == 0) && (sim_building_pos[i + m][j + n] < 4)) {
                w[which_w].canmove++;
            }
        }
    }
}

void find_building_valid_pos (struct worker w[2], int which_w) {
    int i = w[which_w].sim_pos[0]; //模擬:工人現在的位置
    int j = w[which_w].sim_pos[1];

    int m = 0, n = 0; //九宮格範圍
    int a = -1, b = 1, c = -1, d = 1;
    w[which_w].canbuild = 0;

    if (i == 0) a++;
    if (i == 4) b--;
    if (j == 0) c++;
    if (j == 4) d--;
    
    for (m = a; m <= b; m++) {
        for (n = c; n <= d; n++) {
            if ((m == 0) && (n == 0)) continue;
            if ((sim_building_pos[i + m][j + n] < 4) && sim_worker_pos[i + m][j + n] == 0) {
                w[which_w].canbuild++;
            }
        }
    }
}

int verify_oppcan_threefloor_after_mybuild(struct worker w[2], int i, int j, int mi, int mj) {
    int which_w = 0;
    for (which_w = 0; which_w < 2; which_w++) {
        if ((i != w[which_w].now_pos[0] || j != w[which_w].now_pos[1]) && (now_building_pos[i][j] == 2) && w[which_w].now_floor == 2 && (mi != i || mj != j) && abs(i - w[which_w].now_pos[0]) <= 1 && abs(j - w[which_w].now_pos[1]) <= 1) {
            return 1;
        }
    }
    return 0;
}

int *verify_must_defend(struct worker w[2], struct worker myw[2]) { //mi != i, mj != j
    int which_w = 0;
    int i = 0, j = 0;
    int* must_defend_pos = malloc(2 * sizeof(int));
    must_defend_pos[0] = -1; //預設對方沒有立即上3樓的可能
    must_defend_pos[1] = -1;
    //判斷對方有哪個位置是一定要守的(在我守得到的前提下)
    for (which_w = 0; which_w < 2; which_w++) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                if ((now_worker_pos[i][j] == 0) && (now_building_pos[i][j] == 3) && w[which_w].now_floor == 2 && abs(i - w[which_w].now_pos[0]) <= 1 && abs(j - w[which_w].now_pos[1]) <= 1 && (verify_nowcan_block(myw, 0, i, j) || verify_nowcan_block(myw, 1, i, j))) {
                    must_defend_pos[0] = i;
                    must_defend_pos[1] = j;
                    printf("must defend: %d, %d\n", i, j);
                    return must_defend_pos;
                }   
            }
        }
    }
    return must_defend_pos;
}

int verify_nowcan_block(struct worker w[2], int which_w, int bi, int bj) {
    int i = w[which_w].now_pos[0]; //工人現在的位置
    int j = w[which_w].now_pos[1];

    int m = 0, n = 0; //九宮格範圍
    int a = -1, b = 1, c = -1, d = 1;
    int k = 0, l = 0;
    int canmove = 0;
    int index = 0;
    int movepos[8][2]; //紀錄可以移動的位置座標
    for (k = 0; k < 8; k++) {
        for (l = 0; l < 2; l++) {
            movepos[k][l] = -1;
        }
    }
    int canbuild = 0;

    if (i == 0) a++;
    if (i == 4) b--;
    if (j == 0) c++;
    if (j == 4) d--;

    //判定這個棋子能不能走
    for (m = a; m <= b; m++) {
        for (n = c; n <= d; n++) {
            if ((m == 0) && (n == 0)) continue;
            if ((now_building_pos[i + m][j + n] - now_building_pos[i][j] <= 1) && (now_worker_pos[i + m][j + n] == 0) && (now_building_pos[i + m][j + n] < 4)) {
                canmove++;
                movepos[index][0] = i + m;
                movepos[index][1] = j + n;
                index++;
            }
        }
    }
    for (k = 0; k < 8; k++) {
        if (movepos[k][0] == -1) continue;
        if ((now_building_pos[bi][bj] < 4) && now_worker_pos[bi][bj] == 0 && abs(bi - movepos[k][0]) <= 1 && abs(bj - movepos[k][1]) <= 1) canbuild++;
    }

    if (canmove && canbuild) return 1; //若可以移動且蓋得到對方要贏的位置，回傳我必須守這格
    else return 0;

}

int verify_nowcan_win(struct worker w[2]) {
    int which_w = 0;
    int i = 0, j = 0;
    //判斷我有哪個位置是一定要走的
    for (which_w = 0; which_w < 2; which_w++) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                if ((now_worker_pos[i][j] == 0) && (now_building_pos[i][j] == 3) && w[which_w].now_floor == 2 && abs(i - w[which_w].now_pos[0]) <= 1 && abs(j - w[which_w].now_pos[1]) <= 1) {
                    return 1;
                }   
            }
        }
    }
    return 0;
}